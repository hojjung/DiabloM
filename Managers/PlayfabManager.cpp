#include "PlayfabManager.h"

#include "DiabloGameInstance.h"
#include "DungeonManager.h"
#include "Core/PlayFabClientAPI.h"
#include "MobileUtilsBlueprintLibrary.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "PlayFabAdminDataModels.h"
#include "PlayFabClientDataModels.h"
#include "PlayFabServerDataModels.h"
#include "PlayFabUtilities.h"

using namespace PlayFab;

UPlayfabManager::UPlayfabManager()
{
	m_bLoginProcessEnd = false;
	//m_LoadedDgID ;//= "Stage1-1";
	//m_LoadedPlayerClassID;// = "Warrior01";
}

UPlayfabManager::~UPlayfabManager()
{
	
}

void UPlayfabManager::LoadLocalDefaultData()
{
	m_LoadedDgID="Stage1-1";
	m_LoadedPlayerClassID = "Warrior01";
	m_bLoginProcessEnd = true;
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

		clientAPI = IPlayFabModuleInterface::Get().GetClientAPI();

		PlayFab::ClientModels::FLoginWithGoogleAccountRequest request;
		request.CreateAccount = true;
		request.ServerAuthCode = OnlineIdentity->GetAuthToken(0);
		request.PlayerSecret = GetDefault<UPlayFabRuntimeSettings>()->DeveloperSecretKey;
		request.TitleId = GetDefault<UPlayFabRuntimeSettings>()->TitleId;

		bool Result = clientAPI->LoginWithGoogleAccount(request,
		                                                PlayFab::UPlayFabClientAPI::FLoginWithGoogleAccountDelegate::CreateUObject(
			                                                this, &UPlayfabManager::OnSuccessPlayfabLogin),
		                                                PlayFab::FPlayFabErrorDelegate::CreateUObject(
			                                                this, &UPlayfabManager::OnLoginErrorPlayfabReq)
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
	
	PlayFab::ClientModels::FGetUserDataRequest req;
	
	req.PlayFabId = m_PlayfabID;
	
	req.Keys.Add("DungeonID");
	
	req.Keys.Add("PlayerClassID");

	clientAPI->GetUserData(req,
		PlayFab::UPlayFabClientAPI::FGetUserDataDelegate::CreateUObject(this, &UPlayfabManager::OnSuccessGetUserData),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnErrorPlayfabReq));
}

void UPlayfabManager::OnLoginErrorPlayfabReq(const PlayFab::FPlayFabCppError& ErrorResult) 
{
	PRINTF("PlayfabLogin Error Name:%s", *ErrorResult.ErrorName);
	PRINTF("PlayfabLogin Error Message:%s", *ErrorResult.ErrorMessage);
	PRINTF("PlayfabLogin Error Code:%s", *UPlayFabUtilities::getErrorText(ErrorResult.ErrorCode));

	
}

void UPlayfabManager::OnErrorPlayfabReq(const PlayFab::FPlayFabCppError& ErrorResult)
{	
	PRINTF("PlayfabRequest Error Name:%s", *ErrorResult.ErrorName);
	PRINTF("PlayfabRequest Error Message:%s", *ErrorResult.ErrorMessage);
	PRINTF("PlayfabRequest Error Code:%s", *UPlayFabUtilities::getErrorText(ErrorResult.ErrorCode));
}

void UPlayfabManager::OnSuccessGetUserData(const PlayFab::ClientModels::FGetUserDataResult& result) 
{
	PRINTF("GetUserDataSuccess");
	//
	m_LoadedDgID =*result.Data["DungeonID"].Value;
	m_LoadedPlayerClassID =*result.Data["PlayerClassID"].Value;
	//
	PRINTF("DGID: %s",*m_LoadedDgID.ToString());
	PRINTF("PCID: %s",*m_LoadedPlayerClassID.ToString());
	//
	UDiabloGameInstance::Get->m_DungeonManager->LoadCurrentDungeonLevel(m_LoadedDgID,this);
	UDiabloGameInstance::Get->m_PlayerClassManager->LoadPlayerClass(m_LoadedPlayerClassID);
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

