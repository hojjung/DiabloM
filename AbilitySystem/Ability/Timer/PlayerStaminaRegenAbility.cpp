// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStaminaRegenAbility.h"


#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Characters/PlayerDiabloCharacter.h"

UPlayerStaminaRegenAbility::UPlayerStaminaRegenAbility()
{
    FGameplayTag Ability1Tag = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.StaminaHeal"));
    AbilityTags.AddTag(Ability1Tag);
    ActivationOwnedTags.AddTag(Ability1Tag);
}

float UPlayerStaminaRegenAbility::GetAttributeWantFrom()
{
    return Cast<UPlayerDiabloAttribute>( Cast<APlayerDiabloCharacter>( m_OwnerUnit)->GetAttributeSet())->GetStaminaRegen()*m_fInterval;
}
