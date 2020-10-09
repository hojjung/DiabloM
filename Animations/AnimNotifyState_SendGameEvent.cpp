
#include "AnimNotifyState_SendGameEvent.h"
#include "GameplayAbilityTypes.h"
#include "Characters/UnitPawn.h"

UAnimNotifyState_SendGameEvent::UAnimNotifyState_SendGameEvent()
{
    m_CachedAttackEvent=FGameplayTag::RequestGameplayTag("Ability.BaseAttack");//for spawn projectile
}

void UAnimNotifyState_SendGameEvent::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                 float TotalDuration)
{
    
}

void UAnimNotifyState_SendGameEvent::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
    float FrameDeltaTime)
{

    
    // AUnitPawn* Unit=Cast<AUnitPawn>( MeshComp->GetOwner());
    //
    // if(!Unit->GetFocusedTarget())
    // {
    //     return;
    // }
    //
    // if (Unit->GetAlreadyAttacked().Contains(OtherActor))
    // {
    //     return;
    // }
    //
    // FGameplayEventData EventData;
    //
    // EventData.Instigator = Unit;
    // EventData.Target = Unit->GetFocusedTarget();
    //
    // UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Unit,m_CachedAttackEvent,EventData);
}

void UAnimNotifyState_SendGameEvent::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

}
