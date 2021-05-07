#include "AnimNotify_TraceSphere.h"


#include "Components/SkeletalMeshComponent.h"
#include "Interface/SkillUseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"


UAnimNotify_TraceSphere::UAnimNotify_TraceSphere()
{
	m_fRange=600.f;
	m_fSphereRadius=700.f;
}

void UAnimNotify_TraceSphere::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Instigator = Cast<AActor>( MeshComp->GetOwner());

	if(!Instigator)
	{
		return;
	}
	
	FVector StartTrace = (Instigator->GetActorForwardVector() * m_fRange) +Instigator->GetActorLocation();
	
	TArray<FHitResult> Hits;

	if(!UKismetSystemLibrary::SphereTraceMultiForObjects(Instigator,StartTrace,StartTrace,m_fSphereRadius,
        m_ObjType,false,m_IgnoreActors,EDrawDebugTrace::None,Hits,true))
	{
		return;
	}


	ISkillUseCharacter* PL = Cast<ISkillUseCharacter>(Instigator);

	if(!PL)
	{
		return;
	}

	PL->TriggerSkill(m_ID,&Hits);

	PRINTF("GSphereTriggered");
}
