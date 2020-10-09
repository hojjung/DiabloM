// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SendGameEvent.generated.h"

UCLASS()
class DIABLOM_API UAnimNotify_SendGameEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_SendGameEvent();
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FGameplayTag m_CachedAttackEvent;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;
};
