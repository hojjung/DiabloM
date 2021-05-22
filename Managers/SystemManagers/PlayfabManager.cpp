#include "PlayfabManager.h"
#include "InboxManager.h"
#include "JsonObjectConverter.h"
#include "JsonSerializer.h"
#include "MobileUtilsBlueprintLibrary.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "PlayFabClientDataModels.h"
#include "PlayFabAdminDataModels.h"
#include "PlayFabClientModels.h"
#include "PlayFabJsonHelpers.h"
#include "PlayFabJsonObject.h"
#include "PlayFabJsonValue.h"
#include "PlayFabServerDataModels.h"
#include "PlayFabUtilities.h"
#include "Managers/DiabloGameInstance.h"
#include "Objects/MyInAppPurchase.h"
#include "Misc/Base64.h"
#include "Core/PlayFabSettings.h"
#include "Save/SessionTicket.h"

#define LOCTEXT_NAMESPACE "PlayfabManager"
//dungeon 1111200
const FString UPlayfabManager::MainData = TEXT("MainData");      
const FString UPlayfabManager::MainDungeon = TEXT("MainDungeon");   
const FString UPlayfabManager::Quest = TEXT("Quest");         
const FString UPlayfabManager::Daily = TEXT("Daily");        
const FString UPlayfabManager::Gold = TEXT("Gold");
const FString UPlayfabManager::AdmobTime = TEXT("AdmobTime");
const FString UPlayfabManager::PVPStatus = TEXT("PVPStatus");


//
UPlayfabManager::UPlayfabManager()

{
	m_nRanking.SetValue(-123);
	
	m_CurrentVersionName=TEXT("0410NEEDFIX");

	m_bIsCustomID=false;
	//
	m_fDeltaInboxUpdateCooldown =160.f;

}

TSharedPtr<UPlayFabAuthenticationContext> UPlayfabManager::CreateAuthCon(const FString* newSessonTicket)
{
	if(newSessonTicket)//로그인후 새로 만드는거임
	{
		PlayFab::PlayFabSettings::SetClientSessionTicket(*newSessonTicket);
		m_SessionTicket = PlayFab::PlayFabSettings::GetClientSessionTicket();
		
		USessionTicket* SaveGameInstance = Cast<USessionTicket>(UGameplayStatics::CreateSaveGameObject(USessionTicket::StaticClass()));
		SaveGameInstance->m_SessionTicket = m_SessionTicket;
		
		UGameplayStatics::SaveGameToSlot(SaveGameInstance,TEXT("SessionTicket"),0);
		
			
		return TSharedPtr<UPlayFabAuthenticationContext>(NewObject<UPlayFabAuthenticationContext>(),DeleterNot());
	}
	else//로그인 하기전 로컬 로드
	{
		USessionTicket* LoadedSession = Cast<USessionTicket>(UGameplayStatics::LoadGameFromSlot(TEXT("SessionTicket"),0));

		if(LoadedSession && !LoadedSession->m_SessionTicket.IsEmpty())//로드 성공
		{
			PlayFab::PlayFabSettings::SetClientSessionTicket(LoadedSession->m_SessionTicket);
			m_SessionTicket = PlayFab::PlayFabSettings::GetClientSessionTicket();
			
			return TSharedPtr<UPlayFabAuthenticationContext>(NewObject<UPlayFabAuthenticationContext>(),DeleterNot());
		}
	}
	
	PRINTF("No Authcontext");

	return nullptr;
}

int UPlayfabManager::GetRanking()
{
	return m_nRanking.GetValue();
}

void UPlayfabManager::UploadMainData()
{
	
	
	PlayFab::ClientModels::FUpdateUserDataRequest Req;
	Req.Permission=PlayFab::ClientModels::UserDataPermission::UserDataPermissionPublic;
	// //
	Req.Data.Add(MainData,GetMainDataJsonStr());
	Req.Data.Add(Gold,UDiabloGameInstance::Get->m_GoldManager->GetGoldDataStr());
	
	GetClientAPI->UpdateUserData(Req,nullptr,
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
	
	m_fDeltaCountTitleData = 0.f;
}

void UPlayfabManager::UploadCachedDataToServer()
{
	UDiabloGameInstance::Get->m_QuestManager->UploadQuestData();
	UDiabloGameInstance::Get->m_EquipManager->UploadCachedWeaponStoneForServer();
	UDiabloGameInstance::Get->m_PlayerUpgradeManager->UploadCachedSkillStoneForServer();
	UDiabloGameInstance::Get->m_GoldManager->UploadCachedDgkeys();
}

void UPlayfabManager::RequestRefillDungeonKey()
{
	PlayFab::ClientModels::FExecuteCloudScriptRequest Req;
	
	Req.FunctionName = TEXT("RefillDgKeys");
	
	Req.GeneratePlayStreamEvent = true;
	
	GetClientAPI->ExecuteCloudScript(Req,FExeCScriptDele::CreateUObject(this, &UPlayfabManager::OnRefillDgKey),
		FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::TickTryUpdateUserData(float deltaTime)
{
	m_fDeltaCountTitleData += deltaTime;
	
	m_fDeltaCountRanking += deltaTime;
	
	m_fDeltaCountMinutePlaytime += deltaTime;

	m_fDeltaInboxUpdateCooldown += deltaTime;

	m_fDeltaInvenUpdate += deltaTime;

	if (m_fDeltaCountMinutePlaytime > 60.f)
	{
		UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::PlayTime);

		m_fDeltaCountMinutePlaytime=0;
	}

	 if (m_fDeltaCountTitleData > 50.f)
	 {
	 	PRINTF("TryUpdateUserData");
	// 	
	 	UploadMainData();
	 	UploadCachedDataToServer();
	 }

	if (m_fDeltaCountRanking > 220.f)
	{
		PRINTF("TryUpdateRank");
		RequestRetrieveTotalRanking();
		RequestRetrievePlayerAroundRanking();
		RequestRetrieveTotalPVPRanking();
		RequestRetrievePVPPlayerAroundRanking();
		//나의 랭킹 업데이트가 필요
		m_fDeltaCountRanking = 0.f;
	}

	if(m_fDeltaInvenUpdate>100.f)
	{
		RequestGetInventory();
	}

	UDiabloGameInstance::Get->m_AdverManager->Tick(deltaTime);
}

void UPlayfabManager::RequestSetNickname(FString str)
{
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Request Nickname", "닉네임 요청 전송"));
	PlayFab::ClientModels::FUpdateUserTitleDisplayNameRequest DisplayReq;

	DisplayReq.DisplayName = str;

	GetClientAPI->UpdateUserTitleDisplayName(DisplayReq,
	                                         PlayFab::UPlayFabClientAPI::FUpdateUserTitleDisplayNameDelegate::CreateUObject(
		                                         this, &UPlayfabManager::OnNickNameSetSuccess)
	                                         , PlayFab::FPlayFabErrorDelegate::CreateUObject(
		                                         this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnNickNameSetSuccess(const PlayFab::ClientModels::FUpdateUserTitleDisplayNameResult& result)
{
	//result.DisplayName
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Nickname Success", "닉네임 성공,기다려주세요"));
	m_bIsNicknameSet = true;
	m_LoadedNickname = result.DisplayName;

	RequestUploadNewPlayerData();
}

void UPlayfabManager::OnStageCompleteScriptSuccess(const FExeCScriptRslt& rslt)
{
	FString CachedJsonString = rslt.FunctionResult.toJSONString();

	TSharedPtr<FJsonObject> JsonObject; TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(CachedJsonString);
	
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return;	
	}
	FString Result = JsonObject->GetStringField(TEXT("Result"));
}

void UPlayfabManager::OnVersionCheckCloudScriptSuccess(const FExeCScriptRslt& rslt)
{
	FString CachedJsonString = rslt.FunctionResult.toJSONString();

	TSharedPtr<FJsonObject> JsonObject; TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(CachedJsonString);
	
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return;	
	}

	m_ServerVersion = JsonObject->GetStringField(TEXT("ServerVersionName"));
	
	if(m_CurrentVersionName == m_ServerVersion)
	{
		PRINTF("Version Same");
		RequestTitleNews();
		RequestGetMainData();
		RequestGetPVPData();
	}
	else
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Version Changed", "업데이트 해주세요!"));
		m_bIsVersionWrong = true;
	}
}

void UPlayfabManager::OnServerCloseCheckScriptSuccess(const FExeCScriptRslt& rslt)//CheckServerOpen
{
	FString CachedJsonString = rslt.FunctionResult.toJSONString();

	TSharedPtr<FJsonObject> JsonObject; TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(CachedJsonString);
	
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return;	
	}

	bool IsServerOpen = JsonObject->GetBoolField(TEXT("IsServerOpen"));
	
	m_bIsServerClosed = !IsServerOpen;

	m_ServerCloseOpenTime = JsonObject->GetStringField(TEXT("ServerCloseOpenTime"));

	if(!m_bIsServerClosed)
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Server Open", "서버 사용 가능"));
		
		RequestVersionCheck();
	}
	else
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Server Closed", "서버 사용 불가"));
	}

	//ServerCloseOpenTime
}

