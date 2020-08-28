#include "UnitMovement.h"
#include "Characters/UnitPawn.h"
#include "AI/Navigation/AvoidanceManager.h"
#include "DrawDebugHelpers.h"


UUnitMovement::UUnitMovement()
{
	m_bUseRVO = false;
	m_AvoidanceUID = 0;
	m_AvoidanceLockVelocity = FVector::ZeroVector;
	m_AvoidanceLockTimer = 0.0f;
	m_AvoidanceGroup.bGroup0 = true;
	m_GroupsToAvoid.Packed = 0xFFFFFFFF;
	m_GroupsToIgnore.Packed = 0;
	m_AvoidConsiderRadius = 500.f;
	m_AvoidanceWeight = 0.f;
	m_fMaxSpeed = 300.f;
}

void UUnitMovement::BeginPlay()
{
	Super::BeginPlay();


	Cast<AUnitPawn>(GetOwner())->GetCapsuleSize(m_CapsuleHeight, m_CapsuleRadius);

	if (!m_bUseRVO)
	{
		return;
	}
	UAvoidanceManager* AvoidanceManager = GetWorld()->GetAvoidanceManager();
	if (AvoidanceManager)
	{
		AvoidanceManager->RegisterMovementComponent(this, m_AvoidanceWeight);
	}
}

void UUnitMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	Velocity = ConsumeInputVector().GetClampedToSize(1.0f,1.0f) *  m_fMaxSpeed;

	m_MoveVector = Velocity*DeltaTime;
	
	float PastMoveSize = m_MoveVector.Size();

	TickRotate(DeltaTime);

	CalcAvoidanceVelocity(DeltaTime);
	

	m_MoveVector = m_MoveVector.GetClampedToMaxSize(PastMoveSize);

	if (!m_MoveVector.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(m_MoveVector, UpdatedComponent->GetComponentRotation(), true, Hit);

		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(m_MoveVector, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}


	UpdateDefaultRVO();
};

void UUnitMovement::TickRotate(float deltaTime)
{
	FRotator CurrentRotation = UpdatedComponent->GetComponentRotation(); // Normalized
	FRotator DeltaRot = m_RotateSpeed * deltaTime;
	FRotator DesiredRotation = CurrentRotation;

	DesiredRotation = ComputeOrientToMovementRotation(CurrentRotation);
	DesiredRotation.Pitch = 0.f;
	DesiredRotation.Yaw = FRotator::NormalizeAxis(DesiredRotation.Yaw);
	DesiredRotation.Roll = 0.f;

	if (!CurrentRotation.Equals(DesiredRotation, 1e-3f))
	{
		if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredRotation.Yaw, 1e-3f))
		{
			DesiredRotation.Yaw = FMath::FixedTurn(CurrentRotation.Yaw, DesiredRotation.Yaw, DeltaRot.Yaw);
		}

		MoveUpdatedComponent(FVector::ZeroVector, DesiredRotation, /*bSweep*/ false);
	}
}


void UUnitMovement::UpdateDefaultRVO()
{
	UAvoidanceManager* AvoidanceManager = GetWorld()->GetAvoidanceManager();

	if (AvoidanceManager && !m_bWasAvoidanceUpdated)
	{
		AvoidanceManager->UpdateRVO(this);

		SetAvoidanceVelocityLock(AvoidanceManager, AvoidanceManager->LockTimeAfterClean);
	}

	m_bWasAvoidanceUpdated = false;		//Reset for next frame
}

void UUnitMovement::SetAvoidanceVelocityLock(UAvoidanceManager * Avoidance, float Duration)
{
	Avoidance->OverrideToMaxWeight(m_AvoidanceUID, Duration);


	m_AvoidanceLockVelocity = m_MoveVector;
	m_AvoidanceLockTimer = Duration;

}

void UUnitMovement::CalcAvoidanceVelocity(float DeltaTime)
{
	if (!m_bUseRVO)
	{
		return;
	}
	m_AvoidanceLockTimer -= DeltaTime;


	UAvoidanceManager* AvoidanceManager = GetWorld()->GetAvoidanceManager();

	if (m_AvoidanceWeight >= 1.0f || !AvoidanceManager ||GetOwner()->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	UCapsuleComponent *OurCapsule = Cast<AUnitPawn>( GetOwner())->GetCapsule();

	if (!m_MoveVector.IsZero()&& OurCapsule)
	{
		if (m_AvoidanceLockTimer > 0.0f)
		{
			m_MoveVector = m_AvoidanceLockVelocity;
		}
		else
		{
			FVector NewVelocity = AvoidanceManager->GetAvoidanceVelocityForComponent(this);
			
			if (!NewVelocity.Equals(m_MoveVector))		//Really want to branch hint that this will probably not pass
			{
				m_MoveVector = NewVelocity;
				SetAvoidanceVelocityLock(AvoidanceManager, AvoidanceManager->LockTimeAfterAvoid);
			}
			else
			{
				SetAvoidanceVelocityLock(AvoidanceManager, AvoidanceManager->LockTimeAfterClean);	//10 ms of lock time should be adequate.
			}
		}
		AvoidanceManager->UpdateRVO(this);

		m_bWasAvoidanceUpdated = true;
	}
}


void UUnitMovement::NotifyBumpedPawn(APawn * BumpedPawn)
{
	Super::NotifyBumpedPawn(BumpedPawn);

	m_AvoidanceLockTimer = 0.0f;
}

void UUnitMovement::StopActiveMovement()
{
	Super::StopActiveMovement();
	m_MoveVector = FVector::ZeroVector;
	Velocity = m_MoveVector;
}

FRotator UUnitMovement::ComputeOrientToMovementRotation(const FRotator & CurrentRotation) const
{
	if (m_MoveVector.IsNearlyZero(0.01f))
	{
		return CurrentRotation;
	}

	return m_MoveVector.GetSafeNormal().Rotation();
}

void UUnitMovement::SetMoveSpeed(float newSpeed)
{
	m_fMaxSpeed=newSpeed;
}

#pragma region RVO_GETSET

void UUnitMovement::SetRVOAvoidanceUID(int32 UID)
{
	m_AvoidanceUID = UID;
}

int32 UUnitMovement::GetRVOAvoidanceUID()
{
	return m_AvoidanceUID;
}

void UUnitMovement::SetRVOAvoidanceWeight(float Weight)
{
	m_AvoidanceWeight = Weight;
}

float UUnitMovement::GetRVOAvoidanceWeight()
{
	return m_AvoidanceWeight;
}

FVector UUnitMovement::GetRVOAvoidanceOrigin()
{
	return UpdatedComponent->GetComponentLocation() - FVector(0, 0, UpdatedComponent->Bounds.BoxExtent.Z);
}

float UUnitMovement::GetRVOAvoidanceRadius()
{
	return m_CapsuleRadius;
}

float UUnitMovement::GetRVOAvoidanceHeight()
{
	return m_CapsuleHeight;
}

float UUnitMovement::GetRVOAvoidanceConsiderationRadius()
{
	return m_AvoidConsiderRadius;
}

FVector UUnitMovement::GetVelocityForRVOConsideration()
{
	return m_MoveVector;
}

int32 UUnitMovement::GetAvoidanceGroupMask()
{
	return m_AvoidanceGroup.Packed;
}

int32 UUnitMovement::GetGroupsToAvoidMask()
{
	return m_GroupsToAvoid.Packed;
}

int32 UUnitMovement::GetGroupsToIgnoreMask()
{
	return m_GroupsToIgnore.Packed;
}



#pragma endregion
