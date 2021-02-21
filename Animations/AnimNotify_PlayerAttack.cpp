#include "AnimNotify_PlayerAttack.h"


#include "Characters/PlayerDiabloCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_PlayerAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	if(!MeshComp->GetOwner())
	{
		return;
	}

	APlayerDiabloCharacter* PL = Cast<APlayerDiabloCharacter>(MeshComp->GetOwner());

	if(!PL)
	{
		return;
	}

	PL->ApplyDamageToTarget();
}