void UPlayfabManager::OnNewPlayerDataInitSuccess(const FExeCScriptRslt& rslt)
{
	
	RequestServerOpenCheck();
}

void UPlayfabManager::OnRefillDgKey(const FExeCScriptRslt& rslt)
{
	//Update
	UDiabloGameInstance::Get->m_GoldManager->SetDgKeys(20);
}

void UPlayfabManager::OnInboxRefreshSuccess(const FExeCScriptRslt& rslt)
{
	FString CachedJsonString = rslt.FunctionResult.toJSONString();

	UpdateInboxListToClient(CachedJsonString);
}


void UPlayfabManager::Init()
{
	if (m_bLoginProcessStarted)
	{
		return;
	}

	m_bLoginProcessStarted = true;

	m_AryIAPData.Reset();
	m_TitleNews.Reset();
	m_TotalStageRanking.Reset();
	m_PlayerStageRanking.Reset();
	m_TotalPVPRanking.Reset();
	m_PlayerPVPRanking.Reset();
	m_MapCatalogItems.Reset();

	if (UMobileUtilsBlueprintLibrary::CheckInternetConnection())
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Internet Connected", "인터넷 접속 성공"));
	}
	else
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("FAIL-Internet Fail-EndApp", "인터넷 접속 실패"));

		FGenericPlatformMisc::RequestExit(true);
		return;
	}

	GetClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();


	m_Auth = CreateAuthCon();

	if(m_Auth.IsValid())
	{
		//m_LastLoginTime = m_CurrentTime;
		m_PlayfabID = m_Auth.Get()->GetPlayFabId();

		RequestGetServerTime();

		return;
	}

	StartPlayfabLogin();

}

void UPlayfabManager::OnSessionLoginErrorPlayfabReq(const FFailRslt& ErrorResult)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	if (!Subsystem)
	{
		return;
	}
	
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Try Login With Android","로그인 세션 만료,구글로그인 시도"));
	
	IOnlineExternalUIPtr ExternalUi = Subsystem->GetExternalUIInterface();

	if (!ExternalUi)
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("FAIL-GoogleLoginFail-1","FAIL-GoogleLoginFail-1"));
		return;
	}

	ExternalUi->ShowLoginUI(0, false, false,FOnLoginUIClosedDelegate::CreateUObject(this, &UPlayfabManager::HandleExternalUIClose));
}

void UPlayfabManager::StartPlayfabLogin()
{
	
#if PLATFORM_WINDOWS
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Try Login With Desktop", "로그인 시도-PC"));
	

	PlayFab::ClientModels::FLoginWithCustomIDRequest request;
	request.CreateAccount = true;
	request.CustomId = TEXT("JungPC TestID3");
	request.TitleId = GetDefault<UPlayFabRuntimeSettings>()->TitleId;
	
	request.AuthenticationContext =	m_Auth;
	
	m_bIsCustomID = true;
	bool Result = GetClientAPI->LoginWithCustomID(request,
												PlayFab::UPlayFabClientAPI::FLoginWithGoogleAccountDelegate::CreateUObject(
													this, &UPlayfabManager::OnSuccessPlayfabLogin),
												PlayFab::FPlayFabErrorDelegate::CreateUObject(
													this, &UPlayfabManager::OnErrorPlayfabReq)
	);


#endif

	
#if PLATFORM_ANDROID
	//SessionTicket 로그인 시도
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Try Login With Session TIcket","로그인 시도-Google"));

	PlayFab::ClientModels::FLoginWithGoogleAccountRequest SessionRequest;
	
	SessionRequest.CreateAccount = false;
	
	SessionRequest.TitleId = GetDefault<UPlayFabRuntimeSettings>()->TitleId;
	
	SessionRequest.AuthenticationContext = m_Auth;

	GetClientAPI->LoginWithGoogleAccount(SessionRequest,
													PlayFab::UPlayFabClientAPI::FLoginWithGoogleAccountDelegate::CreateUObject(
														this, &UPlayfabManager::OnSuccessPlayfabLogin),
													PlayFab::FPlayFabErrorDelegate::CreateUObject(
														this, &UPlayfabManager::OnSessionLoginErrorPlayfabReq)
	);
#endif
}

