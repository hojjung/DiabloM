// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerManaRegenAbility.h"


#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Characters/PlayerDiabloCharacter.h"

UPlayerManaRegenAbility::UPlayerManaRegenAbility()
{
    FGameplayTag Ability1Tag = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.MpHeal"));
    AbilityTags.AddTag(Ability1Tag);
    ActivationOwnedTags.AddTag(Ability1Tag);
}

float UPlayerManaRegenAbility::GetAttributeWantFrom()
{
    return Cast<UPlayerDiabloAttribute>( Cast<APlayerDiabloCharacter>( m_OwnerUnit)->GetAttributeSet())->GetManaRegen()*m_fInterval;
}
