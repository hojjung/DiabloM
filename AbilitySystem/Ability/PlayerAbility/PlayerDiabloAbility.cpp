#include "PlayerDiabloAbility.h"
#include "Characters/UnitPawn.h"
#include "Characters/PlayerDiabloCharacter.h"

void UPlayerDiabloAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    UDiabloAbility::OnAvatarSet(ActorInfo,Spec);
    m_PlayerPawn=Cast<APlayerDiabloCharacter>( m_OwnerUnit);
}
