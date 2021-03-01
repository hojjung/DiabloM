#include "UnitPawn.h"
#include "NavigationPath.h"
#include "NavigationData.h"
#include "NavigationSystem.h"
#include "Managers/DiabloGameInstance.h"
#include "Animations/MobAnimInstance.h"

AUnitPawn::AUnitPawn(const FObjectInitializer& objInit): Super(objInit)
{
    //
    m_fAttackSpeed = 1.f;
    m_fAttackCDConstant = 0.5f;
    m_fAttackRange=450.f;
    PrimaryActorTick.bCanEverTick = true;
    m_bUseFSM = false;
    m_Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule00");
    m_Capsule->InitCapsuleSize(34.0f, 88.0f);
    m_Capsule->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    m_Capsule->CanCharacterStepUpOn = ECB_No;
    m_Capsule->SetShouldUpdatePhysicsVolume(true);
    m_Capsule->SetCanEverAffectNavigation(false);
    m_Capsule->bDynamicObstacle = false;
    m_Capsule->AreaClass = nullptr; 
    RootComponent = m_Capsule;

    m_Movement = CreateDefaultSubobject<UUnitMovement>("Movement00");
    m_Movement->UpdatedComponent = m_Capsule;

    CreateSkMeshComponent(RootComponent, &m_SkBody, "SkMesh00");
    m_SkBody->bCastDynamicShadow = true;

    m_PFComp = CreateDefaultSubobject<UPathFollowingComponent>(TEXT("PathFollowingComponent"));
    m_PFComp->SetMovementComponent(m_Movement);

    m_NavSys = nullptr;

    m_fMoveAcceptRadius = 100.f;
   
    m_fHitAnimCD=-1.f;

    //GetMovementComponent()->NavAgentProps.
}


void AUnitPawn::CreateSkMeshComponent(USceneComponent* rootWant, USkeletalMeshComponent** refSkComp, FName keyName)
{
    (*refSkComp) = CreateDefaultSubobject<USkeletalMeshComponent>(keyName);
    (*refSkComp)->bOwnerNoSee = false;
    (*refSkComp)->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
    //(*refSkComp)->bCastDynamicShadow = true;//chanage for mobile
    (*refSkComp)->bAffectDynamicIndirectLighting = true;
    (*refSkComp)->PrimaryComponentTick.TickGroup = TG_PrePhysics;
    (*refSkComp)->SetupAttachment(rootWant);
    (*refSkComp)->SetCollisionProfileName("NoCollision");
    (*refSkComp)->SetGenerateOverlapEvents(false);
    (*refSkComp)->SetCanEverAffectNavigation(false);

    (*refSkComp)->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

    (*refSkComp)->bReceiveMobileCSMShadows = false;

    (*refSkComp)->bEnableUpdateRateOptimizations=true;

    (*refSkComp)->bComponentUseFixedSkelBounds=true;
}

// Called when the game starts or when spawned
void AUnitPawn::BeginPlay()
{
    Super::BeginPlay();
    m_NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

    
}

FPathFollowingRequestResult AUnitPawn::MoveToLocation(FVector goalLocation,float additionalAcceptRadius)
{
    if (m_PFComp && m_PFComp->GetStatus() != EPathFollowingStatus::Idle)
    {
        m_PFComp->AbortMove(*this, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
                            , FAIRequestID::CurrentRequest, EPathFollowingVelocityMode::Keep);
    }

    FAIMoveRequest MoveReq(goalLocation);
    MoveReq.SetUsePathfinding(true);
    MoveReq.SetAllowPartialPath(true);
    MoveReq.SetProjectGoalLocation(false);
    MoveReq.SetNavigationFilter(UNavigationQueryFilter::StaticClass());
    MoveReq.SetAcceptanceRadius(GetAcceptRadiusSelfOnly()+10.f+additionalAcceptRadius);
    MoveReq.SetReachTestIncludesAgentRadius(true);
    MoveReq.SetCanStrafe(true);

    return MoveTo(MoveReq);
}

