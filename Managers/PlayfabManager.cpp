#include "PlayfabManager.h"
#include "DiabloGameInstance.h"
#include "DungeonManager.h"
#include "EquipManager.h"
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
const FString UPlayfabManager::Gold = "Gold";
const FString UPlayfabManager::Dg = "Dg";
const FString UPlayfabManager::StatSkill = "Stat&Skill";

const FString UPlayfabManager::SkinClass = "Class";
const FString UPlayfabManager::Weapon = "Weapon";
const FString UPlayfabManager::Wing = "Wing";
const FString UPlayfabManager::Pet = "Pet";
const FString UPlayfabManager::Accessory = "Accessory";
const FString UPlayfabManager::IAP = "IAP";

const FString UPlayfabManager::Quest = "Quest";

UPlayfabManager::UPlayfabManager()
{
	SetRanking(-123);
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

FString UPlayfabManager::GetIAPDataStr()
{
	FString IAPResult = UDiabloGameInstance::Get->m_ShopManager->GetIAPDataStr();
	
	FString GachaResult = UDiabloGameInstance::Get->m_GachaManager->GetGachaLevelStr();

	IAPResult.Append(GachaResult);

	FDateTime CurrentTime = FDateTime::Now().UtcNow();

	FString TimeStr = CurrentTime.ToString();
	
	TimeStr.AppendChar(TEXT('/'));
	
	IAPResult.Append(TimeStr);

	int NextDDay = UDiabloGameInstance::Get->m_ShopManager->m_nDDay;

	FString DDayStr = FString::FromInt(NextDDay);
	
	DDayStr.AppendChar(TEXT('/'));

	IAPResult.Append(DDayStr);
	//
	FDateTime RewardClaimTime = UDiabloGameInstance::Get->m_ShopManager->m_DailyRewardClaimTime;

	FString ClaimTimeStr = RewardClaimTime.ToString();
	
	ClaimTimeStr.AppendChar(TEXT('/'));
	
	IAPResult.Append(ClaimTimeStr);

	
	return IAPResult;
}

void UPlayfabManager::UploadUserTitleData()
{
	PlayFab::ClientModels::FUpdateUserDataRequest Req;
	//
	Req.Data.Add(Dg,UDiabloGameInstance::Get->m_DungeonManager->GetDgDataStr());
	Req.Data.Add(StatSkill,UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetUpgradeDataStr());
	Req.Data.Add(Gold,UDiabloGameInstance::Get->m_GoldManager->GetGoldDataStr());

	Req.Data.Add(Weapon,UDiabloGameInstance::Get->m_EquipManager->GetWeaponDataStr());
	Req.Data.Add(SkinClass,UDiabloGameInstance::Get->m_EquipManager->GetSkinDataStr());
	Req.Data.Add(Pet,UDiabloGameInstance::Get->m_EquipManager->GetPetDataStr());
	Req.Data.Add(Accessory,UDiabloGameInstance::Get->m_EquipManager->GetAccessoryDataStr());
	Req.Data.Add(Wing,UDiabloGameInstance::Get->m_EquipManager->GetWingDataStr());
	
	Req.Data.Add(IAP,GetIAPDataStr());
	

	GetClientAPI->UpdateUserData(Req,nullptr,
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
	//
	m_fDeltaCountTitleData = 0.f;
}

void UPlayfabManager::TickTryUpdateUserData(float deltaTime)
{
	m_fDeltaCountTitleData += deltaTime;
	
	m_fDeltaCountRanking += deltaTime;
	
	m_fDeltaCountMinutePlaytime += deltaTime;

	if (m_fDeltaCountMinutePlaytime > 60.f)
	{
		UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::PlayTime);

		m_fDeltaCountMinutePlaytime=0;
	}

	 if (m_fDeltaCountTitleData > 50.f)
	 {
	 	PRINTF("TryUpdateUserData");
	// 	
	 	UploadUserTitleData();
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

	FTimerHandle hh;
	UDiabloGameInstance::Get->GetTimerManager().SetTimer(hh, this, &UPlayfabManager::RequestGetUserData, 5.5f,
	                                                     false);
}

void UPlayfabManager::OnCloudScriptSuccess(const FExeCScriptRslt& rslt)
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

	FString ServerVersionName = JsonObject->GetStringField(TEXT("ServerVersionName"));
	
	if(m_CurrentVersionName == ServerVersionName)
	{
		PRINTF("Version Same");
	}
	else
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Version Changed", "업데이트 해주세요!"));
		UKismetSystemLibrary::LaunchURL(TEXT("https://play.google.com/store/apps/details?id=com.hereticbyte.dungeonslasher"));
	}
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


void UPlayfabManager::HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex,
                                            const FOnlineError& error)
{

	if (error.bSucceeded)
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("SUCCESS-GoogleLogin", "구글 로그인 성공"));
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
	UDiabloGameInstance::Get->RequestPopupText(
		LOCTEXT("SUCCESS-Playfab Login Success", "SUCCESS-Playfab Login Success"));
	PRINTF("ID:%s", *Result.PlayFabId);

	m_PlayfabID = Result.PlayFabId;

	m_LastLoginTime = Result.LastLoginTime;

	m_bIsNewCreatePlayer = Result.NewlyCreated;
	
	//
	RequestGetAccountInfo();
}

