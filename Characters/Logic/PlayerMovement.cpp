#include "PlayerMovement.h"
#include "NavigationSystem.h"
#include "Managers/DiabloGameInstance.h"

void UPlayerMovement::BeginPlay()
{
	Super::BeginPlay();
	m_NavSys = UDiabloGameInstance::Get->GetNavSys();
}

void UPlayerMovement::MoveProceed(float DeltaTime)
{
	if (UDiabloGameInstance::Get->m_PlayerUpgradeManager->IsRootmotionSkillCasting())
	{
		Velocity = FVector::ZeroVector;
		return;
	}

	if (!Velocity.IsNearlyZero())
	{
		//Velocity.Z = 0.f;

		FVector MoveDelta = Velocity;

		MoveDelta *= DeltaTime;

		FVector NewMove = GetActorFeetLocation() + MoveDelta;

		FNavLocation Loc;

		if (!m_NavSys->ProjectPointToNavigation(NewMove, Loc))
		{
			return;
		}

		FHitResult Hit;

		SafeMoveUpdatedComponent(MoveDelta, UpdatedComponent->GetComponentRotation(), true, Hit);

		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(MoveDelta, 1.f - Hit.Time, Hit.Normal, Hit);
		}
		FVector NewLoc = GetActorLocation();

		NewLoc.Z = GetZAxis();

		PawnOwner->SetActorLocation(NewLoc, true, &Hit);
	}
}
