#include "PlayfabManager.h"
#include "DiabloGameInstance.h"
#include "DungeonManager.h"
#include "EquipManager.h"
#include "InboxManager.h"
#include "JsonObjectConverter.h"
#include "JsonSerializer.h"
#include "MobileUtilsBlueprintLibrary.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "PlayFabClientDataModels.h"
#include "PlayerUpgradeManager.h"
#include "PlayFabAdminDataModels.h"
#include "PlayFabClientModels.h"
#include "PlayFabJsonObject.h"
#include "PlayFabJsonValue.h"
#include "PlayFabServerDataModels.h"
#include "PlayFabUtilities.h"
#include "Objects/MyInAppPurchase.h"
#include "Misc/Base64.h"

#define LOCTEXT_NAMESPACE "PlayfabManager"

//dungeon 1111200
const FString UPlayfabManager::IAP = TEXT("IAP");
const FString UPlayfabManager::GachaLevel= TEXT("GachaLevel");
const FString UPlayfabManager::MainDungeon= TEXT("MainDungeon");
const FString UPlayfabManager::Upgrade= TEXT("Upgrade");
const FString UPlayfabManager::Skill= TEXT("Skill");
const FString UPlayfabManager::Quest= TEXT("Quest");
const FString UPlayfabManager::Gold= TEXT("Gold");
const FString UPlayfabManager::Weapon= TEXT("Weapon");
const FString UPlayfabManager::Skin= TEXT("Skin");
const FString UPlayfabManager::Pet= TEXT("Pet");
const FString UPlayfabManager::Daily= TEXT("Daily");
const FString UPlayfabManager::LogoutTime= TEXT("LogoutTime");
//

UPlayfabManager::UPlayfabManager()
{
	SetRanking(-123);
	
	m_CurrentVersionName=TEXT("0410NEEDFIX");

	m_bIsCustomID=false;
	//
	m_fDeltaInboxUpdateCooldown =160.f;
}

UPlayfabManager::~UPlayfabManager()
{
}

int UPlayfabManager::GetSafeRanking()
{
	int CachecRank = m_nSafeRanking ^ 7777;
	
	if(m_nRanking!=CachecRank)
	{
		PRINTF("Cheated!!!!!");
		RequestCheatAlert();
		return -1;
	}

	return CachecRank;
}

int UPlayfabManager::GetRanking()
{
	return m_nRanking;
}

void UPlayfabManager::SetRanking(int rank)
{
	m_nRanking = rank;
	m_nSafeRanking = m_nRanking ^ 7777;
}

