#include "AnimNotify_TraceBox.h"

#include "Characters/Pawns/UnitPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Interface/SkillUseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

UAnimNotify_TraceBox::UAnimNotify_TraceBox()
{
	m_fRange=600.f;
	m_BoxHalfSize = FVector(300.f,200.f,100.f);
}

void UAnimNotify_TraceBox::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Instigator = Cast<AActor>( MeshComp->GetOwner());
	
	FVector StartTrace = Instigator->GetActorLocation() +(Instigator->GetActorForwardVector() * m_fRange);
	
	//FVector EndTrace = (Instigator->GetActorForwardVector() * m_fRange) + StartTrace;

	TArray<FHitResult> Hits;

	if(!UKismetSystemLibrary::BoxTraceMultiForObjects(Instigator,StartTrace,StartTrace,m_BoxHalfSize,Instigator->GetActorRotation(),
		m_ObjType,false,m_IgnoreActors,EDrawDebugTrace::ForDuration,Hits,true))
	{
		return;
	}

	PRINTF("HitCount:%d",Hits.Num());

	ISkillUseCharacter* PL = Cast<ISkillUseCharacter>(Instigator);

	if(!PL)
	{
		return;
	}

	PL->TriggerSkill(m_ID,&Hits);

}
