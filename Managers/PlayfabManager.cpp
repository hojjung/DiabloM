#include "PlayfabManager.h"
#include "Core/PlayFabClientAPI.h"

void UPlayfabManager::Init()
{
	clientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
	//clientAPI-> SetTitleId (TEXT ( "144"));

	//GetDefault<UPlayFabRuntimeSettings>()->TitleId= (TEXT("144"));
	
	PlayFab::ClientModels::FLoginWithCustomIDRequest request;
	request.CustomId = TEXT("GettingStarted");
	request.CreateAccount = true;

	clientAPI->LoginWithCustomID(request,
        PlayFab::UPlayFabClientAPI::FLoginWithCustomIDDelegate::CreateUObject(this, &UPlayfabManager::OnSuccess),
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayfabManager::OnError)
    );
}

void UPlayfabManager::OnSuccess(const PlayFab::ClientModels::FLoginResult& Result) const
{
	PRINTF("Success");
}

void UPlayfabManager::OnError(const PlayFab::FPlayFabCppError& ErrorResult) const
{
	PRINTF("Error");
}
