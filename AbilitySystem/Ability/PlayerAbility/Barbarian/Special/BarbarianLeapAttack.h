// My First Hack n Slash

#pragma once

#include "CActionBaseInterface.h"
#include "DiabloM.h"
#include "AbilitySystem/Ability/PlayerAbility/BaseSkillTemplate/PlayerBaseAttack.h"
#include "Objs/Actor/SkillIndicator.h"

#include "BarbarianLeapAttack.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UBarbarianLeapAttack : public UPlayerBaseAttack
{
	GENERATED_BODY()

public:
	UBarbarianLeapAttack();
	
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UCameraShake> m_ClassCamShake;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	USoundBase* m_StompSound;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UParticleSystem* m_StompParticle;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> m_AryTraceType;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_fRadius;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_fHeight;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_fJumpDuration;
	UPROPERTY()
	TArray<AActor*> m_AryIgnoreActor;

	TWeakObjectPtr<UCActionBaseInterface> m_CurrentID;

	
	//조이스틱 지정
	//최대 사거리까지
	//인디케이터 생성
	//인디케이터의 기본 지름 정해놓고 스케일로 조정
	//인디케이터의 위치는 네비게이션 가능 지역이여야함

	//손을 놓으면 캐릭터가 포물선으로 튀어 오름
protected:
	void GetCubeControlPoint(FVector start,FVector end,float height,FVector& ctrlPoint1,FVector& ctrlPoint2);
	
	void PlayCurveJumpAction(FVector StartPoint, FVector Destination);

	void TraceStomp();

protected:
	virtual void OnCompletedWrapper() override;
	
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	//
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