void UPlayfabManager::UploadUserTitleData01()
{
	PlayFab::ClientModels::FUpdateUserDataRequest Req;
	Req.Permission=PlayFab::ClientModels::UserDataPermission::UserDataPermissionPublic;
	// //
	Req.Data.Add(IAP,UDiabloGameInstance::Get->m_ShopManager->GetIAPDataStr());
	Req.Data.Add(GachaLevel,UDiabloGameInstance::Get->m_GachaManager->GetGachaDataStr());
	Req.Data.Add(MainDungeon,UDiabloGameInstance::Get->m_DungeonManager->GetDgDataStr());
	Req.Data.Add(Upgrade,UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetUpgradeDataStr());
	Req.Data.Add(Skill,UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetSkillDataStr());
	Req.Data.Add(LogoutTime,FDateTime::UtcNow().ToString());
	

	GetClientAPI->UpdateUserData(Req,FUpdateDele::CreateUObject(this,&UPlayfabManager::UploadTitleData02),
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
	
	m_fDeltaCountTitleData = 0.f;
}

void UPlayfabManager::UploadTitleData02(const FUpdateRslt&)
{
	PlayFab::ClientModels::FUpdateUserDataRequest Req;
	Req.Permission=PlayFab::ClientModels::UserDataPermission::UserDataPermissionPublic;
	//
	Req.Data.Add(Quest,UDiabloGameInstance::Get->m_QuestManager->GetQuestDataStr());
	Req.Data.Add(Gold,UDiabloGameInstance::Get->m_GoldManager->GetGoldDataStr());
	Req.Data.Add(Weapon,UDiabloGameInstance::Get->m_EquipManager->GetWeaponDataStr());
	Req.Data.Add(Skin,UDiabloGameInstance::Get->m_EquipManager->GetSkinDataStr());
	Req.Data.Add(Pet,UDiabloGameInstance::Get->m_EquipManager->GetPetDataStr());
	//
	GetClientAPI->UpdateUserData(Req,nullptr,
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::TickTryUpdateUserData(float deltaTime)
{
	m_fDeltaCountTitleData += deltaTime;
	
	m_fDeltaCountRanking += deltaTime;
	
	m_fDeltaCountMinutePlaytime += deltaTime;

	m_fDeltaInboxUpdateCooldown += deltaTime;

	if (m_fDeltaCountMinutePlaytime > 60.f)
	{
		UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::PlayTime);

		m_fDeltaCountMinutePlaytime=0;
	}

	 if (m_fDeltaCountTitleData > 50.f)
	 {
	 	PRINTF("TryUpdateUserData");
	// 	
	 	UploadUserTitleData01();
	 	UDiabloGameInstance::Get->m_QuestManager->UploadQuestData();
	 }

	if (m_fDeltaCountRanking > 220.f)
	{
		PRINTF("TryUpdateRank");
		RequestRetrieveTotalRanking();
		RequestRetrievePlayerAroundRanking();
		//나의 랭킹 업데이트가 필요
		m_fDeltaCountRanking = 0.f;
	}
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
		RequestGetUserData01();
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
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Server Closed", "서버 사용 가능"));
		
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

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	if (!Subsystem)
	{
		return;
	}


#if PLATFORM_WINDOWS
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Try Login With Desktop", "Try Login With Desktop"));
	GetClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();

	PlayFab::ClientModels::FLoginWithCustomIDRequest request;
	request.CreateAccount = true;
	request.CustomId = TEXT("JungPC TestID3");
	request.TitleId = GetDefault<UPlayFabRuntimeSettings>()->TitleId;
	m_bIsCustomID = true;
	bool Result = GetClientAPI->LoginWithCustomID(request,
	                                              PlayFab::UPlayFabClientAPI::FLoginWithGoogleAccountDelegate::CreateUObject(
		                                              this, &UPlayfabManager::OnSuccessPlayfabLogin),
	                                              PlayFab::FPlayFabErrorDelegate::CreateUObject(
		                                              this, &UPlayfabManager::OnErrorPlayfabReq)
	);


#endif

#if PLATFORM_ANDROID
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Try Login With Android","Try Login With Android"));
	IOnlineExternalUIPtr ExternalUi = Subsystem->GetExternalUIInterface();

	if (!ExternalUi)
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("FAIL-GoogleLoginFail-1","FAIL-GoogleLoginFail-1"));
		return;
	}

	ExternalUi->ShowLoginUI(0, false, false,
	                        FOnLoginUIClosedDelegate::CreateUObject(this, &UPlayfabManager::HandleExternalUIClose));
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

		GetClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();

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

void UPlayfabManager::RequestGetUserData01()
{
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Request Get User Data01", "유저 데이터 가져오는 중 01"));
	FGetUsrDataReq req;

	//Request Data
	req.PlayFabId = m_PlayfabID;
	req.Keys.Add(IAP);
	req.Keys.Add(GachaLevel);
	req.Keys.Add(MainDungeon);
	req.Keys.Add(Upgrade);
	req.Keys.Add(Skill);
	req.Keys.Add(LogoutTime);
	

	GetClientAPI->GetUserData(req,FGetUsrDataDele::CreateUObject(this, &UPlayfabManager::OnSuccessGetUserData01),
		FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::RequestGetUserData02()
{
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Request Get User Data02", "유저 데이터 가져오는 중 02"));
	FGetUsrDataReq req;

	//Request Data
	req.PlayFabId = m_PlayfabID;
	req.Keys.Add(Quest);
	req.Keys.Add(Gold);
	req.Keys.Add(Weapon);
	req.Keys.Add(Skin);
	req.Keys.Add(Pet);
	req.Keys.Add(Daily);
	req.Keys.Add(LogoutTime);

	GetClientAPI->GetUserData(req,FGetUsrDataDele::CreateUObject(this, &UPlayfabManager::OnSuccessGetUserData02),
        FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}
void UPlayfabManager::RequestClaimInbox(int index)//
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetNumberField(TEXT("Index"), index);

	PlayFab::ClientModels::FExecuteCloudScriptRequest Req;
	
	Req.FunctionParameter = PlayFab::FJsonKeeper(JsonObject);
	
	Req.FunctionName = TEXT("ClaimInbox");
	
	Req.GeneratePlayStreamEvent = true;
	
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
	
	Req.GeneratePlayStreamEvent = true;
	
	GetClientAPI->ExecuteCloudScript(Req,FExeCScriptDele::CreateUObject(this, &UPlayfabManager::OnInboxRefreshSuccess),FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
	//
	return true;
}


void UPlayfabManager::OnErrorPlayfabReq(const FFailRslt& ErrorResult)
{
	FString CodeString = UPlayFabUtilities::getErrorText(ErrorResult.ErrorCode);
	
	UDiabloGameInstance::Get->RequestPopupText(CodeString);
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

void UPlayfabManager::OnSuccessGetUserData01(const FGetUsrDataRslt& result)
{
	if (!result.Data.Num())
	{
		PRINTF("DataNull");
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("FAIL-Get User Data Null01", "실패-유저 데이터 획득 없음01"));
	}
	//
	UDiabloGameInstance::Get->m_ShopManager->SetShopDataFromServer(result.Data[IAP].Value);
	UDiabloGameInstance::Get->m_GachaManager->SetGachaLevel(result.Data[GachaLevel].Value);
	UDiabloGameInstance::Get->m_DungeonManager->SetDungeonData(result.Data[MainDungeon].Value);
	UDiabloGameInstance::Get->m_PlayerUpgradeManager->SetUpgradeDataFromServer(result.Data[Upgrade].Value,result.Data[Skill].Value);
	
	RequestGetUserData02();
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
	if (!result.Data.Num())
	{
		PRINTF("DataNull");
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("FAIL-Get User Data Null02", "실패-유저 데이터 획득 없음02"));
	}
	else
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("SUCCESS-Get User Data02", "유저 데이터 획득 성공"));	
	}

	if(FDateTime::Parse(result.Data[LogoutTime].Value,m_LastLogoutTime))
	{
		FTimespan KoreanTime(9,0,0);
		m_LastLogoutTime+=KoreanTime;
	}
	
	//m_LastLogoutTime?
	UDiabloGameInstance::Get->m_DailyManager->SetPrizeManager(result.Data[Daily].Value,m_CurrentTime,m_bIsNewCreatePlayer);
	UDiabloGameInstance::Get->m_QuestManager->SetQuestDataFromServer(result.Data[Quest].Value);
	//Equip Should Init Faster than GoldManager
	UDiabloGameInstance::Get->m_EquipManager->SetEquipDataFromServer(result.Data[Weapon].Value,result.Data[Skin].Value,result.Data[Pet].Value);
	UDiabloGameInstance::Get->m_GoldManager->SetCurrentGold(result.Data[Gold].Value,m_bIsNewCreatePlayer,m_CurrentTime,m_LastLoginTime,m_LastLogoutTime);
	//
	m_bIsLoginCompleted = true;
	
	m_bIsNicknameSet = true;

	RequestCatalogItems();
	RequestRetrievePlayerAroundRanking();
	RequestRetrieveTotalRanking();
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

void UPlayfabManager::OnStageComplete()
{
	int MaxStage = UDiabloGameInstance::Get->m_DungeonManager->GetMaxStage();

	PRINTF("UploadMaxStage:%d",MaxStage);
	
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetNumberField(TEXT("stageLevel"), MaxStage);

	PlayFab::ClientModels::FExecuteCloudScriptRequest Req;
	
	Req.FunctionParameter = PlayFab::FJsonKeeper(JsonObject);
	
	Req.FunctionName = TEXT("OnCompleteLevel");
	
	Req.GeneratePlayStreamEvent = true;
	
	GetClientAPI->ExecuteCloudScript(Req,FExeCScriptDele::CreateUObject(this, &UPlayfabManager::OnStageCompleteScriptSuccess),FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));

	UploadUserTitleData01();
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

	UDiabloGameInstance::Get->m_ShopManager->OnPurchasedGainItem(ItemID);
}

void UPlayfabManager::RequestGetInventory()
{
	PlayFab::ClientModels::FGetUserInventoryRequest Req;
	GetClientAPI->GetUserInventory(Req,
		PlayFab::UPlayFabClientAPI::FGetUserInventoryDelegate::CreateUObject(this,&UPlayfabManager::OnSuccessGetInven),
		FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnSuccessGetInven( const PlayFab::ClientModels::FGetUserInventoryResult& rslt)
{
	m_nLocalGemStone = rslt.VirtualCurrency[TEXT("GG")];
	
	m_OnGemstoneChanged.Broadcast(m_nLocalGemStone);
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
	m_TotalRanking = rslt.Leaderboard;

	m_OnTotalRankReceived.Broadcast(m_TotalRanking);
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

void UPlayfabManager::RequestGetServerTime()
{
	PlayFab::ClientModels::FGetTimeRequest Req;
	GetClientAPI->GetTime(Req,PlayFab::UPlayFabClientAPI::FGetTimeDelegate::CreateUObject(this,&UPlayfabManager::OnSuccessTimeGet));
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
	
	AryInbox.Reserve(20);
	
	if(!FJsonObjectConverter::JsonArrayStringToUStruct(InboxListStr, &AryInbox, 0, 0))
	{
		UDiabloGameInstance::Get->m_InboxManager->SetInboxManager(AryInbox);
		return;
	}

	FTimespan KoreanTime(9,0,0);

	for(auto& InboxRef : AryInbox)
	{
		InboxRef.m_ExpireTimeUTC = DecodePlayfabTimeToUe4Time(InboxRef.ExpireTime);

		InboxRef.m_ExpireTimeUTC+=KoreanTime;

		FString EpxireItmeUTCStr =InboxRef.m_ExpireTimeUTC.ToString(); 
	
		FTimespan OfflineTimeSpawn = InboxRef.m_ExpireTimeUTC - m_CurrentTime;
		
		int Minutes = OfflineTimeSpawn.GetTotalMinutes();
		
		if(Minutes<1)
		{
			PRINTF("Minutes Expired");
			InboxRef.m_bIsExpired = true;
		}
	}
	
	UDiabloGameInstance::Get->m_InboxManager->SetInboxManager(AryInbox);
}


void UPlayfabManager::OnSuccessGetPlayerAroundRanking(const PlayFab::ClientModels::FGetLeaderboardAroundPlayerResult& rslt)
{
	m_PlayerRanking = rslt.Leaderboard;
	
	SetRanking(m_PlayerRanking[0].Position+1);

	m_OnPlayerRankReceived.Broadcast(m_PlayerRanking);
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
	//Req.CharacterId = m_PlayfabID;
	//FPurchaseItemDelegate, const ClientModels::FPurchaseItemResult&
	GetClientAPI->PurchaseItem(Req,PlayFab::UPlayFabClientAPI::FPurchaseItemDelegate::CreateUObject(this,&UPlayfabManager::OnPurchaseWithGemStoneSuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnPurchaseWithGemStoneSuccess(const PlayFab::ClientModels::FPurchaseItemResult& rslt)
{
	FString PurchasedItemID =  rslt.Items[0].ItemId;

	UDiabloGameInstance::Get->m_ShopManager->OnPurchasedGainItem(PurchasedItemID);

}

void UPlayfabManager::AddGemStone(int amount)
{
	PlayFab::ClientModels::FAddUserVirtualCurrencyRequest Req;
	Req.Amount=amount;
	Req.VirtualCurrency=TEXT("GG");
	GetClientAPI->AddUserVirtualCurrency(Req,PlayFab::UPlayFabClientAPI::FAddUserVirtualCurrencyDelegate::CreateUObject(this,&UPlayfabManager::OnAddGemStone),PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnAddGemStone(const PlayFab::ClientModels::FModifyUserVirtualCurrencyResult& rslt)
{
	m_OnGemstoneChanged.Broadcast(rslt.Balance);
}

void UPlayfabManager::OnBossBattleStart()
{
	m_fDeltaCountRanking-=20.f;
	m_fDeltaCountTitleData=0.f;
}

void UPlayfabManager::RequestItemTest()
{
	PlayFab::ClientModels::FItemInstance ItemTest;
	
}

void UPlayfabManager::UploadDailyData(const FString dailyJsonStr)
{
	FUpdateReq Req;
	Req.Permission=PlayFab::ClientModels::UserDataPermission::UserDataPermissionPublic;
	
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


#undef LOCTEXT_NAMESPACE
