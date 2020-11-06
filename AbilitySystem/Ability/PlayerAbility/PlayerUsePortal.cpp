#include "PlayerUsePortal.h"
#include "AbilitySystem/Task/PlayMontageAndWaitForEvent.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"


UPlayerUsePortal::UPlayerUsePortal()//스턴과 피격에 끊겨야하고 움직일수 없음,버튼 뗏을때도 끊겨야함,완료될때 포탈 이펙트 출력
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Combat.Ability.Skill")));

    m_AbilityInputID = EAbilityInputID::MakingPortal;

    m_AbilityID = EAbilityInputID::MakingPortal;

    m_TagEventEndAbility = FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility"));

    m_TagEventReceive = FGameplayTag::RequestGameplayTag(FName("Combat.Ability.Skill.Portal"));

    AbilityTags.AddTag(m_TagEventReceive);
    
    ActivationOwnedTags.AddTag(m_TagEventReceive);

}

void UPlayerUsePortal::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
    }
    
    PRINTF("ActPortal!");
    float CastSpeed = m_PlayerPawn->GetCastSpeed();
    //
    m_DmgTookDelegate = m_PlayerPawn->GetAttributeSet()->m_OnDmgTook.
                                        AddUObject(this, &UPlayerUsePortal::OnDamageTook);
    
    PlayAbilityAnimation(m_PotionDrinkMotion, NAME_None, 1.f/CastSpeed);
    m_PlayerPawn->SetBlockMove();
    //void ExecuteGameplayCue(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);
    m_Audio=UGameplayStatics::SpawnSoundAtLocation(m_PlayerPawn,m_PortalSound,m_PlayerPawn->GetActorLocation(),FRotator(),0.5f,1.2f);
}

void UPlayerUsePortal::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UPlayerUsePortal::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPlayerUsePortal::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
    if (EventTag == m_TagEventEndAbility)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return;
    }
    
    if (EventTag == m_TagEventReceive)
    {
        if (!m_PlayerPawn)
        {
            EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        }

        PRINTF("Portal!");
        
        UDiabloGameInstance::Get->GetDungeonManager()->PortalToVillage();
    }
}

void UPlayerUsePortal::PlayAbilityAnimation(UAnimMontage* MontageToPlay, FName playSection, float AttackSpeed)
{
    UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
      this, NAME_None, MontageToPlay, FGameplayTagContainer(), AttackSpeed,
      playSection, true, 1.0f);

    Task->OnBlendOut.AddDynamic(this, &UPlayerUsePortal::OnCompleted);
    Task->OnCompleted.AddDynamic(this, &UPlayerUsePortal::OnCompleted);
    Task->OnInterrupted.AddDynamic(this, &UPlayerUsePortal::OnCancelled);
    Task->OnCancelled.AddDynamic(this, &UPlayerUsePortal::OnCancelled);
    Task->EventReceived.AddDynamic(this, &UPlayerUsePortal::EventReceived);

    Task->ReadyForActivation();
}
void UPlayerUsePortal::OnDamageTook(float damage)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
  
}

void UPlayerUsePortal::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    m_PlayerPawn->GetAttributeSet()->m_OnDmgTook.Remove(m_DmgTookDelegate);
    m_PlayerPawn->SetUnblockMove();
    m_Audio->Deactivate();
}

bool UPlayerUsePortal::CommitAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    bool PreReturn=Super::CommitAbility(Handle, ActorInfo, ActivationInfo);

    return PreReturn && UDiabloGameInstance::Get->GetDungeonManager()->IsPlayerInDg();
}
