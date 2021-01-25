#include "BarbarianLeapAttack.h"
#include "NavigationSystem.h"
#include "AbilitySystem/Task/PlayMontageAndWaitForEvent.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameMode.h"

UBarbarianLeapAttack::UBarbarianLeapAttack()
{
	m_fRadius=200.f;
	m_fHeight=500.f;
}

void UBarbarianLeapAttack::GetCubeControlPoint(FVector start, FVector end, float height,FVector& ctrlPoint1,FVector& ctrlPoint2)
{
	FVector point1 = start;

	point1.Z += height;

	ctrlPoint1 = point1;

	FVector point2 = end;

	point2.Z += height;

	ctrlPoint2 = point2;

}

void UBarbarianLeapAttack::PlayCurveJumpAction(FVector StartPoint, FVector Destination)
{
	UCBezierCurve* Curve = UCActionFactory::MakeCurve();
	
	FVector ControlPoint1;
	
	FVector ControlPoint2;
	
	GetCubeControlPoint(StartPoint, Destination, m_fHeight,ControlPoint1,ControlPoint2);
	
	Curve->InitializeCube(StartPoint, ControlPoint1,ControlPoint2, Destination);

	UCFollowBezierCurvePathAction* PathAction = UCActionFactory::MakeFollowBezierPathAction(m_PlayerPawn, Curve, m_fJumpDuration);

	PathAction->m_OnComplete.AddUObject(this, &UPlayerBaseAttack::OnCompletedWrapper);
	
	PathAction->m_OnKill.AddUObject(this, &UPlayerBaseAttack::OnCancelledWrapper);
	
	ADiabloGameMode::Get->GetPlayerActionManager()->AddAction(PathAction);
	
	m_CurrentID = PathAction;
}

void UBarbarianLeapAttack::TraceStomp()
{
	PRINTF("TaceStomp");
	//cam shake
	//effect
	//sound
	ADiabloPlayerController::Get->ClientPlayCameraShake(m_ClassCamShake);
	
	FVector PlayerPoint = m_PlayerPawn->GetMovementComponent()->GetActorFeetLocation();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),m_StompParticle,PlayerPoint);

	FRotator Rot;
	
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(),m_StompSound,PlayerPoint,Rot,3.5f);

	TArray<AActor*> OutActor;
		
	if(UKismetSystemLibrary::SphereOverlapActors(GetWorld(),PlayerPoint,m_fRadius,m_AryTraceType,AMonsterPawn::StaticClass(),m_AryIgnoreActor,OutActor))
	{
		for(AActor* hitActor : OutActor)
		{
			AMonsterPawn* TargetChar = Cast<AMonsterPawn>( hitActor);
			
			if (!m_PlayerPawn||!TargetChar||!TargetChar->IsAlive())
			{
				return;
			}

			if(DealDamageToTarget(TargetChar, m_PlayerPawn))
			{
			
			}
		}
	}
}

void UBarbarianLeapAttack::OnCompletedWrapper()
{
	TraceStomp();
	
	Super::OnCompletedWrapper();
}

void UBarbarianLeapAttack::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	m_AryIgnoreActor.Reset();
	m_AryIgnoreActor.Add(m_PlayerPawn);
}

void UBarbarianLeapAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	m_PlayerPawn->SetBlockMove();

	float TotalLength = m_BaseAttackMotion->GetPlayLength();

	float Rate = TotalLength /m_fJumpDuration;
	
	UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
       this, NAME_None, m_BaseAttackMotion, FGameplayTagContainer(), 1,
       NAME_None, false, 1.0f);
	
	Task->ReadyForActivation();

	ADiabloGameMode::Get->GetPlayerActionManager()->RemoveActionByID(m_CurrentID.Get());

	FVector StartPoint = m_PlayerPawn->GetMovementComponent()->GetActorFeetLocation();//for path find

	FVector Destination = ASkillIndicator::GetCurrent->GetActorLocation();

	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(StartPoint,Destination);

	m_PlayerPawn->SetActorRotation(Rot);
	
	PlayCurveJumpAction(StartPoint, Destination);
}

void UBarbarianLeapAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	m_PlayerPawn->SetUnblockMove();
}