void UPlayfabManager::RequestGetUserData()
{
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Request Get User Data", "Request Get User Data"));
	FGetUsrDataReq req;

	//Request Data
	req.PlayFabId = m_PlayfabID;
	req.Keys.Add(Gold);
	req.Keys.Add(Dg);
	req.Keys.Add(StatSkill);
	req.Keys.Add(Quest);
	req.Keys.Add(SkinClass);
	req.Keys.Add(Weapon);
	req.Keys.Add(Wing);
	req.Keys.Add(Pet);
	req.Keys.Add(Accessory);
	req.Keys.Add(IAP);


	GetClientAPI->GetUserData(req,
	                          FGetUsrDataDele::CreateUObject(this, &UPlayfabManager::OnSuccessGetUserData),
	                          FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}




void UPlayfabManager::OnErrorPlayfabReq(const FFailRslt& ErrorResult)
{
	FString CodeString = UPlayFabUtilities::getErrorText(ErrorResult.ErrorCode);
	
	UDiabloGameInstance::Get->RequestPopupText(CodeString);
	//break;
	m_OnPlayfabError.Broadcast(CodeString);
}

void UPlayfabManager::OnSuccessGetUserData(const FGetUsrDataRslt& result)
{
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("SUCCESS-Get User Data", "SUCCESS-Get User Data"));

	if (!result.Data.Num())
	{
		PRINTF("DataNull");
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("FAIL-Get User Data Null", "FAIL-Get User Data Null"));
		//Something Fucked
	}
	//
	m_LoadedGold = result.Data[Gold].Value;
	m_LoadedDg = result.Data[Dg].Value;
	m_LoadedStatSkill = result.Data[StatSkill].Value;
	m_LoadedQuest = result.Data[Quest].Value;
	m_LoadedClass = result.Data[SkinClass].Value;
	m_LoadedWeapon = result.Data[Weapon].Value;
	m_LoadedWing = result.Data[Wing].Value;
	m_LoadedPet = result.Data[Pet].Value;
	m_LoadedAccessory = result.Data[Accessory].Value;

	m_LoadedIAP = result.Data[IAP].Value;
	//
	m_LoadedIAP.ParseIntoArray(m_AryIAPData,TEXT("/"));
	//
	UDiabloGameInstance::Get->m_GoldManager->SetCurrentGold(m_LoadedGold);
	UDiabloGameInstance::Get->m_DungeonManager->SetDungeonLevel(*m_LoadedDg);
	UDiabloGameInstance::Get->m_PlayerUpgradeManager->SetUpgradeDataFromServer(m_LoadedStatSkill);
	UDiabloGameInstance::Get->m_EquipManager->SetEquipDataFromServer(m_LoadedClass, m_LoadedWeapon, m_LoadedWing,
	                                                                 m_LoadedPet, m_LoadedAccessory);
	
	UDiabloGameInstance::Get->m_QuestManager->SetQuestDataFromServer(m_LoadedQuest);

	UDiabloGameInstance::Get->m_ShopManager->SetShopDataFromServer();
	
	m_bIsLoginCompleted = true;
	m_bIsNicknameSet = true;

	RequestVersionCheck();

	PlayFab::ClientModels::FGetCatalogItemsRequest Req;
	GetClientAPI->GetCatalogItems(Req, PlayFab::UPlayFabClientAPI::FGetCatalogItemsDelegate::
	                              CreateLambda([&](const PlayFab::ClientModels::FGetCatalogItemsResult cIRslt)
	                              {
		                              for(const PlayFab::ClientModels::FCatalogItem& CatalogItem : cIRslt.Catalog)
		                              {
		                              		m_MapCatalogItems.Add(CatalogItem.ItemId,CatalogItem);      
		                              }
	                              }), FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));

	RequestRetrievePlayerAroundRanking();
	RequestRetrieveTotalRanking();
	RequestGetServerTime();
}

