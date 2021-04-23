// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "PlayFabClientDataModels.h"
#include "UObject/NoExportTypes.h"
#include "PVPManager.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPVPManager : public UObject
{
	GENERATED_BODY()
public:
	void RequestPVPMatching();

	void OnRequestComplete(const PlayFab::ClientModels::FGetLeaderboardAroundPlayerResult& rslt);

	void OnGetOtherPlayerSuccess(const PlayFab::ClientModels::FGetUserDataResult&);
};