void UPlayfabManager::RequestUploadNewPlayerData()
{
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Player Data Init", "플레이어 신규데이터 작성중"));
	PlayFab::ClientModels::FExecuteCloudScriptRequest Req;
	
	Req.FunctionName = TEXT("InitPlayerTitleData");
	
	Req.GeneratePlayStreamEvent = true;
	
	GetClientAPI->ExecuteCloudScript(Req,FExeCScriptDele::CreateUObject(this, &UPlayfabManager::OnNewPlayerDataInitSuccess),
		FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::RequestPVPMatching(int aroundCount,PlayFab::UPlayFabClientAPI::FGetLeaderboardAroundPlayerDelegate completeDele)
{
	PlayFab::ClientModels::FGetLeaderboardAroundPlayerRequest Req;
	Req.StatisticName=TEXT("PVP_MMR");
	Req.MaxResultsCount=aroundCount;
	Req.PlayFabId = m_PlayfabID;
	//PlayFab::UPlayFabClientAPI::FGetLeaderboardAroundPlayerDelegate::CreateUObject(this,&UPlayfabManager::OnSuccessGetPlayerAroundRanking
	GetClientAPI->GetLeaderboardAroundPlayer(Req,completeDele,
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::UploadAdmobTime(const FDateTime& date_time)
{
	FUpdateReq Req;
	
	Req.Data.Add(AdmobTime,date_time.ToString());

	GetClientAPI->UpdateUserData(Req,nullptr,
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}


void UPlayfabManager::HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex,
                                            const FOnlineError& error)
{

	if (error.bSucceeded)
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("SUCCESS-GoogleLogin", "구글 로그인 성공01"));
		TryLoginPlayfabGoogle(uniqueId);
		
	}
	else
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("FAIL-GoogleLoginFail-2", "실패-앱을 종료후 구글 계정 로그인 먼저해주세요"));
		//FGenericPlatformMisc::RequestExit(true);
	}
}

void UPlayfabManager::TryLoginPlayfabGoogle(TSharedPtr<const FUniqueNetId> uniqueId) //클라아이디를 키서명으로 바꿔봄
{
	if (UMobileUtilsBlueprintLibrary::CheckGooglePlayServices())
	{
		IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

		IOnlineIdentityPtr OnlineIdentity = Subsystem->GetIdentityInterface();

		auto Status = OnlineIdentity->GetLoginStatus(0);

		switch (Status)
		{
		case ELoginStatus::NotLoggedIn:
			UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("FAIL-LoginStatus:NotLoggedin",
			                                                   "FAIL-LoginStatus:NotLoggedin"));
			break;
		case ELoginStatus::UsingLocalProfile:
			UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("FAIL-LoginStatus:UsingLocalProfile",
			                                                   "FAIL-LoginStatus:UsingLocalProfile"));
			break;
		case ELoginStatus::LoggedIn:
			UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("LoginStatus:LoggedIn", "LoginStatus:LoggedIn"));
			break;
		default: ;
		}

		

		PlayFab::ClientModels::FLoginWithGoogleAccountRequest request;
		request.CreateAccount = true;
		request.ServerAuthCode = OnlineIdentity->GetAuthToken(0);
		//request.PlayerSecret = GetDefault<UPlayFabRuntimeSettings>()->DeveloperSecretKey;
		request.TitleId = GetDefault<UPlayFabRuntimeSettings>()->TitleId;

		bool Result = GetClientAPI->LoginWithGoogleAccount(request,
		                                                   PlayFab::UPlayFabClientAPI::FLoginWithGoogleAccountDelegate::CreateUObject(
			                                                   this, &UPlayfabManager::OnSuccessPlayfabLogin),
		                                                   PlayFab::FPlayFabErrorDelegate::CreateUObject(
			                                                   this, &UPlayfabManager::OnErrorPlayfabReq)
		);


		if (!Result)
		{
			UDiabloGameInstance::Get->RequestPopupText(
				LOCTEXT("Fail-Request PlayfabLogin", "Fail-Request PlayfabLogin"));
		}
	}
	else
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("FAIL-GooglePlay not checked ?",
		                                                   "FAIL-GooglePlay not checked ?"));
	}
}

void UPlayfabManager::OnSuccessPlayfabLogin(const PlayFab::ClientModels::FLoginResult& Result)
{
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("SUCCESS-Playfab Login Success", "구글 로그인 성공02"));

	FString SeTicket = Result.SessionTicket;

	m_Auth = CreateAuthCon(&SeTicket);

	m_PlayfabID = Result.PlayFabId;

	m_LastLoginTime = Result.LastLoginTime;
	
	FTimespan KoreanTime(9,0,0);
	
	m_LastLoginTime+=KoreanTime;
	
	m_bIsNewCreatePlayer = Result.NewlyCreated;

	if(m_bIsNewCreatePlayer)//닉네임 설정떠야함
	{
		if(m_bIsCustomID)
		{
			
		}
		//RequestUploadNewPlayerData();
		//Create Player's TitleData,플레이펩 오토메이션은 없어져야함
		//그리고 그 모든것이 끝났을때
	}
	RequestGetServerTime();
	//
	//RequestGetAccountInfo();
}

void UPlayfabManager::RequestGetMainData()
{
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Request Get User Data01", "유저 데이터 가져오는 중 01"));
	FGetUsrDataReq req;

	//Request Data
	req.PlayFabId = m_PlayfabID;
	
	req.Keys.Add(MainData);
	req.Keys.Add(MainDungeon);
	req.Keys.Add(Quest);
	req.Keys.Add(Daily);
	req.Keys.Add(Gold);
	req.Keys.Add(AdmobTime);
	//다른데이터?
	GetClientAPI->GetUserData(req,FGetUsrDataDele::CreateUObject(this, &UPlayfabManager::OnSuccessGetMainData),
		FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));

	
}

