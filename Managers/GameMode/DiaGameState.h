#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DiaGameState.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API ADiaGameState : public AGameState
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};