FPathFollowingRequestResult AUnitPawn::MoveToActor(AActor* goalTarget,float additionalAcceptRadius)
{
    if (m_PFComp && m_PFComp->GetStatus() != EPathFollowingStatus::Idle)
    {
        m_PFComp->AbortMove(*this, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
                            , FAIRequestID::CurrentRequest, EPathFollowingVelocityMode::Keep);
    }

    FAIMoveRequest MoveReq(goalTarget);
    MoveReq.SetUsePathfinding(true);
    MoveReq.SetAllowPartialPath(true);
    MoveReq.SetNavigationFilter(UNavigationQueryFilter::StaticClass());
    MoveReq.SetAcceptanceRadius(GetAcceptRadiusToOther()+additionalAcceptRadius);
    MoveReq.SetReachTestIncludesAgentRadius(true);
    MoveReq.SetCanStrafe(true);

    return MoveTo(MoveReq);
}


FPathFollowingRequestResult AUnitPawn::MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath)
{
    FPathFollowingRequestResult ResultData;
    ResultData.Code = EPathFollowingRequestResult::Failed;

    if (MoveRequest.IsValid() == false)
    {
        PRINTF("InvalidRequest");
        return ResultData;
    }
    bool bCanRequestMove = true;
    bool bAlreadyAtGoal = false;

    if (!MoveRequest.IsMoveToActorRequest())
    {
        if (MoveRequest.GetGoalLocation().ContainsNaN() || FAISystem::IsValidLocation(MoveRequest.GetGoalLocation()) ==
            false)
        {
            bCanRequestMove = false;
        }

        if (bCanRequestMove && MoveRequest.IsProjectingGoal())
        {
            UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
            const FNavAgentProperties& AgentProps = GetNavAgentPropertiesRef();
            FNavLocation ProjectedLocation;

            if (NavSys && !NavSys->ProjectPointToNavigation(MoveRequest.GetGoalLocation(), ProjectedLocation,
                                                            INVALID_NAVEXTENT, &AgentProps))
            {
                bCanRequestMove = false;
            }

            MoveRequest.UpdateGoalLocation(ProjectedLocation.Location);
        }

        bAlreadyAtGoal = bCanRequestMove && m_PFComp->HasReached(MoveRequest);
    }
    else
    {
        bAlreadyAtGoal = bCanRequestMove && m_PFComp->HasReached(MoveRequest);
    }

    if (bAlreadyAtGoal)
    {
        ResultData.MoveId = m_PFComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Success);
        ResultData.Code = EPathFollowingRequestResult::AlreadyAtGoal;
    }
    else if (bCanRequestMove)
    {
        FPathFindingQuery PFQuery;

        const bool bValidQuery = BuildPathfindingQuery(MoveRequest, PFQuery);
        if (bValidQuery)
        {
            FNavPathSharedPtr Path;
            FindPathForMoveRequest(MoveRequest, PFQuery, Path);

            const FAIRequestID RequestID = Path.IsValid()
                                               ? RequestMove(MoveRequest, Path)
                                               : FAIRequestID::InvalidRequest;
            if (RequestID.IsValid())
            {
                bool bAllowStrafe = MoveRequest.CanStrafe();
                ResultData.MoveId = RequestID;
                ResultData.Code = EPathFollowingRequestResult::RequestSuccessful;

                if (OutPath)
                {
                    *OutPath = Path;
                }
            }
        }
    }

    if (ResultData.Code == EPathFollowingRequestResult::Failed)
    {
        ResultData.MoveId = m_PFComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Invalid);
    }

    return ResultData;
}

void AUnitPawn::FindPathForMoveRequest(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query,
                                       FNavPathSharedPtr& OutPath) const
{
    if (m_NavSys)
    {
        FPathFindingResult PathResult = m_NavSys->FindPathSync(Query);
        if (PathResult.Result != ENavigationQueryResult::Error)
        {
            if (PathResult.IsSuccessful() && PathResult.Path.IsValid())
            {
                if (MoveRequest.IsMoveToActorRequest())
                {
                    PathResult.Path->SetGoalActorObservation(*MoveRequest.GetGoalActor(), 100.0f);
                }

                PathResult.Path->EnableRecalculationOnInvalidation(true);
                OutPath = PathResult.Path;
            }
        }
    }
}

