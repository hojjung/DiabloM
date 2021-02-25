#include "PlayfabManager.h"
#include "DiabloGameInstance.h"
#include "DungeonManager.h"
#include "EquipManager.h"
#include "MobileUtilsBlueprintLibrary.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "PlayFabClientDataModels.h"
#include "PlayerUpgradeManager.h"

using namespace PlayFab;
//dungeon 1111200
const FString  UPlayfabManager::PlayerAtkDmg01Key = "PlayerAtkDmg01";
const FString  UPlayfabManager::PlayerAtkCri01Key = "PlayerAtkCri01";
const FString  UPlayfabManager::PlayerAtkCDmg01Key = "PlayerAtkCDmg01";
//
const FString  UPlayfabManager::PlayerSkill01Key = "PlayerSkill01";
const FString  UPlayfabManager::PlayerSkill02Key = "PlayerSkill02";
const FString  UPlayfabManager::PlayerSkill03Key = "PlayerSkill03";
//
const FString  UPlayfabManager::PlayerClassKey = "PlayerClass";
const FString  UPlayfabManager::PlayerWeaponKey = "PlayerWeapon";
const FString  UPlayfabManager::PlayerWingKey = "PlayerWing";
const FString  UPlayfabManager::PlayerPetKey = "PlayerPet";
const FString  UPlayfabManager::PlayerAccessoryKey = "PlayerAccessory";
//
const FString  UPlayfabManager::DungeonUnlockKey = "DungeonUnlock";
const FString  UPlayfabManager::PlayerGoldKey = "PlayerGold";
//


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

void UPlayfabManager::TickTryUpdateUserData(float deltaTime)
{
	m_fDeltaCounter+=deltaTime;

	if(m_fDeltaCounter<5.f)
	{
		return;
	}
	PRINTF("TryUpdateUserData");

	m_fDeltaCounter = 0.f;

	
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


	#if PLATFORM_WINDOWS
	GetClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();

	PlayFab::ClientModels::FLoginWithCustomIDRequest request;
	request.CreateAccount = true;
	request.CustomId = "JungPC TestID";
	request.TitleId = GetDefault<UPlayFabRuntimeSettings>()->TitleId;

	bool Result = GetClientAPI->LoginWithCustomID(request,
                                                    PlayFab::UPlayFabClientAPI::FLoginWithGoogleAccountDelegate::CreateUObject(
                                                        this, &UPlayfabManager::OnSuccessPlayfabLogin),
                                                    PlayFab::FPlayFabErrorDelegate::CreateUObject(
                                                        this, &UPlayfabManager::OnErrorPlayfabReq)
    );


	#endif
	
	#if PLATFORM_ANDROID
	
	IOnlineExternalUIPtr ExternalUi = Subsystem->GetExternalUIInterface();

	if (!ExternalUi)
	{
		PRINTF("noui-GoogleLogin Fail");
		LoadLocalDefaultData();
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
		PRINTF("GoogleLogin Success");
		TryLoginPlayfabGoogle(uniqueId);
	}
	else
	{
		PRINTF("GoogleLogin Fail");
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
		FPlayerClassSpec PlayerClass;
		m_LoadedPlayerClass =  PlayerClass.ParseToString();
		
		FEquipmentSpec Wing;
		m_LoadedAccessory =  Wing.ParseToString();
		
		FEquipmentSpec Pet;
		m_LoadedPet = Pet.ParseToString();
		
		FEquipmentSpec Weapon;
		m_LoadedWeapon = Weapon.ParseToString();
		
		FEquipmentSpec Accessory;
		m_LoadedAccessory = Accessory.ParseToString();
	}

	PRINTF("ID:%s", *Result.PlayFabId);

	m_PlayfabID = Result.PlayFabId;
	
	FGetUsrDataReq req;
	
	//Request Data
	req.PlayFabId = m_PlayfabID;
	req.Keys.Add(PlayerGoldKey);
	req.Keys.Add(DungeonUnlockKey);
	
	req.Keys.Add(PlayerAtkDmg01Key);
	req.Keys.Add(PlayerAtkCri01Key);
	req.Keys.Add(PlayerAtkCDmg01Key);
	
	req.Keys.Add(PlayerSkill01Key);
	req.Keys.Add(PlayerSkill02Key);
	req.Keys.Add(PlayerSkill03Key);

	req.Keys.Add(PlayerClassKey);
	req.Keys.Add(PlayerWeaponKey);
	req.Keys.Add(PlayerWingKey);
	req.Keys.Add(PlayerPetKey);
	req.Keys.Add(PlayerAccessoryKey);
	

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
	m_LoadedDgUnlockedID = result.Data[DungeonUnlockKey].Value;
	//
	
	//
	m_nLoadedPlAtkDmg01 = FCString::Atoi(*result.Data[PlayerAtkDmg01Key].Value);
	m_nLoadedPlAtkCri01 = FCString::Atoi(*result.Data[PlayerAtkCri01Key].Value);
	m_nLoadedPlAtkCDmg01 = FCString::Atoi(*result.Data[PlayerAtkCDmg01Key].Value);
	//
	m_nLoadedPlSkill01 = FCString::Atoi(*result.Data[PlayerSkill01Key].Value);
	m_nLoadedPlSkill02 = FCString::Atoi(*result.Data[PlayerSkill02Key].Value);
	m_nLoadedPlSkill03 = FCString::Atoi(*result.Data[PlayerSkill03Key].Value);
	//
	
	//
	UDiabloGameInstance::Get->m_GoldManager->SetCurrentGold(result.Data[PlayerGoldKey].Value);
	UDiabloGameInstance::Get->m_DungeonManager->LoadCurrentDungeonLevel(*m_LoadedDgUnlockedID,this);
	UDiabloGameInstance::Get->m_PlayerUpgradeManager->SetUpgradeDataFromServer(this);
	UDiabloGameInstance::Get->m_EquipManager->SetEquipDataFromServer(this);
}


