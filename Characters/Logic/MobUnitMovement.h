// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "UnitMovement.h"
#include "Characters/Pawns/MonsterPawn.h"

#include "MobUnitMovement.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UMobUnitMovement : public UUnitMovement
{
	GENERATED_BODY()
protected:
    UPROPERTY()
    TWeakObjectPtr<AMonsterPawn> m_OwnerMob;
    
public:
   virtual void BeginPlay() override;
    
   virtual FRotator GetRotationNotMove(const FRotator& rot) const override;
};
