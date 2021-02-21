#include "AnimNotify_SendGameEvent.h"
#include "Characters/UnitPawn.h"

UAnimNotify_SendGameEvent::UAnimNotify_SendGameEvent()
{
    //m_CachedAttackEvent=FGameplayTag::RequestGameplayTag("Combat.Ability.Skill.BaseAttack");//for spawn projectile
}

void UAnimNotify_SendGameEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    AUnitPawn* Unit=Cast<AUnitPawn>( MeshComp->GetOwner());

    if(!Unit)
    {
        return;
    }
    
}
