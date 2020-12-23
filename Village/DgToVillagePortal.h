// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Village/Portal.h"
#include "DgToVillagePortal.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API ADgToVillagePortal : public APortal
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override; // need binding
	
public:
	virtual void Interact(AActor* instigator)override;
};