bool AUnitPawn::BuildPathfindingQuery(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query) const
{
    bool bResult = false;

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    const ANavigationData* NavData = (NavSys == nullptr)
                                         ? nullptr
                                         : MoveRequest.IsUsingPathfinding()
                                         ? NavSys->GetNavDataForProps(GetNavAgentPropertiesRef())
                                         : NavSys->GetAbstractNavData();

    if (NavData)
    {
        FVector GoalLocation = MoveRequest.GetGoalLocation();
        if (MoveRequest.IsMoveToActorRequest())
        {
            const INavAgentInterface* NavGoal = Cast<const INavAgentInterface>(MoveRequest.GetGoalActor());
            if (NavGoal)
            {
                const FVector Offset = NavGoal->GetMoveGoalOffset(this);
                GoalLocation = FQuatRotationTranslationMatrix(MoveRequest.GetGoalActor()->GetActorQuat(),
                                                              NavGoal->GetNavAgentLocation()).TransformPosition(Offset);
            }
            else
            {
                GoalLocation = MoveRequest.GetGoalActor()->GetActorLocation();
            }
        }

        FSharedConstNavQueryFilter NavFilter = UNavigationQueryFilter::GetQueryFilter(
            *NavData, this, MoveRequest.GetNavigationFilter());
        Query = FPathFindingQuery(*this, *NavData, GetNavAgentLocation(), GoalLocation, NavFilter);
        Query.SetAllowPartialPaths(MoveRequest.IsUsingPartialPaths());

        if (m_PFComp)
        {
            m_PFComp->OnPathfindingQuery(Query);
        }

        bResult = true;
    }

    return bResult;
}

FAIRequestID AUnitPawn::RequestMove(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr Path)
{
    uint32 RequestID = FAIRequestID::InvalidRequest;
    RequestID = m_PFComp->RequestMove(MoveRequest, Path);
    return RequestID;
}

void AUnitPawn::OnDeathAnimEnd()
{
    Destroy();
}

void AUnitPawn::UpdateMoveSpeed() const
{
    //Cast<UUnitMovement>(GetMovementComponent())->SetMoveSpeed(GetMoveSpeed());
}

float AUnitPawn::GetAcceptRadiusToOther()
{
    float MyCapsule =GetCapsule()->GetScaledCapsuleRadius();

    return GetFocusedTarget()? MyCapsule+GetFocusedTarget()->GetCapsule()->GetScaledCapsuleRadius():MyCapsule;
}

float AUnitPawn::GetAcceptRadiusSelfOnly()
{
    return GetCapsule()->GetScaledCapsuleRadius();;
}

bool AUnitPawn::CanSeeTarget()
{
    if(!GetFocusedTarget())
    {
        return false;
    }

    if(!GetController())
    {
        PRINTF("UNIT-NoCon");
        return  false;
    }

    return GetController()->LineOfSightTo(GetFocusedTarget());
}

float AUnitPawn::TryAttack()
{
    if(m_BaseAttackAnim&&m_fAttackCD<0.f)
    {
        float AnimMongLen = PlayAnimMontage(m_BaseAttackAnim,1*m_fAttackSpeed,NAME_None);

        m_fAttackCD =m_fAttackCDConstant;

        return AnimMongLen;;
    }

    return 0.f;
}


void AUnitPawn::TakeDmg(BigInt amount, AUnitPawn* attacker)
{
    
}

void AUnitPawn::GetHP(BigInt& cH, BigInt& mH)
{
    cH = m_fCurrentHP;
    mH = m_fMaxHP;
}


// Called every frame
void AUnitPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    m_fTickDeltaTime = DeltaTime;

    m_fAttackCD-=DeltaTime;
}


UPawnMovementComponent* AUnitPawn::GetMovementComponent() const
{
    return m_Movement;
}


void AUnitPawn::GetCapsuleSize(float& height, float& radius)
{
    height = m_Capsule->GetScaledCapsuleHalfHeight();
    radius = m_Capsule->GetScaledCapsuleRadius();
}

FRotator AUnitPawn::GetHomingRotToTarget()
{
    
    FRotator NewRot = GetActorRotation();

    if (!m_FocusedEnemy.Get())
    {
        return NewRot;
    }

    NewRot.Yaw = UKismetMathLibrary::RInterpTo(
        NewRot, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), m_FocusedEnemy->GetActorLocation()),
        m_fTickDeltaTime, 5.5f).Yaw;

    return NewRot;
}

void AUnitPawn::HomingRotateToTarget()
{
    if (!m_FocusedEnemy.Get())
    {
        return;
    }

    FRotator NewRot=GetHomingRotToTarget();
    
    SetActorRotation(NewRot);
}

bool AUnitPawn::IsEulerAngleAcceptForTarget(float eulerAngle)
{
    float AngleForDot = FMath::Cos(FMath::DegreesToRadians(eulerAngle));

    FVector const SelfToOther = GetFocusedTarget()->GetActorLocation() - GetActorLocation();

    FVector const SelfToOtherDir = SelfToOther.GetSafeNormal();
    
    FVector const MyFacingDir = GetActorRotation().Vector();

    return ((SelfToOtherDir | MyFacingDir) >= AngleForDot);
}

