#include "PlayfabManager.h"
#include "DiabloGameInstance.h"
#include "DungeonManager.h"
#include "EquipManager.h"
#include "MobileUtilsBlueprintLibrary.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "PlayFabClientDataModels.h"
#include "PlayerUpgradeManager.h"
#include "PlayFabUtilities.h"

using namespace PlayFab;
//dungeon 1111200
const FString  UPlayfabManager::Gold = "Gold";
const FString  UPlayfabManager::Dg = "Dg";
const FString  UPlayfabManager::Stat = "Stat";
const FString  UPlayfabManager::Skill = "Skill";

const FString  UPlayfabManager::Class = "Class";
const FString  UPlayfabManager::Weapon = "Weapon";
const FString  UPlayfabManager::Wing = "Wing";
const FString  UPlayfabManager::Pet = "Pet";
const FString  UPlayfabManager::Accessory = "Accessory";


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
	if (m_bLoginProcessStarted)
	{
		return;
	}

	m_bLoginProcessStarted = true;
	
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
	PRINTF("PL-ANdroid");
	IOnlineExternalUIPtr ExternalUi = Subsystem->GetExternalUIInterface();

	if (!ExternalUi)
	{
		PRINTF("noui-GoogleLogin Fail");
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

	PRINTF("ID:%s", *Result.PlayFabId);

	m_PlayfabID = Result.PlayFabId;
	
	if (Result.NewlyCreated)
	{
		PRINTF("Playfab New Player Created");
		PRINTF("Please Wait For Update Data");

		FTimerHandle hh;
		UDiabloGameInstance::Get->GetTimerManager().SetTimer(hh,this,&UPlayfabManager::RequestGetUserData,2.5f,false);
		return;
	}

	RequestGetUserData();
}

void UPlayfabManager::RequestGetUserData()
{
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


void UPlayfabManager::OnErrorPlayfabReq(const FFailRslt& ErrorResult)
{	
	PRINTF("PlayfabRequest Error Name:%s", *ErrorResult.ErrorName);
	PRINTF("PlayfabRequest Error Message:%s", *ErrorResult.ErrorMessage);
	PRINTF("PlayfabRequest Error Code:%s", *UPlayFabUtilities::getErrorText(ErrorResult.ErrorCode));
}

void UPlayfabManager::OnSuccessGetUserData(const FGetUsrDataRSlt& result) 
{
	PRINTF("GetUserDataSuccess");
	if(!result.Data.Num())
	{
		PRINTF("DataNull");
		//Something Fucked
	}
	//
	m_LoadedGold = result.Data[Gold	].Value;
	m_LoadedDg = result.Data[Dg		].Value;
	m_LoadedStat = result.Data[Stat	].Value;
	m_LoadedSkill = result.Data[Skill	].Value;
	m_LoadedClass = result.Data[Class	].Value;
	m_LoadedWeapon = result.Data[Weapon ].Value;
	m_LoadedWing = result.Data[Wing	].Value;
	m_LoadedPet = result.Data[Pet	].Value;
	m_LoadedAccessory = result.Data[Accessory].Value;
	//
	UDiabloGameInstance::Get->m_GoldManager->SetCurrentGold(m_LoadedGold);
	UDiabloGameInstance::Get->m_DungeonManager->SetDungeonLevel(*m_LoadedDg);
	UDiabloGameInstance::Get->m_PlayerUpgradeManager->SetUpgradeDataFromServer(m_LoadedStat,m_LoadedSkill);
	UDiabloGameInstance::Get->m_EquipManager->SetEquipDataFromServer(m_LoadedClass,m_LoadedWeapon,m_LoadedWing,m_LoadedPet,m_LoadedAccessory);


	m_bIsLoginCompleted = true;
}


