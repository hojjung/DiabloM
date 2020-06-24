#include "BaseStatEffect.h"
#include "AbilitySystem/Attribute/BaseDiabloAttribute.h"


UBaseStatEffect::UBaseStatEffect()
{
	FGameplayModifierInfo MaxHealth;
	MaxHealth.Attribute = UBaseDiabloAttribute::GetMaxHealthAttribute();
	MaxHealth.ModifierMagnitude = FGameplayEffectModifierMagnitude();
}