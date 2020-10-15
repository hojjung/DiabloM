// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHpRegenAbility.h"

#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Characters/PlayerDiabloCharacter.h"


UPlayerHpRegenAbility::UPlayerHpRegenAbility()
{
    FGameplayTag Ability1Tag = FGameplayTag::RequestGameplayTag(FName("Item.Option.Attribute.Health+"));
    AbilityTags.AddTag(Ability1Tag);
    ActivationOwnedTags.AddTag(Ability1Tag);
}

void UPlayerHpRegenAbility::OnTimer()
{
    Super::OnTimer();
}

float UPlayerHpRegenAbility::GetAttributeWantFrom()
{
    return Cast<UPlayerDiabloAttribute>( Cast<APlayerDiabloCharacter>( m_OwnerUnit)->GetAttributeSet())->GetHealthRegen()*m_fInterval;
}

//Blueprint'/Game/Blueprints/Abilities/GameEffect/GE_PlayerRegen.GE_PlayerRegen'