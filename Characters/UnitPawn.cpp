#include "UnitPawn.h"
#include "NavigationPath.h"
#include "NavigationData.h"
#include "NavigationSystem.h"
#include "AbilitySystem/Components/DiabloAbilitySystemComp.h"
#include "Managers/DiabloGameInstance.h"


AUnitPawn::AUnitPawn(const FObjectInitializer& objInit): Super(objInit)
{
    PrimaryActorTick.bCanEverTick = true;
    m_bUseFSM = false;
    m_Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule00");
    m_Capsule->InitCapsuleSize(34.0f, 88.0f);
    m_Capsule->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    m_Capsule->CanCharacterStepUpOn = ECB_No;
    m_Capsule->SetShouldUpdatePhysicsVolume(true);
    m_Capsule->SetCanEverAffectNavigation(false);
    m_Capsule->bDynamicObstacle = true;
    RootComponent = m_Capsule;

    m_Movement = CreateDefaultSubobject<UUnitMovement>("Movement00");
    m_Movement->UpdatedComponent = m_Capsule;

    CreateSkMeshComponent(RootComponent, &m_SkBody, "SkMesh00");
    m_SkBody->bCastDynamicShadow = true;

    m_nCharacterLevel = 1;
    m_AbilitySystemComponent = CreateDefaultSubobject<UDiabloAbilitySystemComp>("AbilitySystemComponent00");
    m_AbilitySystemComponent->SetIsReplicated(true); //bCachedIsNetSimulated
    m_AttributeSet = CreateDefaultSubobject<UBaseDiabloAttribute>("AttributeSet00");

    m_PFComp = CreateDefaultSubobject<UPathFollowingComponent>(TEXT("PathFollowingComponent"));
    m_PFComp->SetMovementComponent(m_Movement);

    m_NavSys = nullptr;

    m_fMoveAcceptRadius = 100.f;

    m_EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.RemoveOnDeath"));
    m_DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
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
    (*refSkComp)->SetCollisionProfileName("CharacterMesh");
    (*refSkComp)->SetGenerateOverlapEvents(false);
    (*refSkComp)->SetCanEverAffectNavigation(false);

    (*refSkComp)->CastShadow = false;
    (*refSkComp)->bCastDynamicShadow = false;
    (*refSkComp)->bReceiveMobileCSMShadows = false;
}

// Called when the game starts or when spawned
void AUnitPawn::BeginPlay()
{
    Super::BeginPlay();
    m_NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

    
}

FPathFollowingRequestResult AUnitPawn::MoveToLocation(FVector goalLocation)
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
    MoveReq.SetAcceptanceRadius(GetAcceptRadiusSelfOnly()+10.f);
    MoveReq.SetReachTestIncludesAgentRadius(true);
    MoveReq.SetCanStrafe(true);

    return MoveTo(MoveReq);
}

FPathFollowingRequestResult AUnitPawn::MoveToActor(AActor* goalTarget)
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
    MoveReq.SetAcceptanceRadius(GetAcceptRadiusToOther());
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

    return GetController()->LineOfSightTo(GetFocusedTarget());
}


void AUnitPawn::StartAttack()
{
    m_OnStartAttack.Broadcast();
}

void AUnitPawn::EndAttack()
{
    m_OnEndAttack.Broadcast();
}

// Called every frame
void AUnitPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    m_fTickDeltaTime = DeltaTime;
 
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

UDiabloAbilitySystemComp* AUnitPawn::GetDiaAbilitySystem() const
{
    return m_AbilitySystemComponent;
}

void AUnitPawn::DoBaseAttack()
{
    GetDiaAbilitySystem()->TryActivateAbility(m_BaseAttackHandle);
}

void AUnitPawn::HomingRotateToTarget()
{
    if (!m_FocusedEnemy.Get())
    {
        return;
    }

    FRotator NewRot = GetActorRotation();

    NewRot.Yaw = UKismetMathLibrary::RInterpTo(
        NewRot, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), m_FocusedEnemy->GetActorLocation()),
        m_fTickDeltaTime, 5.5f).Yaw;
    SetActorRotation(NewRot);
}

UAbilitySystemComponent* AUnitPawn::GetAbilitySystemComponent() const
{
    return GetDiaAbilitySystem();
}


void AUnitPawn::PrintStats()
{
    //m_AttributeSet->PrintStats();
}


float AUnitPawn::GetHealth() const
{
    return m_AttributeSet->GetHealth();
}

float AUnitPawn::GetHpPercentOne() const
{
    float Per = GetHealth() / GetMaxHealth();
    Per = FMath::Clamp(Per, 0.f, 1.f);
    return Per;
}

float AUnitPawn::GetMaxHealth() const
{
    return m_AttributeSet->GetMaxHealth();
}

float AUnitPawn::GetMoveSpeed() const
{
    return m_AttributeSet->GetMoveSpeed();
}

bool AUnitPawn::HasDropItem()
{
    return false;
}


bool AUnitPawn::SetCharacterLevel(int NewLevel)
{
    if (NewLevel > MAXLEVEL)
        return false;

    if (m_nCharacterLevel != NewLevel && NewLevel > 0)
    {
        m_nCharacterLevel = NewLevel;
        SetUnitStatEffect();
    }
    return true;
}

bool AUnitPawn::IsAlive()
{
    return GetHealth() > 0.0f;
}

UBaseDiabloAttribute* AUnitPawn::GetAttributeSet() const
{
    return m_AttributeSet;
}

FVector AUnitPawn::GetVelocity() const
{
    return GetMovementComponent()->Velocity;
}

void AUnitPawn::FocusTarget(AUnitPawn* target)
{
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
    SetActorTickEnabled(false);
    GetCapsule()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMovementComponent()->SetActive(false);

    m_OnCharacterDied.Broadcast(this);

    m_bUseFSM=false;

    if (IsValid(GetDiaAbilitySystem()))
    {
        GetDiaAbilitySystem()->CancelAllAbilities();

        FGameplayTagContainer EffectTagsToRemove;
        EffectTagsToRemove.AddTag(m_EffectRemoveOnDeathTag);
        int32 NumEffectsRemoved = GetDiaAbilitySystem()->RemoveActiveEffectsWithTags(EffectTagsToRemove);

        GetDiaAbilitySystem()->AddLooseGameplayTag(m_DeadTag);
    }

    if (m_DeathMontage)
    {
        float AnimLength = PlayAnimMontage(m_DeathMontage) - 0.2f;
        
        if (GEngine->GetNetMode(GetWorld()) < NM_Client)
        {
            FTimerHandle TimerHandle_OnTimer;
            
            GetWorldTimerManager().SetTimer(TimerHandle_OnTimer, this, &AUnitPawn::OnDeathAnimEnd,
                                                           AnimLength,
                                                           false);
        }
    }
    else
    {
        OnDeathAnimEnd();
    }
}


float AUnitPawn::GetAttackSpeed() const
{
    return m_AttributeSet->GetAttackSpeed();
}

void AUnitPawn::SetUnitStatEffect()
{
    FGameplayEffectContextHandle EffectContext = m_AbilitySystemComponent->MakeEffectContext();
    EffectContext.AddSourceObject(this);

    FGameplayEffectSpecHandle NewHandle = m_AbilitySystemComponent->MakeOutgoingSpec(
        m_GEUnitStat, GetCharacterLevel(), EffectContext);

    FActiveGameplayEffectHandle ActiveGEHandle = m_AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
        *NewHandle.Data.Get(), m_AbilitySystemComponent);
}

