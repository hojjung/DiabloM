#include "SkillTrigger.h"

#include "Characters/PlayerDiabloCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void USkillTrigger::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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

	PL->TriggerSkill(m_SkillID);
}
