#include "UnitMovement.h"
#include "Characters/UnitPawn.h"
#include "AI/Navigation/AvoidanceManager.h"
#include "DrawDebugHelpers.h"


UUnitMovement::UUnitMovement()
{
    m_fMoveSpeedRatio = 1.0f;
    m_bUseRVO = false;
    m_AvoidanceUID = 0;
    m_AvoidanceLockVelocity = FVector::ZeroVector;
    m_AvoidanceLockTimer = 0.0f;
    m_AvoidanceGroup.bGroup0 = true;
    m_GroupsToAvoid.Packed = 0xFFFFFFFF;
    m_GroupsToIgnore.Packed = 0;
    m_AvoidConsiderRadius = 500.f;
    m_AvoidanceWeight = 0.4f;
    m_fMaxSpeed = 300.f;
    m_fDashDuration = 0.f;
    bUseAccelerationForPaths=true;
    m_AvoidanceWeight=0.3f;
    m_RotateSpeed=FRotator(0,240.f,0);
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
    //if (AvoidanceManager)
    {
        AvoidanceManager->RegisterMovementComponent(this, m_AvoidanceWeight);
    }
}

void UUnitMovement::CalcVelocity(float DeltaTime)
{
    Velocity = ConsumeInputVector().GetClampedToSize(1.0f, 1.0f) * m_fMaxSpeed * m_fMoveSpeedRatio;

    if (m_fDashDuration > 0.f)
    {
        m_fDashDuration -= DeltaTime;
        Velocity += m_DashDelta;
    }

    if (m_bUseRVO)
    {
        CalcAvoidanceVelocity(DeltaTime);
    }
}

void UUnitMovement::MoveProceed(float DeltaTime)
{
    if (!Velocity.IsNearlyZero())
    {
        FVector MoveDelta = Velocity;
        
        MoveDelta*=DeltaTime;
        
        FHitResult Hit;
        
        SafeMoveUpdatedComponent(MoveDelta, UpdatedComponent->GetComponentRotation(), true, Hit);

        if (Hit.IsValidBlockingHit())
        {
            SlideAlongSurface(MoveDelta, 1.f - Hit.Time, Hit.Normal, Hit);
        }
    }
}

void UUnitMovement::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                  FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
    {
        return;
    }

    CalcVelocity(DeltaTime);
   
    TickRotate(DeltaTime);

    MoveProceed(DeltaTime);

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

        MoveUpdatedComponent(FVector::ZeroVector, DesiredRotation, false);
    }
}


void UUnitMovement::UpdateDefaultRVO()
{
    if (!m_bUseRVO)
    {
        return;
    }
    UAvoidanceManager* AvoidanceManager = GetWorld()->GetAvoidanceManager();

    if (AvoidanceManager && !m_bWasAvoidanceUpdated)
    {
        AvoidanceManager->UpdateRVO(this);

        SetAvoidanceVelocityLock(AvoidanceManager, AvoidanceManager->LockTimeAfterClean);
    }

    m_bWasAvoidanceUpdated = false; //Reset for next frame
}

void UUnitMovement::SetAvoidanceVelocityLock(UAvoidanceManager* Avoidance, float Duration)
{
    Avoidance->OverrideToMaxWeight(m_AvoidanceUID, Duration);


    m_AvoidanceLockVelocity = Velocity;
    m_AvoidanceLockTimer = Duration;
}

void UUnitMovement::CalcAvoidanceVelocity(float DeltaTime)
{
    m_AvoidanceLockTimer -= DeltaTime;

    UAvoidanceManager* AvoidanceManager = GetWorld()->GetAvoidanceManager();

    if (m_AvoidanceWeight >= 1.0f || !AvoidanceManager || GetOwner()->GetLocalRole() != ROLE_Authority)
    {
        return;
    }

    UCapsuleComponent* OurCapsule = Cast<AUnitPawn>(GetOwner())->GetCapsule();

    if (!Velocity.IsZero() && OurCapsule)
    {
        if (m_AvoidanceLockTimer > 0.0f)
        {
            Velocity = m_AvoidanceLockVelocity;
          
        }
        else
        {
            FVector NewVelocity = AvoidanceManager->GetAvoidanceVelocityForComponent(this);

            if (!NewVelocity.Equals(Velocity)) //Really want to branch hint that this will probably not pass
            {
                Velocity = NewVelocity;
                SetAvoidanceVelocityLock(AvoidanceManager, AvoidanceManager->LockTimeAfterAvoid);
            }
            else
            {
                SetAvoidanceVelocityLock(AvoidanceManager, AvoidanceManager->LockTimeAfterClean);
            }
        }
        AvoidanceManager->UpdateRVO(this);

        m_bWasAvoidanceUpdated = true;
    }
}


void UUnitMovement::NotifyBumpedPawn(APawn* BumpedPawn)
{
    Super::NotifyBumpedPawn(BumpedPawn);

    m_AvoidanceLockTimer = 0.0f;
}

void UUnitMovement::StopActiveMovement()
{
    Super::StopActiveMovement();
    Velocity = FVector::ZeroVector;
    m_DashDelta= FVector::ZeroVector;
    m_fDashDuration =-1.f;
}

FRotator UUnitMovement::ComputeOrientToMovementRotation(const FRotator& CurrentRotation) const
{
    if (Velocity.IsNearlyZero(0.01f))//회전각이 0이여서 // 몬스터의 경우 추적 대상이 존재한다면 추적대상을 바라봐야함,이함수랑 별개로 만들어야할듯? ㄴㄴ 그냥 움직일때는 고개돌리는게 맞을듯
    {
        return  GetRotationNotMove(CurrentRotation);
    }

    return Velocity.GetSafeNormal().Rotation();
}

void UUnitMovement::SetMoveSpeed(float newSpeed)
{
    m_fMaxSpeed = newSpeed;
}

void UUnitMovement::SetMoveSpeedRatio(float newRatioMax1)
{
    m_fMoveSpeedRatio = newRatioMax1;
}

void UUnitMovement::SetDash(FVector dashDelta, float duration)
{
    m_fDashDuration = duration;
    m_DashDelta = dashDelta;
}

FRotator UUnitMovement::GetRotationNotMove(const FRotator& rot) const
{
    return rot;
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
    return Velocity;
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
