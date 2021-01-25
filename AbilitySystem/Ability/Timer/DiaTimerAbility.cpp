#include "DiaTimerAbility.h"

#include "Characters/PlayerDiabloCharacter.h"
#include "Characters/UnitPawn.h"
#include "Perception/PawnSensingComponent.h"


UDiaTimerAbility::UDiaTimerAbility()
{
    m_bActivateAbilityOnGranted = true;
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;



    m_fInterval = 0.33f;
}

void UDiaTimerAbility::SetSensingUpdatesEnabled(const bool bEnabled)
{
    if (bEnabled && m_fInterval > 0.f)
    {
        const float InitialDelay = (m_fInterval * FMath::SRand()) + KINDA_SMALL_NUMBER;

        SetTimer(InitialDelay);
    }
    else
    {
        m_OwnerUnit->GetWorldTimerManager().ClearTimer(m_TimerHandle_OnTimer);
        //SetTimer(0.f);
    }
}


void UDiaTimerAbility::SetGameEffectSpec()
{
    m_TimerEffectSpecHandle=MakeOutgoingGameplayEffectSpec(
      m_EffectWant_SetByCaller, GetAbilityLevel());

    float Value=GetAttributeWantFrom();
    
   m_TimerEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_GamePlayTagSetByCaller,Value);
    
    PRINTF("Timer Value:%f",Value);
}

void UDiaTimerAbility::OnTimer()
{
    m_OwnerUnit->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToSelf(
        *m_TimerEffectSpecHandle.Data);

    SetTimer(m_fInterval);
}

float UDiaTimerAbility::GetAttributeWantFrom()
{
    return 0.f;
}

void UDiaTimerAbility::SetTimer(const float TimeDelay)
{
    if (GetAvatarActorFromActorInfo() && GEngine->GetNetMode(GetWorld()) < NM_Client)
    {
        GetAvatarActorFromActorInfo()->GetWorldTimerManager().SetTimer(m_TimerHandle_OnTimer, this,
                                                                       &UDiaTimerAbility::OnTimer,
                                                                       m_fInterval,
                                                                       false);
    }

}

void UDiaTimerAbility::SetSensingInterval(const float newSensingInterval)
{
    if (m_fInterval != newSensingInterval)
    {
        m_fInterval = newSensingInterval;

        if (GetAvatarActorFromActorInfo())
        {
            if (m_fInterval <= 0.f)
            {
                SetTimer(0.f);
            }
            else
            {
                float CurrentElapsed = GetAvatarActorFromActorInfo()->GetWorldTimerManager().GetTimerElapsed(
                    m_TimerHandle_OnTimer);

                CurrentElapsed = FMath::Max(0.f, CurrentElapsed);

                if (CurrentElapsed < m_fInterval)
                {
                    SetTimer(m_fInterval - CurrentElapsed);
                }
                else if (CurrentElapsed > m_fInterval)
                {
                    SetTimer(KINDA_SMALL_NUMBER);
                }
            }
        }
    }
}


void UDiaTimerAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
    SetGameEffectSpec();
    
    SetSensingUpdatesEnabled(true);
}

void UDiaTimerAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                  const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo,
                                  bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    
    SetSensingUpdatesEnabled(false);

}

