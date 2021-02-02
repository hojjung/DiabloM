#pragma once
#include "DiabloM.h"
#include "GameFramework/GameMode.h"
#include "DiaStartMenu.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API ADiaStartMenu : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void StartPlay() override;
};
