// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "OnlineError.h"
#include "PlayFab.h"
#include "Core/PlayFabError.h"
#include "Core/PlayFabClientDataModels.h"
#include "PlayfabManager.generated.h"

UCLASS()
class DIABLOM_API UPlayfabManager : public UObject
{
	GENERATED_BODY()
public:
	bool m_bIsLogined = false;
	
	void Init();
	
	void OnSuccess(const PlayFab::ClientModels::FLoginResult& Result) const;
	
	void OnError(const PlayFab::FPlayFabCppError& ErrorResult) const;

	void HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex, const FOnlineError& error);

private:
	void TryLoginPlayfabGoogle(TSharedPtr<const FUniqueNetId> uniqueId);

private:
	PlayFabClientPtr clientAPI = nullptr;
};
