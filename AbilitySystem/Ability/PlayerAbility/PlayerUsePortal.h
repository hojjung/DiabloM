#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerAbility/PlayerDiabloAbility.h"
#include "PlayerUsePortal.generated.h"

class UPlayMontageAndWaitForEvent;

UCLASS()
class DIABLOM_API UPlayerUsePortal : public UPlayerDiabloAbility
{
    GENERATED_BODY()

public:
    UPlayerUsePortal();

protected:
    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    FGameplayTag m_TagEventEndAbility;
    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    FGameplayTag m_TagEventReceive;
    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    UAnimMontage* m_PotionDrinkMotion;
    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    USoundBase* m_PortalSound;

    FDelegateHandle m_DmgTookDelegate;

    UPROPERTY()
    UAudioComponent* m_Audio;

    UPROPERTY()
    UPlayMontageAndWaitForEvent* m_Task;
protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData* TriggerEventData) override;

    UFUNCTION()
    void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

    UFUNCTION()
    void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

    UFUNCTION()
    void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

    void PlayAbilityAnimation(UAnimMontage* MontageToPlay, FName playSection, float AttackSpeed);

    void OnDamageTook(float damage);

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
};


