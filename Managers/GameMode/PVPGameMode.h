// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "GameFramework/GameMode.h"
#include "PVPGameMode.generated.h"

class AOtherPlayerPawn;
/**
 * 
 */
UCLASS()
class DIABLOM_API APVPGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	APVPGameMode();
	
	virtual void Tick(float DeltaSeconds) override;

};
