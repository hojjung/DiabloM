#include "AnimNotify_TraceSphere.h"

#include "Characters/UnitPawn.h"

UAnimNotify_TraceSphere::UAnimNotify_TraceSphere()
{
	m_CachedAttackEvent=FGameplayTag::RequestGameplayTag("Combat.Ability.Skill.BaseAttack");//for spawn projectile
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
        m_ObjType,false,m_IgnoreActors,EDrawDebugTrace::ForOneFrame,Hits,true))
	{
		return;
	}

	AUnitPawn* Unit=Cast<AUnitPawn>(Instigator);
	
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
