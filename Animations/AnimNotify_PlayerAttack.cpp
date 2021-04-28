#include "AnimNotify_PlayerAttack.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Components/SkeletalMeshComponent.h"


void UAnimNotify_PlayerAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	if(!MeshComp->GetOwner())
	{
		return;
	}

	AUnitPawn* Unit = Cast<AUnitPawn>(MeshComp->GetOwner());

	if(!Unit)
	{
		return;
	}

	Unit->TriggerSkill(m_ID);
}