void UPlayfabManager::RequestGetOtherPlayerMainData(const FString& playfabID,FGetUsrDataDele onSuccess)
{
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Request Get OtherPlayer Data", "타 유저 데이터 가져오는 중"));
	FGetUsrDataReq req;
	//Request Data
	req.PlayFabId = playfabID;
	req.Keys.Add(MainData);
	
	GetClientAPI->GetUserData(req,onSuccess,
		FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::RequestClaimInbox(int index)//
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetNumberField(TEXT("Index"), index);

	PlayFab::ClientModels::FExecuteCloudScriptRequest Req;
	
	Req.FunctionParameter = PlayFab::FJsonKeeper(JsonObject);
	
	Req.FunctionName = TEXT("ClaimInbox");
	
	GetClientAPI->ExecuteCloudScript(Req,FExeCScriptDele::CreateUObject(this, &UPlayfabManager::OnInboxRefreshSuccess),FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::RequestClaimAllInbox()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	PlayFab::ClientModels::FExecuteCloudScriptRequest Req;
	
	Req.FunctionParameter = PlayFab::FJsonKeeper(JsonObject);
	
	Req.FunctionName = TEXT("ClaimAllInbox");
	
	GetClientAPI->ExecuteCloudScript(Req,FExeCScriptDele::CreateUObject(this, &UPlayfabManager::OnInboxRefreshSuccess),FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

bool UPlayfabManager::RequestInboxList()
{
	if(m_fDeltaInboxUpdateCooldown<150)
	{
		return false;
	}
	PRINTF("PlayfabManager-InboxRequest");
	
	m_fDeltaInboxUpdateCooldown =0.f;
	
	PlayFab::ClientModels::FExecuteCloudScriptRequest Req;
	
	Req.FunctionName = TEXT("RefreshInbox");
	
	GetClientAPI->ExecuteCloudScript(Req,FExeCScriptDele::CreateUObject(this, &UPlayfabManager::OnInboxRefreshSuccess),FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
	//
	return true;
}


void UPlayfabManager::OnErrorPlayfabReq(const FFailRslt& ErrorResult)
{
	FString CodeString = UPlayFabUtilities::getErrorText(ErrorResult.ErrorCode);
	
	UDiabloGameInstance::Get->RequestPopupText(CodeString);
	
	if(1074 ==ErrorResult.ErrorCode ||CodeString.IsEmpty())
	{
		StartPlayfabLogin();
		return;	
	}
	//break;
	m_OnPlayfabError.Broadcast(CodeString);
}

FDateTime UPlayfabManager::DecodePlayfabTimeToUe4Time(FString playfabTime)
{
	FDateTime DateTimeOut;
	FString DateString = playfabTime;
	
	if (!FDateTime::ParseIso8601(*DateString, DateTimeOut))
	{
		PRINTF("TimeChangeFail");
	}

	return DateTimeOut;
}

FString UPlayfabManager::GetMainDataJsonStr()
{
	UPlayFabJsonObject* TotalMaindataJsonObj = UPlayFabJsonObject::ConstructJsonObject(this);
	//Logout
	TotalMaindataJsonObj->SetStringField(TEXT("LogoutTime"),FDateTime::UtcNow().ToString());
	//IAP
	UDiabloGameInstance::Get->m_ShopManager->SetIAPDataToJson(TotalMaindataJsonObj);
	//Gacha
	UDiabloGameInstance::Get->m_GachaManager->SetGachaDataToJson(TotalMaindataJsonObj);
	//Upgrade
	UDiabloGameInstance::Get->m_PlayerUpgradeManager->SetUpgradeDataToJson(TotalMaindataJsonObj);
	//Skill
	UDiabloGameInstance::Get->m_PlayerUpgradeManager->SetSkillDataToJson(TotalMaindataJsonObj);
	//Weapon
	UDiabloGameInstance::Get->m_EquipManager->SetWeaponDataToJson(TotalMaindataJsonObj);
	//Skin
	UDiabloGameInstance::Get->m_EquipManager->SetSkinDataToJson(TotalMaindataJsonObj);

	UDiabloGameInstance::Get->m_EquipManager->SetWingDataToJson(TotalMaindataJsonObj);
	//Pet
	UDiabloGameInstance::Get->m_EquipManager->SetPetDataToJson(TotalMaindataJsonObj);
	//
	UPlayFabJsonObject* CurrentEquippedObj = UPlayFabJsonObject::ConstructJsonObject(this);

	CurrentEquippedObj->SetNumberField(TEXT("EquippedSkin"),UDiabloGameInstance::Get->m_EquipManager->m_nSelectedSkin);
	CurrentEquippedObj->SetNumberField(TEXT("EquippedWeapon"),UDiabloGameInstance::Get->m_EquipManager->m_nSelectedWeapon);
	CurrentEquippedObj->SetNumberField(TEXT("EquippedWeaponLevel"),UDiabloGameInstance::Get->m_EquipManager->GetCurrentWeapon().Level);
	CurrentEquippedObj->SetNumberField(TEXT("EquippedPet"),UDiabloGameInstance::Get->m_EquipManager->m_nSelectedPet);
	CurrentEquippedObj->SetNumberField(TEXT("EquippedPetLevel"),UDiabloGameInstance::Get->m_EquipManager->GetCurrentPet()?UDiabloGameInstance::Get->m_EquipManager->GetCurrentPet()->Level:-1);
	
	TotalMaindataJsonObj->SetObjectField(TEXT("CurrentEquipped"),CurrentEquippedObj);

	//TotalMaindataJsonObj->SetStringField(TEXT("PVPStatus"),);
	//
	return TotalMaindataJsonObj->EncodeJson();
}

void UPlayfabManager::OnSuccessGetMainData(const FGetUsrDataRslt& result)
{
	if (!result.Data.Num())
	{
		PRINTF("DataNull");
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("FAIL-Get User Data Null01", "실패-유저 데이터 획득 없음01"));
	}
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("SUCCESS Get User Data", "게임 준비 완료"));
	SetMainDataToManagers(result.Data[MainData].Value);
	
	if(FDateTime::Parse(result.Data[AdmobTime].Value,m_LastAdmobTime))
	{
		FTimespan KoreanTime(9,0,0);
		m_LastAdmobTime+=KoreanTime;
		UDiabloGameInstance::Get->m_AdverManager->SetTimeCooldownFromServer(m_LastAdmobTime,m_CurrentTime);
	}
	//
	UDiabloGameInstance::Get->m_NormalDgManager->SetDungeonData(result.Data[MainDungeon].Value);
	UDiabloGameInstance::Get->m_QuestManager->SetQuestDataFromServer(result.Data[Quest].Value);
	UDiabloGameInstance::Get->m_DailyManager->SetPrizeManager(result.Data[Daily].Value,m_CurrentTime,m_bIsNewCreatePlayer);
	UDiabloGameInstance::Get->m_GoldManager->SetCurrentGold(result.Data[Gold].Value,m_bIsNewCreatePlayer,m_CurrentTime,m_LastLoginTime,m_LastLogoutTime);
	
	m_bIsLoginCompleted = true;
	m_bIsNicknameSet = true;
	RequestCatalogItems();
	RequestRetrievePlayerAroundRanking();
	RequestRetrieveTotalRanking();
	RequestRetrieveTotalPVPRanking();
	RequestRetrievePVPPlayerAroundRanking();
}

void UPlayfabManager::RequestCatalogItems()
{
	PlayFab::ClientModels::FGetCatalogItemsRequest Req;
	GetClientAPI->GetCatalogItems(Req, PlayFab::UPlayFabClientAPI::FGetCatalogItemsDelegate::
	                              CreateLambda([&](const PlayFab::ClientModels::FGetCatalogItemsResult cIRslt)
	                              {
		                              for(const PlayFab::ClientModels::FCatalogItem& CatalogItem : cIRslt.Catalog)
		                              {
			                              m_MapCatalogItems.Add(CatalogItem.ItemId,CatalogItem);      
		                              }
	                              }), FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnSuccessGetUserData02(const FGetUsrDataRslt& result)
{

}

void UPlayfabManager::RequestGetAccountInfo()
{
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("RequestGetAccountInfo", "계정 정보 요청"));
	FGetAccntInfoReq Req;

	GetClientAPI->GetAccountInfo(Req, FGetAccntInfoDele::CreateUObject(this, &UPlayfabManager::OnSuccessGetAccountInfo),
	                             FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnSuccessGetAccountInfo(const FGetAccntInfoRslt& rslt)
{
	if (rslt.AccountInfo->TitleInfo->isBanned)
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Banned Player", "정지된 플레이어"));
		return;
	}

	if (rslt.AccountInfo->TitleInfo->DisplayName.IsEmpty())
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Please Set Nickname", "닉네임을 설정해주세요"));
		m_bShowNicknameSet = true;
		return;
	}

	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Welcome", "환영 합니다"));

	m_LoadedNickname = rslt.AccountInfo->TitleInfo->DisplayName;
	m_bIsNicknameSet = true;
	//RequestGetUserData();

	RequestServerOpenCheck();
}



void UPlayfabManager::BuyIAP(FString itemId, bool bIsConsumable)
{
	if(!m_MapCatalogItems.Find(itemId))
	{
		UDiabloGameInstance::Get->RequestPopupText(TEXT("엥? 아이템이 없는뎁쇼?-01"));	
		return;
	}
	
	FInAppPurchaseProductRequest IAPRequest;

	IAPRequest.bIsConsumable = bIsConsumable;

	IAPRequest.ProductIdentifier = itemId; //
	
	//FInAppPurchaseReceiptInfo
	
	UMyInAppPurchase* Proxy = UMyInAppPurchase::CreateProxyObjectForInAppPurchase(
		UDiabloGameInstance::Get->GetPlCon(), IAPRequest);

	Proxy->OnSuccess.AddDynamic(this, &UPlayfabManager::PurchaseSuccess);

	Proxy->OnFailure.AddDynamic(this, &UPlayfabManager::PurchaseFail);

}

void UPlayfabManager::PurchaseVirtualItem(FString itemUniqueId)
{
	if(!m_MapCatalogItems.Find(itemUniqueId))
	{
		UDiabloGameInstance::Get->RequestPopupText(TEXT("엥? 아이템이 없는뎁쇼?-02"));	
		return;
	}

	PlayFab::ClientModels::FCatalogItem& ItemWant = m_MapCatalogItems[itemUniqueId];
	
	PlayFab::ClientModels::FPurchaseItemRequest Req;
	Req.VirtualCurrency=TEXT("GG");
	Req.Price = ItemWant.VirtualCurrencyPrices[TEXT("GG")];
	Req.CatalogVersion = ItemWant.CatalogVersion;
	Req.CharacterId = m_PlayfabID;
	Req.ItemId = ItemWant.ItemId;
	
	GetClientAPI->PurchaseItem(Req,PlayFab::UPlayFabClientAPI::FPurchaseItemDelegate::CreateLambda(
		[&](const PlayFab::ClientModels::FPurchaseItemResult& rslt)
	{
			RequestGetInventory();
			
	}));
}


void UPlayfabManager::PurchaseSuccess(EInAppPurchaseState::Type completionStatus,const FInAppPurchaseProductInfo& inAppPurchaseInformation)
{
	UDiabloGameInstance::Get->RequestPopupText("IAP Purchase Need Validate");
	
	TSharedPtr<FJsonObject> JsonObject; TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(inAppPurchaseInformation.ReceiptData);
	
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return;	
	}

	FString ReceiptData;
	FBase64::Decode(JsonObject->GetStringField(TEXT("receiptData")),ReceiptData);
	
	FString Signature = JsonObject->GetStringField(TEXT("signature"));
	//
	PlayFab::ClientModels::FValidateGooglePlayPurchaseRequest GooglePlayReq;
	GooglePlayReq.CurrencyCode = inAppPurchaseInformation.CurrencyCode;
	GooglePlayReq.ReceiptJson = ReceiptData;
	GooglePlayReq.Signature = Signature;
	GooglePlayReq.PurchasePrice = inAppPurchaseInformation.RawPrice;
	//
	GetClientAPI->ValidateGooglePlayPurchase(GooglePlayReq,PlayFab::UPlayFabClientAPI::
		FValidateGooglePlayPurchaseDelegate::CreateUObject(this,&UPlayfabManager::OnIAPGoogleValidateSuccess),
		FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::PurchaseFail(EInAppPurchaseState::Type completionStatus,const FInAppPurchaseProductInfo& inAppPurchaseInformation)
{
	UDiabloGameInstance::Get->RequestPopupText(TEXT("결제 실패!"));
}

void UPlayfabManager::UploadNormalDungeon()
{
	int MaxStage = UDiabloGameInstance::Get->m_NormalDgManager->m_nMyMaxStageLevel.GetValue();

	PRINTF("UploadMaxStage:%d",MaxStage);
	
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetNumberField(TEXT("stageLevel"), MaxStage);

	PlayFab::ClientModels::FExecuteCloudScriptRequest Req;
	
	Req.FunctionParameter = PlayFab::FJsonKeeper(JsonObject);
	
	Req.FunctionName = TEXT("OnCompleteLevel");
	
	Req.GeneratePlayStreamEvent = true;
	
	GetClientAPI->ExecuteCloudScript(Req,FExeCScriptDele::CreateUObject(this, &UPlayfabManager::OnStageCompleteScriptSuccess),FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));

	UDiabloGameInstance::Get->m_QuestManager->UploadQuestData();
}

void UPlayfabManager::RequestVersionCheck()
{
	PlayFab::ClientModels::FExecuteCloudScriptRequest Req;
	
	Req.FunctionName =TEXT( "CheckVersion");
	
	Req.GeneratePlayStreamEvent = true;
	
	GetClientAPI->ExecuteCloudScript(Req,FExeCScriptDele::CreateUObject(this, &UPlayfabManager::OnVersionCheckCloudScriptSuccess),
		FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::RequestServerOpenCheck()
{
	PlayFab::ClientModels::FExecuteCloudScriptRequest Req;
	
	Req.FunctionName =TEXT( "CheckServerOpen");
	
	Req.GeneratePlayStreamEvent = true;
	
	GetClientAPI->ExecuteCloudScript(Req,FExeCScriptDele::CreateUObject(this, &UPlayfabManager::OnServerCloseCheckScriptSuccess),
        FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnIAPGoogleValidateSuccess(const PlayFab::ClientModels::FValidateGooglePlayPurchaseResult& purchaseResult)
{
	FString ItemID = purchaseResult.Fulfillments[0].FulfilledItems[0].ItemId;

	UDiabloGameInstance::Get->RequestPopupText(FString::Printf(TEXT("결제성공.아이템적용 진행중")));

	UDiabloGameInstance::Get->m_ShopManager->OnPurchasedGainItem(ItemID,true);
}

void UPlayfabManager::RequestGetInventory()
{
	m_fDeltaInvenUpdate=0;
	PlayFab::ClientModels::FGetUserInventoryRequest Req;
	GetClientAPI->GetUserInventory(Req,
		PlayFab::UPlayFabClientAPI::FGetUserInventoryDelegate::CreateUObject(this,&UPlayfabManager::OnSuccessGetInven),
		FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnSuccessGetInven( const PlayFab::ClientModels::FGetUserInventoryResult& rslt)
{
	UDiabloGameInstance::Get->m_GoldManager->SetGemStones(rslt.VirtualCurrency[TEXT("GG")]);
	UDiabloGameInstance::Get->m_GoldManager->SetDgKeys(rslt.VirtualCurrency[TEXT("KK")]);
	UDiabloGameInstance::Get->m_GoldManager->SetPetTickets(rslt.VirtualCurrency[TEXT("PT")]);
	UDiabloGameInstance::Get->m_GoldManager->SetSkillStones(rslt.VirtualCurrency[TEXT("SS")]);
	UDiabloGameInstance::Get->m_GoldManager->SetWeaponStones(rslt.VirtualCurrency[TEXT("WS")]);
	UDiabloGameInstance::Get->m_GoldManager->SetWingTickets(rslt.VirtualCurrency[TEXT("WT")]);
}

void UPlayfabManager::OnSuccessTimeGet(const PlayFab::ClientModels::FGetTimeResult& rslt)
{
	m_CurrentTime = rslt.Time;
	FTimespan KoreanTime(9,0,0);
	m_CurrentTime+=KoreanTime;

	

	RequestGetAccountInfo();
}

void UPlayfabManager::RequestRetrieveTotalRanking()
{
	PlayFab::ClientModels::FGetLeaderboardRequest Req;
	
	Req.StatisticName=TEXT("StageLevel");
	Req.StartPosition = 0;
	Req.MaxResultsCount = 100;

	GetClientAPI->GetLeaderboard(Req,PlayFab::UPlayFabClientAPI::FGetLeaderboardDelegate::CreateUObject(this,&UPlayfabManager::OnSuccessGetTotalRanking),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnSuccessGetTotalRanking(const PlayFab::ClientModels::FGetLeaderboardResult& rslt)
{
	m_TotalStageRanking = rslt.Leaderboard;

	m_OnTotalStageRankReceived.Broadcast(m_TotalStageRanking);
}

void UPlayfabManager::OnSuccessGetTotalPVPRanking(const PlayFab::ClientModels::FGetLeaderboardResult& rslt)
{
	m_TotalPVPRanking = rslt.Leaderboard;

	m_OnTotalPVPRankReceived.Broadcast(m_TotalPVPRanking);
}

void UPlayfabManager::RequestRetrievePlayerAroundRanking()
{
	PlayFab::ClientModels::FGetLeaderboardAroundPlayerRequest Req;
	Req.StatisticName=TEXT("StageLevel");
	Req.MaxResultsCount=1;
	Req.PlayFabId = m_PlayfabID;
	
	GetClientAPI->GetLeaderboardAroundPlayer(Req,PlayFab::UPlayFabClientAPI::FGetLeaderboardAroundPlayerDelegate::CreateUObject(this,&UPlayfabManager::OnSuccessGetPlayerAroundRanking),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::RequestRetrieveTotalPVPRanking()
{
	PlayFab::ClientModels::FGetLeaderboardRequest Req;
	
	Req.StatisticName=TEXT("PVP_MMR");
	Req.StartPosition = 0;
	Req.MaxResultsCount = 100;

	GetClientAPI->GetLeaderboard(Req,PlayFab::UPlayFabClientAPI::FGetLeaderboardDelegate::CreateUObject(this,&UPlayfabManager::OnSuccessGetTotalPVPRanking),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::RequestRetrievePVPPlayerAroundRanking()
{
	PlayFab::ClientModels::FGetLeaderboardAroundPlayerRequest Req;
	Req.StatisticName=TEXT("PVP_MMR");
	Req.MaxResultsCount=1;
	Req.PlayFabId = m_PlayfabID;
	
	GetClientAPI->GetLeaderboardAroundPlayer(Req,PlayFab::UPlayFabClientAPI::FGetLeaderboardAroundPlayerDelegate::CreateUObject(this,&UPlayfabManager::OnSuccessGetPVPPlayerAroundRanking),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::RequestGetServerTime()
{
	PlayFab::ClientModels::FGetTimeRequest Req;
	GetClientAPI->GetTime(Req,PlayFab::UPlayFabClientAPI::FGetTimeDelegate::CreateUObject(this,&UPlayfabManager::OnSuccessTimeGet),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::RequestTitleNews()
{
	PlayFab::ClientModels::FGetTitleNewsRequest Req;
	Req.Count = 5;
	GetClientAPI->GetTitleNews(Req,PlayFab::UPlayFabClientAPI::FGetTitleNewsDelegate::CreateUObject(this,&UPlayfabManager::OnSuccessGetTitleNews),PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::UpdateInboxListToClient(FString InboxListStr)
{
	TArray<FInboxSpec> AryInbox;
	
	if(!FJsonObjectConverter::JsonArrayStringToUStruct(InboxListStr, &AryInbox, 0, 0))
	{
		UDiabloGameInstance::Get->m_InboxManager->SetInboxManager(AryInbox);//실패시 그냥 빈배열줌
		return;
	}
	
	AryInbox.Reserve(20);

	FTimespan KoreanTime(9,0,0);

	for(auto& InboxRef : AryInbox)
	{
		FDateTime ExpireTime = DecodePlayfabTimeToUe4Time(InboxRef.ExpireTime);
		
		ExpireTime+=KoreanTime;
		
		FTimespan OfflineTimeSpawn = ExpireTime - m_CurrentTime;

		int Hours =  OfflineTimeSpawn.GetTotalHours();

		if(Hours>0)
		{
			InboxRef.m_StrRemainTime =  FString::Printf(TEXT("%d시간"),Hours);
		}
		else
		{
			int Minutes = OfflineTimeSpawn.GetTotalMinutes();
			
			if(Minutes<1)
			{
				PRINTF("Minutes Expired");
				InboxRef.m_bIsExpired = true;
			}
			else
			{
				InboxRef.m_StrRemainTime =  FString::Printf(TEXT("%d분"),Hours);	
			}
		}
	}
	
	UDiabloGameInstance::Get->m_InboxManager->SetInboxManager(AryInbox);
}


void UPlayfabManager::OnSuccessGetPlayerAroundRanking(const PlayFab::ClientModels::FGetLeaderboardAroundPlayerResult& rslt)
{
	m_PlayerStageRanking = rslt.Leaderboard;

	m_nRanking.SetValue(m_PlayerStageRanking[0].Position+1);

	m_OnPlayerStageRankReceived.Broadcast(m_PlayerStageRanking);
}

void UPlayfabManager::OnSuccessGetPVPPlayerAroundRanking(
	const PlayFab::ClientModels::FGetLeaderboardAroundPlayerResult& rslt)
{
	m_PlayerPVPRanking = rslt.Leaderboard;

	m_nPVPRanking.SetValue(m_PlayerPVPRanking[0].Position+1);

	m_OnPlayerPVPRankReceived.Broadcast(m_PlayerPVPRanking);
}

void UPlayfabManager::OnSuccessGetTitleNews(const PlayFab::ClientModels::FGetTitleNewsResult& rslt)
{
	m_TitleNews=rslt.News;
}

void UPlayfabManager::RequestCheatAlert()
{
	PlayFab::ClientModels::FExecuteCloudScriptRequest Req;
	Req.FunctionName = TEXT("CheatAlert");
	Req.GeneratePlayStreamEvent=true;
	GetClientAPI->ExecuteCloudScript(Req);
}

void UPlayfabManager::PurchaseWithGemStone(int amount,FString itemName)
{
	PlayFab::ClientModels::FPurchaseItemRequest Req;
	Req.Price = amount;
	Req.ItemId = itemName;
	Req.VirtualCurrency=TEXT("GG");
	GetClientAPI->PurchaseItem(Req,PlayFab::UPlayFabClientAPI::FPurchaseItemDelegate::CreateUObject(this,&UPlayfabManager::OnPurchaseWithVirtualCurrencySuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnPurchaseWithVirtualCurrencySuccess(const PlayFab::ClientModels::FPurchaseItemResult& rslt)
{
	FString PurchasedItemID =  rslt.Items[0].ItemId;

	UDiabloGameInstance::Get->m_ShopManager->OnPurchasedGainItem(PurchasedItemID,true);

}

void UPlayfabManager::AddGemStone(int amount)
{
	PlayFab::ClientModels::FAddUserVirtualCurrencyRequest Req;
	Req.Amount=amount;
	Req.VirtualCurrency=TEXT("GG");
	GetClientAPI->AddUserVirtualCurrency(Req,PlayFab::UPlayFabClientAPI::FAddUserVirtualCurrencyDelegate::CreateUObject(this,&UPlayfabManager::OnAddGemStone),PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::PurchaseWithPetTicket(int amount)
{
	FString ItemID;
	
	if(amount==1)
	{
		ItemID = TEXT("gachapet01");
	}
	else if(amount==11)
	{
		ItemID = TEXT("gachapet11");
	}
	
	PlayFab::ClientModels::FPurchaseItemRequest Req;
	Req.Price = amount;
	Req.ItemId = ItemID;
	Req.VirtualCurrency=TEXT("PT");
	//Req.CharacterId = m_PlayfabID;
	//FPurchaseItemDelegate, const ClientModels::FPurchaseItemResult&
	GetClientAPI->PurchaseItem(Req,PlayFab::UPlayFabClientAPI::FPurchaseItemDelegate::CreateUObject(this,&UPlayfabManager::OnPurchaseWithVirtualCurrencySuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::PurchaseWithWingTicket(int amount, FString itemName)
{
	PlayFab::ClientModels::FPurchaseItemRequest Req;
	Req.Price = amount;
	Req.ItemId = itemName;
	Req.VirtualCurrency=TEXT("WT");
	GetClientAPI->PurchaseItem(Req,PlayFab::UPlayFabClientAPI::FPurchaseItemDelegate::CreateUObject(this,&UPlayfabManager::OnPurchaseWithVirtualCurrencySuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::SubtractWeaponStone(int amount)
{
	PlayFab::ClientModels::FSubtractUserVirtualCurrencyRequest Req;
	Req.Amount = amount;
	Req.VirtualCurrency = TEXT("WS");
	
	GetClientAPI->SubtractUserVirtualCurrency(Req,PlayFab::UPlayFabClientAPI::FSubtractUserVirtualCurrencyDelegate::CreateUObject(this,&UPlayfabManager::OnAddWeaponStone),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::SubtractSkillStone(int amount)
{
	PlayFab::ClientModels::FSubtractUserVirtualCurrencyRequest Req;
	Req.Amount = amount;
	Req.VirtualCurrency = TEXT("SS");
	
	GetClientAPI->SubtractUserVirtualCurrency(Req,PlayFab::UPlayFabClientAPI::FSubtractUserVirtualCurrencyDelegate::CreateUObject(this,&UPlayfabManager::OnAddSkillStone),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::SubtractDgKey(int amount)
{
	PlayFab::ClientModels::FSubtractUserVirtualCurrencyRequest Req;
	Req.Amount = amount;
	Req.VirtualCurrency = TEXT("KK");
	
	GetClientAPI->SubtractUserVirtualCurrency(Req,PlayFab::UPlayFabClientAPI::FSubtractUserVirtualCurrencyDelegate::CreateUObject(this,&UPlayfabManager::OnAddDgKeys),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::AddPetTicket(int amount)
{
	PlayFab::ClientModels::FAddUserVirtualCurrencyRequest Req;
	Req.Amount=amount;
	Req.VirtualCurrency=TEXT("PT");
	GetClientAPI->AddUserVirtualCurrency(Req,PlayFab::UPlayFabClientAPI::FAddUserVirtualCurrencyDelegate::CreateUObject(this,&UPlayfabManager::OnAddPetTicket),PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::AddWingTicket(int amount)
{
	PlayFab::ClientModels::FAddUserVirtualCurrencyRequest Req;
	Req.Amount=amount;
	Req.VirtualCurrency=TEXT("WT");
	GetClientAPI->AddUserVirtualCurrency(Req,PlayFab::UPlayFabClientAPI::FAddUserVirtualCurrencyDelegate::CreateUObject(this,&UPlayfabManager::OnAddWingTicket),PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));	
}

void UPlayfabManager::AddDgKey(int amount)
{
	PlayFab::ClientModels::FAddUserVirtualCurrencyRequest Req;
	Req.Amount=amount;
	Req.VirtualCurrency=TEXT("KK");
	GetClientAPI->AddUserVirtualCurrency(Req,PlayFab::UPlayFabClientAPI::FAddUserVirtualCurrencyDelegate::CreateUObject(this,&UPlayfabManager::OnAddDgKeys),PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::AddWeaponStones(int amount)
{
	PlayFab::ClientModels::FAddUserVirtualCurrencyRequest Req;
	Req.Amount=amount;
	Req.VirtualCurrency=TEXT("WS");
	GetClientAPI->AddUserVirtualCurrency(Req,PlayFab::UPlayFabClientAPI::FAddUserVirtualCurrencyDelegate::CreateUObject(this,&UPlayfabManager::OnAddWeaponStone),PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::AddSkillStones(int amount)
{
	PlayFab::ClientModels::FAddUserVirtualCurrencyRequest Req;
	Req.Amount=amount;
	Req.VirtualCurrency=TEXT("SS");
	GetClientAPI->AddUserVirtualCurrency(Req,PlayFab::UPlayFabClientAPI::FAddUserVirtualCurrencyDelegate::CreateUObject(this,&UPlayfabManager::OnAddSkillStone),PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnAddGemStone(const PlayFab::ClientModels::FModifyUserVirtualCurrencyResult& rslt)
{
	UDiabloGameInstance::Get->m_GoldManager->SetGemStones(rslt.Balance);
}

void UPlayfabManager::OnAddWeaponStone(const PlayFab::ClientModels::FModifyUserVirtualCurrencyResult& rslt)
{
	UDiabloGameInstance::Get->m_GoldManager->SetWeaponStones(rslt.Balance);
}

void UPlayfabManager::OnAddSkillStone(const PlayFab::ClientModels::FModifyUserVirtualCurrencyResult& rslt)
{
	UDiabloGameInstance::Get->m_GoldManager->SetSkillStones(rslt.Balance);
}

void UPlayfabManager::OnAddPetTicket(const PlayFab::ClientModels::FModifyUserVirtualCurrencyResult& rslt)
{
	UDiabloGameInstance::Get->m_GoldManager->SetPetTickets(rslt.Balance);
}

void UPlayfabManager::OnAddWingTicket(const PlayFab::ClientModels::FModifyUserVirtualCurrencyResult& rslt)
{
	UDiabloGameInstance::Get->m_GoldManager->SetWingTickets(rslt.Balance);
}

void UPlayfabManager::OnAddDgKeys(const PlayFab::ClientModels::FModifyUserVirtualCurrencyResult& rslt)
{
	UDiabloGameInstance::Get->m_GoldManager->SetDgKeys(rslt.Balance);
}

void UPlayfabManager::OnBossBattleStart()
{
	m_fDeltaCountRanking-=20.f;
	m_fDeltaCountTitleData=0.f;
}

void UPlayfabManager::SetMainDataToManagers(const FString& maindataFromServer)
{
	UPlayFabJsonObject* JsonObj =UPlayFabJsonObject::ConstructJsonObject(this);

	if(!JsonObj->DecodeJson(maindataFromServer))
	{
		return;
	}

	FString LogoutLoaded = JsonObj->GetStringField(TEXT("LogoutTime"));

	if(FDateTime::Parse(LogoutLoaded,m_LastLogoutTime))
	{
		FTimespan KoreanTime(9,0,0);
		m_LastLogoutTime+=KoreanTime;
	}
	
	UDiabloGameInstance::Get->m_ShopManager->SetShopDataFromServer(JsonObj->GetObjectField(TEXT("IAP")));
	UDiabloGameInstance::Get->m_GachaManager->SetGachaLevel(JsonObj->GetObjectField(TEXT("Gacha")));
	UDiabloGameInstance::Get->m_PlayerUpgradeManager->SetUpgradeDataFromServer(JsonObj->GetObjectField(TEXT("Upgrade")),JsonObj->GetObjectField(TEXT("Skill")));
	UDiabloGameInstance::Get->m_EquipManager->
	SetEquipDataFromServer(
		JsonObj->GetArrayField(TEXT("Skin")),
		JsonObj->GetArrayField(TEXT("Weapon")),
		JsonObj->GetArrayField(TEXT("Wing")),
		JsonObj->GetArrayField(TEXT("Pet")));
}

void UPlayfabManager::OnPVPUploadSuccess(const FExeCScriptRslt& rslt)
{
	FString CachedJsonString = rslt.FunctionResult.toJSONString();

	UDiabloGameInstance::Get->m_PVPManager->SetPVPDataBeforeUpload(CachedJsonString);

}

void UPlayfabManager::OnPVPGetSuccess(const FExeCScriptRslt& rslt)
{
	FString CachedJsonString = rslt.FunctionResult.toJSONString();

	UDiabloGameInstance::Get->m_PVPManager->SetPVPData(CachedJsonString);
}


void UPlayfabManager::UploadDailyData(const FString dailyJsonStr)
{
	FUpdateReq Req;
	
	Req.Data.Add(Daily,dailyJsonStr);

	GetClientAPI->UpdateUserData(Req,nullptr,
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::UploadQuestData(const FString& data)
{
	PlayFab::ClientModels::FUpdateUserDataRequest Req;
	Req.Permission=PlayFab::ClientModels::UserDataPermission::UserDataPermissionPublic;
	
	Req.Data.Add(Quest,data);

	GetClientAPI->UpdateUserData(Req,nullptr,
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::UploadGold(BigInt gold)
{
	FUpdateReq Req;
	Req.Permission=PlayFab::ClientModels::UserDataPermission::UserDataPermissionPublic;
	
	Req.Data.Add(Gold,gold.ToString());

	GetClientAPI->UpdateUserData(Req,nullptr,
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}
//OnPVPComplete Win:2,Lose:3


void UPlayfabManager::OnPvPComplete()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetNumberField(TEXT("Win"), UDiabloGameInstance::Get->m_PVPManager->GetWin());

	JsonObject->SetNumberField(TEXT("Lose"), UDiabloGameInstance::Get->m_PVPManager->GetLose());

	PlayFab::ClientModels::FExecuteCloudScriptRequest Req;
	
	Req.FunctionParameter = PlayFab::FJsonKeeper(JsonObject);
	
	Req.FunctionName = TEXT("OnPVPComplete");
	
	GetClientAPI->ExecuteCloudScript(Req,FExeCScriptDele::CreateUObject(this, &UPlayfabManager::OnPVPUploadSuccess),FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::RequestGetPVPData()
{
	PlayFab::ClientModels::FExecuteCloudScriptRequest Req;
	
	Req.FunctionName = TEXT("GetPVPData");
	
	GetClientAPI->ExecuteCloudScript(Req,FExeCScriptDele::CreateUObject(this, &UPlayfabManager::OnPVPGetSuccess),FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));	
}//OnPVPUploadSuccess

#undef LOCTEXT_NAMESPACE
