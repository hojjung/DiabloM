#include "PlayfabManager.h"
#include "DiabloGameInstance.h"
#include "DungeonManager.h"
#include "EquipManager.h"
#include "MobileUtilsBlueprintLibrary.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "PlayFabClientDataModels.h"
#include "PlayerUpgradeManager.h"
#include "PlayFabAdminDataModels.h"
#include "PlayFabServerDataModels.h"
#include "PlayFabUtilities.h"
#include "Objects/MyInAppPurchase.h"
using namespace PlayFab;

#define LOCTEXT_NAMESPACE "PlayfabManager"

//dungeon 1111200
const FString UPlayfabManager::Gold = "Gold";
const FString UPlayfabManager::Dg = "Dg";
const FString UPlayfabManager::Stat = "Stat";
const FString UPlayfabManager::Skill = "Skill";

const FString UPlayfabManager::Class = "Class";
const FString UPlayfabManager::Weapon = "Weapon";
const FString UPlayfabManager::Wing = "Wing";
const FString UPlayfabManager::Pet = "Pet";
const FString UPlayfabManager::Accessory = "Accessory";


UPlayfabManager::UPlayfabManager()
{
	//m_LoadedDgID ;//= "Stage1-1";
	//m_LoadedPlayerClassID;// = "Warrior01";
}

UPlayfabManager::~UPlayfabManager()
{
}

void UPlayfabManager::ShowBannerAd(bool able)
{
	if (able && !GetDefault<UPlayFabRuntimeSettings>()->bIsVIPGameVersion)
	{
		UKismetSystemLibrary::ShowAdBanner(0, true);
	}
	else
	{
		UKismetSystemLibrary::HideAdBanner();
	}
}

void UPlayfabManager::TickTryUpdateUserData(float deltaTime)
{
	m_fDeltaCounter += deltaTime;

	if (m_fDeltaCounter < 5.f)
	{
		return;
	}
	PRINTF("TryUpdateUserData");

	m_fDeltaCounter = 0.f;
}

void UPlayfabManager::RequestSetNickname(FString str)
{
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Request Nickname","Request Nickname"));
	ClientModels::FUpdateUserTitleDisplayNameRequest DisplayReq;

	DisplayReq.DisplayName = str;

	GetClientAPI->UpdateUserTitleDisplayName(DisplayReq,
	                                         UPlayFabClientAPI::FUpdateUserTitleDisplayNameDelegate::CreateUObject(
		                                         this, &UPlayfabManager::OnNickNameSetSuccess)
	                                         , PlayFab::FPlayFabErrorDelegate::CreateUObject(
		                                         this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnNickNameSetSuccess(const PlayFab::ClientModels::FUpdateUserTitleDisplayNameResult& result)
{
	//result.DisplayName
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Nickname Success","Nickname Success"));
	m_bIsNicknameSet=true;
	m_LoadedNickname=result.DisplayName;

	FTimerHandle hh;
	UDiabloGameInstance::Get->GetTimerManager().SetTimer(hh, this, &UPlayfabManager::RequestGetUserData, 2.5f,
                                                         false);
}

void UPlayfabManager::OnCloudScriptSuccess(const FExeCScriptRslt& rslt)
{
	PRINTF("Cloud Script Success");
}

void UPlayfabManager::PurchaseIAPItem(FString itemUniqueId)
{
	
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
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Internet Connected","Internet Connected"));
	}
	else
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("FAIL-Internet Fail-EndApp","FAIL-Internet Fail-EndApp"));

		FGenericPlatformMisc::RequestExit(true);
		return;
	}

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	if (!Subsystem)
	{
		return;
	}


#if PLATFORM_WINDOWS
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Try Login With Desktop","Try Login With Desktop"));
	GetClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();

	PlayFab::ClientModels::FLoginWithCustomIDRequest request;
	request.CreateAccount = true;
	request.CustomId = "JungPC TestID3";
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
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("SUCCESS-GoogleLogin","SUCCESS-GoogleLogin"));
		TryLoginPlayfabGoogle(uniqueId);
	}
	else
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("FAIL-GoogleLoginFail-2","FAIL-GoogleLoginFail-2"));
		FGenericPlatformMisc::RequestExit(true);
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
			UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("FAIL-LoginStatus:NotLoggedin","FAIL-LoginStatus:NotLoggedin"));
			break;
		case ELoginStatus::UsingLocalProfile: 
			UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("FAIL-LoginStatus:UsingLocalProfile","FAIL-LoginStatus:UsingLocalProfile"));
			break;
		case ELoginStatus::LoggedIn: 
			UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("LoginStatus:LoggedIn","LoginStatus:LoggedIn"));
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
			UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Fail-Request PlayfabLogin","Fail-Request PlayfabLogin"));
		}
	}
	else
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("FAIL-GooglePlay not checked ?","FAIL-GooglePlay not checked ?"));
	}
}


void UPlayfabManager::OnSuccessPlayfabLogin(const PlayFab::ClientModels::FLoginResult& Result)
{
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("SUCCESS-Playfab Login Success","SUCCESS-Playfab Login Success"));
	PRINTF("ID:%s", *Result.PlayFabId);

	m_PlayfabID = Result.PlayFabId;

	if (Result.NewlyCreated)
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("New Player","New Player"));
	}
	//
	RequestGetAccountInfo();
	//RequestGetUserData();
}

