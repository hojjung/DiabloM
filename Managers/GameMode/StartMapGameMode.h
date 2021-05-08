// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "DiabloGameMode.h"
#include "PlayFabClientDataModels.h"
#include "GameFramework/GameMode.h"
#include "StartMapGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API AStartMapGameMode : public ADiabloGameMode
{
	GENERATED_BODY()

public:
	AStartMapGameMode();
	
	virtual void BeginPlay() override;

};
