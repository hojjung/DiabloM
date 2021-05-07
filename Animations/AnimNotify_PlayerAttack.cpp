#include "AnimNotify_PlayerAttack.h"

#include "Characters/Pawns/UnitPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Interface/SkillUseCharacter.h"


void UAnimNotify_PlayerAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	if(!MeshComp->GetOwner())
	{
		return;
	}

	ISkillUseCharacter* Unit = Cast<ISkillUseCharacter>(MeshComp->GetOwner());

	if(!Unit)
	{
		return;
	}

	Unit->TriggerSkill(m_ID);
}
