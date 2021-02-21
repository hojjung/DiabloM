// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DiaPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API ADiaPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};
