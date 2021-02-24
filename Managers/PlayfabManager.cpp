#include "PlayfabManager.h"
#include "DiabloGameInstance.h"
#include "DungeonManager.h"
#include "MobileUtilsBlueprintLibrary.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "PlayFabClientDataModels.h"
#include "PlayerUpgradeManager.h"

using namespace PlayFab;

const FString  UPlayfabManager::PlayfabDungeonIDKey = "DungeonID";

const FString  UPlayfabManager::PlayfabPlayerClassIDKey = "PlayerClassID";

const FString  UPlayfabManager::PlayerAtkDmg01Key = "PlayerAtkDmg01";

const FString  UPlayfabManager::PlayerAtkCri01Key = "PlayerAtkCri01";

const FString  UPlayfabManager::PlayerAtkCDmg01Key = "PlayerAtkCDmg01";

const FString  UPlayfabManager::PlayerGoldKey = "PlayerGold";



UPlayfabManager::UPlayfabManager()
{
	m_bLoginProcessEnd = false;
	//m_LoadedDgID ;//= "Stage1-1";
	//m_LoadedPlayerClassID;// = "Warrior01";
}

UPlayfabManager::~UPlayfabManager()
{
	
}

void UPlayfabManager::ShowBannerAd(bool able)
{
	if(able && !GetDefault<UPlayFabRuntimeSettings>()->bIsVIPGameVersion)
	{
		UKismetSystemLibrary::ShowAdBanner(0,true);
	}
	else
	{
		UKismetSystemLibrary::HideAdBanner();
	}
}

void UPlayfabManager::LoadLocalDefaultData()
{
	PRINTF("Use Local Default Data");
	m_bLoginProcessEnd = true;
	
	m_LoadedDgID="Stage1-1";
	m_LoadedPlayerClassID = "Warrior01";
	m_nLoadedPlAtkDmg01 = 1;
	m_nLoadedPlAtkCri01 = 1;
	m_nLoadedPlAtkCDmg01= 1;

	UDiabloGameInstance::Get->m_DungeonManager->LoadCurrentDungeonLevel(*m_LoadedDgID,this);
	UDiabloGameInstance::Get->m_PlayerClassManager->LoadPlayerClass(*m_LoadedPlayerClassID);
	UDiabloGameInstance::Get->m_PlayerUpgradeManager->SetUpgradeDataFromServer(this);
	//UDiabloGameInstance::Get->m_GoldManager->SetCurrentGold(result.Data[PlayerGoldKey].Value); //just zero
}

void UPlayfabManager::Init()
{
	if (m_bIsLogined)
	{
		return;
	}
	
	if (UMobileUtilsBlueprintLibrary::CheckInternetConnection())
	{
		PRINTF("Internet Connected");
	}
	else
	{
		PRINTF("Internet Fail-EndApp");

		FGenericPlatformMisc::RequestExit(true);
		return;
	}

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	if (!Subsystem)
	{
		return;
	}

	IOnlineExternalUIPtr ExternalUi = Subsystem->GetExternalUIInterface();

	if (!ExternalUi)
	{
		PRINTF("noui-GoogleLogin Fail");
		LoadLocalDefaultData();
		return;
	}

	ExternalUi->ShowLoginUI(0, false, false,
	                        FOnLoginUIClosedDelegate::CreateUObject(this, &UPlayfabManager::HandleExternalUIClose));
}

int UPlayfabManager::GetPlAtkDmg01Lv()
{
	return m_nLoadedPlAtkDmg01;
}

int UPlayfabManager::GetPlAtkCri01Lv()
{
	return m_nLoadedPlAtkCri01;
}

int UPlayfabManager::GetPlAtkCDmg01Lv()
{
	return m_nLoadedPlAtkCDmg01;
}

void UPlayfabManager::HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex,
                                            const FOnlineError& error)
{
	if (error.bSucceeded)
	{
		PRINTF("GoogleLogin Success");
		TryLoginPlayfabGoogle(uniqueId);
	}
	else
	{
		PRINTF("GoogleLogin Fail");
		
		LoadLocalDefaultData();
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
		case ELoginStatus::NotLoggedIn: PRINTF("LoginStatus:NotLoggedin");
			break;
		case ELoginStatus::UsingLocalProfile: PRINTF("LoginStatus:UsingLocalProfile");
			break;
		case ELoginStatus::LoggedIn: PRINTF("LoginStatus:LoggedIn");
			m_bIsLogined = true;
			m_bLoginProcessEnd = true;
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
			PRINTF("FAIL - Request PlayfabLogin Fail");
		}
	}
	else
	{
		PRINTF("FAIL - GooglePlay not checked ?");
	}
}

void UPlayfabManager::OnSuccessPlayfabLogin(const PlayFab::ClientModels::FLoginResult& Result)
{
	PRINTF("Playfab Login Success");

	if (Result.NewlyCreated)
	{
		PRINTF("Playfab New Player Created");
	}

	PRINTF("ID:%s", *Result.PlayFabId);

	m_PlayfabID = Result.PlayFabId;
	
	FGetUsrDataReq req;
	
	//Request Data
	req.PlayFabId = m_PlayfabID;
	req.Keys.Add(PlayfabDungeonIDKey);
	req.Keys.Add(PlayfabPlayerClassIDKey);
	req.Keys.Add(PlayerAtkDmg01Key);
	req.Keys.Add(PlayerAtkCri01Key);
	req.Keys.Add(PlayerAtkCDmg01Key);
	req.Keys.Add(PlayerGoldKey);
	

	GetClientAPI->GetUserData(req,
		FGetUsrDataDele::CreateUObject(this, &UPlayfabManager::OnSuccessGetUserData),
		FFailDele::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));

}


void UPlayfabManager::OnErrorPlayfabReq(const FFailRslt& ErrorResult)
{	
	PRINTF("PlayfabRequest Error Name:%s", *ErrorResult.ErrorName);
	PRINTF("PlayfabRequest Error Message:%s", *ErrorResult.ErrorMessage);
	PRINTF("PlayfabRequest Error Code:%s", *UPlayFabUtilities::getErrorText(ErrorResult.ErrorCode));
}

void UPlayfabManager::OnSuccessGetUserData(const FGetUsrDataRSlt& result) 
{
	PRINTF("GetUserDataSuccess");
	//
	m_LoadedDgID = result.Data[PlayfabDungeonIDKey].Value;
	m_LoadedPlayerClassID = result.Data[PlayfabPlayerClassIDKey].Value;
	m_nLoadedPlAtkDmg01 = FCString::Atoi(*result.Data[PlayerAtkDmg01Key].Value);
	m_nLoadedPlAtkCri01 = FCString::Atoi(*result.Data[PlayerAtkCri01Key].Value);
	m_nLoadedPlAtkCDmg01 = FCString::Atoi(*result.Data[PlayerAtkCDmg01Key].Value);
	//
	UDiabloGameInstance::Get->m_GoldManager->SetCurrentGold(result.Data[PlayerGoldKey].Value);
	UDiabloGameInstance::Get->m_DungeonManager->LoadCurrentDungeonLevel(*m_LoadedDgID,this);
	UDiabloGameInstance::Get->m_PlayerClassManager->LoadPlayerClass(*m_LoadedPlayerClassID);
	UDiabloGameInstance::Get->m_PlayerUpgradeManager->SetUpgradeDataFromServer(this);
}


