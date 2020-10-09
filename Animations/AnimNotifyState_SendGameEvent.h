// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_SendGameEvent.generated.h"

UCLASS()
class DIABLOM_API UAnimNotifyState_SendGameEvent : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotifyState_SendGameEvent();
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FGameplayTag m_CachedAttackEvent;
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)override;
};