void UPlayfabManager::RequestGetUserData()
{
	
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Request Get User Data","Request Get User Data"));
	FGetUsrDataReq req;

	//Request Data
	req.PlayFabId = m_PlayfabID;
	req.Keys.Add(Gold);
	req.Keys.Add(Dg);
	req.Keys.Add(Stat);
	req.Keys.Add(Skill);
	req.Keys.Add(Class);
	req.Keys.Add(Weapon);
	req.Keys.Add(Wing);
	req.Keys.Add(Pet);
	req.Keys.Add(Accessory);

	GetClientAPI->GetUserData(req,
	                          FGetUsrDataDele::CreateUObject(this, &UPlayfabManager::OnSuccessGetUserData),
	                          FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::SetOnlineStatus()
{
	ClientModels::FExecuteCloudScriptRequest Req;
	Req.FunctionName="SetOnlineState";
	GetClientAPI->ExecuteCloudScript(Req,
		FExeCScriptDele::CreateUObject(this,&UPlayfabManager::OnCloudScriptSuccess),
		FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::SetOfflineStatus()
{
	ClientModels::FExecuteCloudScriptRequest Req;
	Req.FunctionName="SetOfflineState";
	GetClientAPI->ExecuteCloudScript(Req,
        FExeCScriptDele::CreateUObject(this,&UPlayfabManager::OnCloudScriptSuccess),
        FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}


void UPlayfabManager::OnErrorPlayfabReq(const FFailRslt& ErrorResult)
{
	FString CodeString = UPlayFabUtilities::getErrorText(ErrorResult.ErrorCode);
	PRINTF("PlayfabRequest Error Name:%s", *ErrorResult.ErrorName);
	PRINTF("PlayfabRequest Error Message:%s", *ErrorResult.ErrorMessage);
	PRINTF("PlayfabRequest Error Code:%s", *CodeString);

	UDiabloGameInstance::Get->RequestPopupText(CodeString);
		//break;
	m_OnPlayfabError.Broadcast(CodeString);
}

void UPlayfabManager::OnSuccessGetUserData(const FGetUsrDataRslt& result)
{
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("SUCCESS-Get User Data","SUCCESS-Get User Data"));
	
	if (!result.Data.Num())
	{
		PRINTF("DataNull");
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("FAIL-Get User Data Null","FAIL-Get User Data Null"));
		//Something Fucked
	}
	//
	m_LoadedGold = result.Data[Gold].Value;
	m_LoadedDg = result.Data[Dg].Value;
	m_LoadedStat = result.Data[Stat].Value;
	m_LoadedSkill = result.Data[Skill].Value;
	m_LoadedClass = result.Data[Class].Value;
	m_LoadedWeapon = result.Data[Weapon].Value;
	m_LoadedWing = result.Data[Wing].Value;
	m_LoadedPet = result.Data[Pet].Value;
	m_LoadedAccessory = result.Data[Accessory].Value;
	//
	UDiabloGameInstance::Get->m_GoldManager->SetCurrentGold(m_LoadedGold);
	UDiabloGameInstance::Get->m_DungeonManager->SetDungeonLevel(*m_LoadedDg);
	UDiabloGameInstance::Get->m_PlayerUpgradeManager->SetUpgradeDataFromServer(m_LoadedStat, m_LoadedSkill);
	UDiabloGameInstance::Get->m_EquipManager->SetEquipDataFromServer(m_LoadedClass, m_LoadedWeapon, m_LoadedWing,
	                                                                 m_LoadedPet, m_LoadedAccessory);


	SetOnlineStatus();
	m_bIsLoginCompleted = true;
	m_bIsNicknameSet = true;

}

void UPlayfabManager::RequestGetAccountInfo()
{
	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("RequestGetAccountInfo","RequestGetAccountInfo"));
	FGetAccntInfoReq Req;

	GetClientAPI->GetAccountInfo(Req,FGetAccntInfoDele::CreateUObject(this,&UPlayfabManager::OnSuccessGetAccountInfo),
		FFailDele::CreateUObject(this,&UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnSuccessGetAccountInfo(const FGetAccntInfoRslt& rslt)
{
	if(rslt.AccountInfo->TitleInfo->isBanned)
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Banned Player","Banned Player"));
		return;
	}

	
	if(rslt.AccountInfo->TitleInfo->DisplayName.IsEmpty())
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Please Set Nickname","Please Set Nickname"));
		m_bShowNicknameSet = true;
		return;
	}

	UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Welcome","Welcome"));
	
	m_LoadedNickname=rslt.AccountInfo->TitleInfo->DisplayName;
	m_bIsNicknameSet = true;
	RequestGetUserData();
}

void UPlayfabManager::BuyIAP(FString id,bool bIsConsumable)
{
	

	FInAppPurchaseProductRequest IAPRequest;
	
	IAPRequest.bIsConsumable=bIsConsumable;
	
	IAPRequest.ProductIdentifier=id;//

	UMyInAppPurchase* Proxy = UMyInAppPurchase::CreateProxyObjectForInAppPurchase(UDiabloGameInstance::Get->GetPlCon(), IAPRequest);
	
	Proxy->OnSuccess.AddDynamic(this,&UPlayfabManager::PurchaseSuccess);
	
	Proxy->OnFailure.AddDynamic(this,&UPlayfabManager::PurchaseFail);
}

void UPlayfabManager::PurchaseSuccess(EInAppPurchaseState::Type completionStatus,
    const FInAppPurchaseProductInfo& inAppPurchaseInformation)
{
	PRINTF("Purchase Success");
	UDiabloGameInstance::Get->RequestPopupText("Purchase Success");
}

void UPlayfabManager::PurchaseFail(EInAppPurchaseState::Type completionStatus,
    const FInAppPurchaseProductInfo& inAppPurchaseInformation)
{
	PRINTF("Purchase Fail");
	UDiabloGameInstance::Get->RequestPopupText("Purchase Fail");
}
#undef LOCTEXT_NAMESPACE