void UPlayfabManager::RequestGetAccountInfo()
{
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("RequestGetAccountInfo", "RequestGetAccountInfo"));
	FGetAccntInfoReq Req;

	GetClientAPI->GetAccountInfo(Req, FGetAccntInfoDele::CreateUObject(this, &UPlayfabManager::OnSuccessGetAccountInfo),
	                             FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnSuccessGetAccountInfo(const FGetAccntInfoRslt& rslt)
{
	if (rslt.AccountInfo->TitleInfo->isBanned)
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Banned Player", "Banned Player"));
		return;
	}


	if (rslt.AccountInfo->TitleInfo->DisplayName.IsEmpty())
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Please Set Nickname", "Please Set Nickname"));
		m_bShowNicknameSet = true;
		return;
	}

	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Welcome", "Welcome"));

	m_LoadedNickname = rslt.AccountInfo->TitleInfo->DisplayName;
	m_bIsNicknameSet = true;
	RequestGetUserData();
}

void UPlayfabManager::BuyIAP(FString itemId, bool bIsConsumable)
{
	if(!m_MapCatalogItems.Find(itemId))
	{
		UDiabloGameInstance::Get->RequestPopupText("NoItem,Update Need");	
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
		UDiabloGameInstance::Get->RequestPopupText("NoItem,Update Need");	
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
	UDiabloGameInstance::Get->RequestPopupText("IAP Purchase Fail!");
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
	
	GetClientAPI->ExecuteCloudScript(Req,FExeCScriptDele::CreateUObject(this, &UPlayfabManager::OnCloudScriptSuccess),FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));

	UploadUserTitleData();
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

void UPlayfabManager::OnIAPGoogleValidateSuccess(const PlayFab::ClientModels::FValidateGooglePlayPurchaseResult& purchaseResult)
{
	FString ItemID = purchaseResult.Fulfillments[0].FulfilledItems[0].ItemId;

	UDiabloGameInstance::Get->RequestPopupText(FString::Printf(TEXT("IAP Purchase Success!:%s"),*ItemID));

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
	PRINTF("GetInven Success");
	
	int Currency = rslt.VirtualCurrency[TEXT("GG")];
	
	m_OnGemstoneChanged.Broadcast(Currency);
}

void UPlayfabManager::OnSuccessTimeGet(const PlayFab::ClientModels::FGetTimeResult& rslt)
{
	m_CurrentTime = rslt.Time;

	FTimespan OfflineTimeSpawn;
	
	TArray<FString> AryItemBought;

	m_LoadedIAP.ParseIntoArray(AryItemBought,TEXT("/"));

	bool LogoutParseSuccess = FDateTime::Parse(AryItemBought[8],m_LastLogoutTime);

	bool LogoutTimeIsValid = (m_LastLogoutTime - m_LastLoginTime).GetTotalMinutes() > 0;//check time fixed device

	FString LastLogout = m_LastLogoutTime.ToString();
	FString LastLogint = m_LastLoginTime.ToString();
	FString CurrentTime = m_CurrentTime.ToString();
	
	if(LogoutParseSuccess&&LogoutTimeIsValid)
	{
		OfflineTimeSpawn = m_CurrentTime - m_LastLogoutTime ;		
	}
	else
	{
		OfflineTimeSpawn = m_CurrentTime - m_LastLoginTime;
		//Fail, WE Use Login Time
	}
		
	int Minuts =  OfflineTimeSpawn.GetTotalMinutes();

	PRINTF("LastLogin%s,CurrentTime:%s,TimeSpan:%s,Minutes:%d",*m_LastLoginTime.ToString(),*m_CurrentTime.ToString(),*OfflineTimeSpawn.ToString(),Minuts);
	
	UDiabloGameInstance::Get->m_GoldManager->SetOfflineMinutes(Minuts);
	UDiabloGameInstance::Get->m_ShopManager->SetOfflineHours(m_CurrentTime);
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

void UPlayfabManager::OnSuccessGetPlayerAroundRanking(const PlayFab::ClientModels::FGetLeaderboardAroundPlayerResult& rslt)
{
	m_PlayerRanking = rslt.Leaderboard;
	
	SetRanking(m_PlayerRanking[0].Position+1);

	m_OnPlayerRankReceived.Broadcast(m_PlayerRanking);
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

void UPlayfabManager::UploadQuestData(const FString& data)
{
	PlayFab::ClientModels::FUpdateUserDataRequest Req;
	
	Req.Data.Add(Quest,data);

	GetClientAPI->UpdateUserData(Req,nullptr,
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}



void UPlayfabManager::UploadUpgradeData(const FString& data)
{
	PlayFab::ClientModels::FUpdateUserDataRequest Req;
	
	Req.Data.Add(StatSkill,data);

	GetClientAPI->UpdateUserData(Req,nullptr,
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::UploadEquipData(const FString& weaponData, const FString& skinData, const FString& petData,
	const FString& accessoryData, const FString& wingData)
{
	PlayFab::ClientModels::FUpdateUserDataRequest Req;
	
	Req.Data.Add(Weapon,weaponData);
	Req.Data.Add(SkinClass,skinData);
	Req.Data.Add(Pet,petData);
	Req.Data.Add(Accessory,accessoryData);
	Req.Data.Add(Wing,wingData);

	GetClientAPI->UpdateUserData(Req,nullptr,
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::UploadIAPData()
{
	PlayFab::ClientModels::FUpdateUserDataRequest Req;
	
	Req.Data.Add(IAP,GetIAPDataStr());

	GetClientAPI->UpdateUserData(Req,nullptr,
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::UploadDungeonData(int currentDungeon, int maxDungeon)
{
	FString DgStr = FString::Printf(TEXT("%d:%d"),currentDungeon,maxDungeon);
	
	PlayFab::ClientModels::FUpdateUserDataRequest Req;
	
	Req.Data.Add(Dg,DgStr);

	GetClientAPI->UpdateUserData(Req,nullptr,
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnBossBattleStart()
{
	m_fDeltaCountRanking-=20.f;
	m_fDeltaCountTitleData=0.f;
}

void UPlayfabManager::UploadGold(BigInt gold)
{
	PlayFab::ClientModels::FUpdateUserDataRequest Req;
	
	Req.Data.Add(Gold,gold.ToString());

	GetClientAPI->UpdateUserData(Req,nullptr,
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}


#undef LOCTEXT_NAMESPACE
