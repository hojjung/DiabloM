// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/Components/DiabloAbilitySystemComp.h"

#include "DiabloAbility.generated.h"

/**
 * 
 */
class AUnitPawn;
UCLASS()
class DIABLOM_API UDiabloAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UDiabloAbility();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	FText m_ShowingName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	UTexture2D* m_AbilityIcon;
	
protected:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool m_bActivateAbilityOnGranted;

	UPROPERTY()
	AUnitPawn* m_OwnerUnit;
protected:

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};