// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "AbilitySystem/Ability/DiabloAbility.h"
#include "AbilitySystem/Ability/AbilitySkillActors/AbilityProjectile.h"
#include "Characters/MonsterPawn.h"
#include "Characters/UnitMovement.h"
#include "MonsterBaseRangeAttack.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UMonsterBaseRangeAttack : public UDiabloAbility
{
	GENERATED_BODY()
	
public:
	UMonsterBaseRangeAttack();
	
protected:
	FGameplayTag m_TagTookPhysDamage;
	FGameplayTag m_TagTookFireDamage;
	FGameplayTag m_TagTookIceDamage;
	FGameplayTag m_TagTookElecDamage;
	FGameplayTag m_TagTookPoisonDamage;
	FGameplayTag m_TagEventEndAbility;
	FGameplayTag m_TagEventBaseAttack;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AAbilityProjectile> m_ClassBullet;
	
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_fAttackRange;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_fAttackAngle;

	float m_fAttackRangeSqr;
	float m_fAttackAngleCos;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* m_BaseAttackMotion;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY()
	AMonsterPawn* m_MonsterPawn;
	
	UPROPERTY()
	TArray<AAbilityProjectile*> m_AryMissle;

	int m_nBulletIndexForPull;

protected:
	void PlayAbilityAnimation(UAnimMontage* MontageToPlay, FName playSection,float AttackSpeed);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	void CreateBulletPool(const FGameplayAbilityActorInfo* ActorInfo);

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	bool CheckAttackRange(const AActor* other) const;
	
	UFUNCTION()
    void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
    void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
    void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	AAbilityProjectile* GetBullet();
};
