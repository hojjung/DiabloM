// My First Hack n Slash


#include "PlayerRageRegenAbility.h"

#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Characters/PlayerDiabloCharacter.h"

UPlayerRageRegenAbility::UPlayerRageRegenAbility()
{
	FGameplayTag Ability1Tag = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.RageHeal"));
	AbilityTags.AddTag(Ability1Tag);
	ActivationOwnedTags.AddTag(Ability1Tag);
}

float UPlayerRageRegenAbility::GetAttributeWantFrom()
{
	float RR = Cast<UPlayerDiabloAttribute>( Cast<APlayerDiabloCharacter>( m_OwnerUnit)->GetAttributeSet())->GetRageRegen();

	
	return RR*m_fInterval;
}
