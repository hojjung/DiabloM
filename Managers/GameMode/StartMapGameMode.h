// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "PlayFabClientDataModels.h"
#include "GameFramework/GameMode.h"
#include "StartMapGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API AStartMapGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	bool m_bLock = false;
	virtual void Tick(float DeltaSeconds) override;
};