bool AUnitPawn::IsDotAngleAcceptForTarget(float dotAngle)
{
    FVector const SelfToOther = GetFocusedTarget()->GetActorLocation() - GetActorLocation();

    FVector const SelfToOtherDir = SelfToOther.GetSafeNormal();
    
    FVector const MyFacingDir = GetActorRotation().Vector();

    return ((SelfToOtherDir | MyFacingDir) >= dotAngle);
}

float AUnitPawn::GetHpPercentOne() const
{
    BigInt CopiedCH = m_fCurrentHP;

    CopiedCH.Multiply(100);

    CopiedCH.Divide(m_fMaxHP);
    
    float Percent100 = CopiedCH.ToInt();

    float Percent1 =  Percent100 / 100.f;

    Percent1 = FMath::Clamp(Percent1,0.f,1.f);
    
    return Percent1;
}

bool AUnitPawn::IsAlive() const
{
    return true;
}

FVector AUnitPawn::GetVelocity() const
{
    return GetMovementComponent()->Velocity;
}

FVector* AUnitPawn::GetVelocityPtr() const
{
    return &(GetMovementComponent()->Velocity);
}

void AUnitPawn::SetBlockMove()
{
    Cast<UUnitMovement>( GetMovementComponent())->SetMoveSpeed(0.f);
}

void AUnitPawn::SetUnblockMove()
{
    //Cast<UUnitMovement>( GetMovementComponent())->SetMoveSpeed(GetAttributeSet()->GetMoveSpeed());
}


float AUnitPawn::GetMoveSpeed()
{
    return GetMovementComponent()->GetMaxSpeed();
}

bool AUnitPawn::IsMoving()
{
    return !GetMovementComponent()->Velocity.IsZero();
}

int AUnitPawn::GetAccuLevel()
{
    return 1;
}

float AUnitPawn::GetAttackSpeed()
{
    return m_fAttackSpeed;
}

float AUnitPawn::GetAttackRange()
{
    return m_fAttackRange;
}

FVector AUnitPawn::GetLastSeenLocation()
{
    return FVector::ZeroVector;
}

void AUnitPawn::FocusTarget(AUnitPawn* target)
{
}

float AUnitPawn::PlayAnim(UAnimSequenceBase* animAsset, bool isLoop)
{
    m_SkBody->PlayAnimation(animAsset,isLoop);

    return animAsset->SequenceLength;
}

float AUnitPawn::PlayAnimMontage(UAnimMontage* anim_montage, float InPlayRate, FName StartSectionName)
{
    auto* AnimInstance = m_SkBody->GetAnimInstance();

    if (anim_montage && AnimInstance)
    {
        float const Duration = AnimInstance->Montage_Play(anim_montage, InPlayRate);

        if (Duration > 0.f)
        {
            // Start at a given Section.
            if (StartSectionName != NAME_None)
            {
                AnimInstance->Montage_JumpToSection(StartSectionName, anim_montage);
            }

            return Duration;
        }
    }

    return 0.f;
}

void AUnitPawn::StopAnimMontage(UAnimMontage* AnimMontage)
{
    UAnimInstance* AnimInstance = m_SkBody->GetAnimInstance();
    
    UAnimMontage* MontageToStop = (AnimMontage) ? AnimMontage : GetCurrentMontage();
    
    bool bShouldStopMontage = AnimInstance && MontageToStop && !AnimInstance->Montage_GetIsStopped(MontageToStop);

    if (bShouldStopMontage)
    {
        AnimInstance->Montage_Stop(MontageToStop->BlendOut.GetBlendTime(), MontageToStop);
    }
}

UAnimMontage* AUnitPawn::GetCurrentMontage()
{
    UAnimInstance* AnimInstance = m_SkBody->GetAnimInstance();
    if (AnimInstance)
    {
        return AnimInstance->GetCurrentActiveMontage();    
    }

    return nullptr;
}

void AUnitPawn::Die()
{
   
}


void AUnitPawn::StopMove()
{
    GetMovementComponent()->StopMovementImmediately();
    m_PFComp->PauseMove(FAIRequestID::CurrentRequest, EPathFollowingVelocityMode::Reset);
}
