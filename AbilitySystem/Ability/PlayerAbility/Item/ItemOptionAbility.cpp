// My First Hack n Slash


#include "ItemOptionAbility.h"

UItemOptionAbility::UItemOptionAbility()
{
    m_bActivateAbilityOnGranted=true;
}

void UItemOptionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    PRINTF("ItemOptionAbility 11");
}

void UItemOptionAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    PRINTF("ItemOptionAbility 22");
}

void UItemOptionAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateCancelAbility)
{
    PRINTF("ItemOptionAbility 33");
}
