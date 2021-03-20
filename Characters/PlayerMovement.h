// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"


#include "Characters/UnitMovement.h"
#include "PlayerMovement.generated.h"

class UNavigationSystemV1;
/**
 * 
 */
UCLASS()
class DIABLOM_API UPlayerMovement : public UUnitMovement
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	UNavigationSystemV1* m_NavSys;
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void MoveProceed(float DeltaTime) override;
	
};
