// My First Hack n Slash


#include "AnimNotify_TraceBox.h"

#include "Characters/UnitPawn.h"

UAnimNotify_TraceBox::UAnimNotify_TraceBox()
{
	m_CachedAttackEvent=FGameplayTag::RequestGameplayTag("Combat.Ability.Skill.BaseAttack");//for spawn projectile
	m_fRange=600.f;
	m_BoxHalfSize = FVector(300.f,200.f,100.f);
}

void UAnimNotify_TraceBox::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Instigator = Cast<AActor>( MeshComp->GetOwner());
	
	FVector StartTrace = Instigator->GetActorLocation();
	
	FVector EndTrace = (Instigator->GetActorForwardVector() * m_fRange) + StartTrace;

	TArray<FHitResult> Hits;

	if(!UKismetSystemLibrary::BoxTraceMultiForObjects(Instigator,StartTrace,EndTrace,m_BoxHalfSize,Instigator->GetActorRotation(),
		m_ObjType,false,m_IgnoreActors,EDrawDebugTrace::ForOneFrame,Hits,true))
	{
		return;
	}

	AUnitPawn* Unit=Cast<AUnitPawn>( Instigator);
	
	if(!Unit)
	{
		return;
	}

	for(FHitResult& Hitten : Hits)
	{
		FGameplayEventData EventData;
		
		EventData.Instigator = Unit;
		
		EventData.Target = Hitten.GetActor();//?

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Unit,m_CachedAttackEvent,EventData);
	}
}
