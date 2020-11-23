// My First Hack n Slash


#include "MobUnitMovement.h"

#include "MonsterPawn.h"

void UMobUnitMovement::BeginPlay()
{
    Super::BeginPlay();

    m_OwnerMob = Cast<AMonsterPawn>(GetOwner());
}

FRotator UMobUnitMovement::GetRotationNotMove(const FRotator& rot) const
{
    return m_OwnerMob->GetHomingRotToTarget();
}
