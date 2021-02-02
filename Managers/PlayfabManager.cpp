#include "PlayfabManager.h"
#include "DiabloGameMode.h"
#include "Core/PlayFabClientAPI.h"
#include "MobileUtilsBlueprintLibrary.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "PlayFabAdminDataModels.h"
#include "PlayFabUtilities.h"


void UPlayfabManager::Init()
{
	if(m_bIsLogined)
	{
		return;
	}
	m_bIsLogined = true;
	
	if(UMobileUtilsBlueprintLibrary::CheckInternetConnection())
	{
		PRINTF("Internet Connected");
	}
	else
	{
		PRINTF("Internet Fail");
	}

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	if(!Subsystem)
	{
		return;
	}

	IOnlineExternalUIPtr ExternalUi = Subsystem->GetExternalUIInterface();

	if(!ExternalUi)
	{
		return;
	}
	
	ExternalUi->ShowLoginUI(0, false, false, FOnLoginUIClosedDelegate::CreateUObject(this, &UPlayfabManager::HandleExternalUIClose));
}

void UPlayfabManager::OnSuccess(const PlayFab::ClientModels::FLoginResult& Result) const
{
	PRINTF("Playfab Login Success");
	
	if(Result.NewlyCreated)
	{
		PRINTF("Playfab New Player Created");
	}

	PRINTF("ID:%s",*Result.PlayFabId);
}

void UPlayfabManager::OnError(const PlayFab::FPlayFabCppError& ErrorResult) const
{
	PRINTF("Playfab Login Error Name:%s",*ErrorResult.ErrorName);
	PRINTF("Playfab Login Error Message:%s",*ErrorResult.ErrorMessage);
	PRINTF("Playfab Login Error Code:%s",*UPlayFabUtilities::getErrorText(ErrorResult.ErrorCode));
}

void UPlayfabManager::TryLoginPlayfabGoogle(TSharedPtr<const FUniqueNetId> uniqueId)//클라아이디를 키서명으로 바꿔봄
{
	if(UMobileUtilsBlueprintLibrary::CheckGooglePlayServices())
	{
		PRINTF("GooglePlay Connected - UniqueID:%s",*uniqueId->ToString());
		
		IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

		IOnlineIdentityPtr OnlineIdentity = Subsystem->GetIdentityInterface();

		PRINTF("UserID:%s", *OnlineIdentity->GetPlayerNickname(*uniqueId));

		PRINTF("TryGetUnique:%s", *OnlineIdentity->GetUniquePlayerId(0)->ToString());

		PRINTF("AuthToken:%s", *OnlineIdentity->GetAuthToken(0));

		auto Status = OnlineIdentity->GetLoginStatus(0);

		switch (Status) { case ELoginStatus::NotLoggedIn:	PRINTF("LoginStatus:NotLoggedin");  break;
		case ELoginStatus::UsingLocalProfile:	PRINTF("LoginStatus:UsingLocalProfile");	break;
		case ELoginStatus::LoggedIn:	PRINTF("LoginStatus:LoggedIn");	break;
		default: ; }

		clientAPI = IPlayFabModuleInterface::Get().GetClientAPI();

		PlayFab::ClientModels::FLoginWithGoogleAccountRequest request;
		request.CreateAccount = true;
		request.ServerAuthCode = OnlineIdentity->GetAuthToken(0);
		request.PlayerSecret = GetDefault<UPlayFabRuntimeSettings>()->DeveloperSecretKey;
		request.TitleId =  GetDefault<UPlayFabRuntimeSettings>()->TitleId;

		bool Result =  clientAPI->LoginWithGoogleAccount(request,
		                                                 PlayFab::UPlayFabClientAPI::FLoginWithGoogleAccountDelegate::CreateUObject(
			                                                 this, &UPlayfabManager::OnSuccess),
		                                                 PlayFab::FPlayFabErrorDelegate::CreateUObject(
			                                                 this, &UPlayfabManager::OnError)
		);


		if(!Result)
		{
			PRINTF("FAIL - Request PlayfabLogin Fail");
		}
	}
	else
	{
		PRINTF("FAIL - GooglePlay not checked ?");
	}
	}

	void UPlayfabManager::HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex,
	                                            const FOnlineError& error)
	{
		if(error.bSucceeded)
		{
			PRINTF("GoogleLogin Success");	
		}
		else
		{
			PRINTF("GoogleLogin Fail");
			EOnlineErrorResult ErrorResult = error.GetErrorResult();

			switch (ErrorResult)
			{
			case EOnlineErrorResult::Success:PRINTF("GoogleLogin Success");break;
			case EOnlineErrorResult::NoConnection: PRINTF("NoConnection");	break;
			case EOnlineErrorResult::RequestFailure: PRINTF("RequestFailure");	break;
			case EOnlineErrorResult::InvalidCreds: PRINTF("InvalidCreds");	break;
			case EOnlineErrorResult::InvalidUser: PRINTF("InvalidUser");	break;
			case EOnlineErrorResult::InvalidAuth: PRINTF("InvalidAuth");	break;
			case EOnlineErrorResult::AccessDenied: PRINTF("AccessDenied");	break;
			case EOnlineErrorResult::TooManyRequests: PRINTF("TooManyRequests");	break;
			case EOnlineErrorResult::AlreadyPending: PRINTF("AlreadyPending");	break;
			case EOnlineErrorResult::InvalidParams: PRINTF("InvalidParams");	break;
			case EOnlineErrorResult::CantParse: PRINTF("CantParse");	break;
			case EOnlineErrorResult::InvalidResults: PRINTF("InvalidResults");	break;
			case EOnlineErrorResult::IncompatibleVersion: PRINTF("IncompatibleVersion");	break;
			case EOnlineErrorResult::NotConfigured: PRINTF("NotConfigured");	break;
			case EOnlineErrorResult::NotImplemented: PRINTF("NotImplemented");	break;
			case EOnlineErrorResult::MissingInterface: PRINTF("MissingInterface");	break;
			case EOnlineErrorResult::Canceled: PRINTF("Canceled");	break;
			case EOnlineErrorResult::FailExtended: PRINTF("FailExtended");	break;
			case EOnlineErrorResult::Unknown: PRINTF("Unknown");	break;
			default: ;
			}
		
			PRINTF("Message::%s",*error.ErrorMessage.ToString());
			PRINTF("Raw::%s",*error.GetErrorRaw());
		}
	
		TryLoginPlayfabGoogle(uniqueId);
	}

