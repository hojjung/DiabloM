// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "GameFramework/GameMode.h"
#include "MagicDragonMode.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API AMagicDragonMode : public AGameMode
{
	GENERATED_BODY()
public:
	AMagicDragonMode();

	virtual void Tick(float DeltaSeconds) override;
};
