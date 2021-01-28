// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "PlayFab.h"
#include "Core/PlayFabError.h"
#include "Core/PlayFabClientDataModels.h"
#include "PlayfabManager.generated.h"

UCLASS()
class DIABLOM_API UPlayfabManager : public UObject
{
	GENERATED_BODY()
public:
	void Init();
	
	void OnSuccess(const PlayFab::ClientModels::FLoginResult& Result) const;
	
	void OnError(const PlayFab::FPlayFabCppError& ErrorResult) const;

private:
	PlayFabClientPtr clientAPI = nullptr;
};
