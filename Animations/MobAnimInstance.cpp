// My First Hack n Slash


#include "MobAnimInstance.h"

#include "Characters/MonsterPawn.h"

void FMobAnimInstanceProxy::InitializeObjects(UAnimInstance* InAnimInstance)
{
    Super::InitializeObjects(InAnimInstance);
    
    m_MobAnim = Cast<UMobAnimInstance>(InAnimInstance);
}

void FMobAnimInstanceProxy::Update(float DeltaSeconds)
{
    m_MobAnim->UpdateMoveFlag();
}

void UMobAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();
    
    m_Owner=Cast<AMonsterPawn>( TryGetPawnOwner());
}

void UMobAnimInstance::UpdateMoveFlag()
{
#if WITH_EDITOR
    if(!m_Owner)
    {
        return;
    }
#endif
    m_bIsMoving=m_Owner->IsMoving();
}

