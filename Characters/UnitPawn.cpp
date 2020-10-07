#include "UnitPawn.h"
#include "NavigationPath.h"
#include "NavigationData.h"
#include "NavigationSystem.h"
#include "AbilitySystem/Components/DiabloAbilitySystemComp.h"
#include "Managers/DiabloGameInstance.h"


AUnitPawn::AUnitPawn(const FObjectInitializer& objInit): Super(objInit)
{
    PrimaryActorTick.bCanEverTick = true;

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

    m_EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("Effect.RemoveOnDeath"));
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

void AUnitPawn::MoveToLocation(FVector goalLocation)
{
    const bool bAlreadyAtGoal = m_PFComp->HasReached(goalLocation, EPathFollowingReachMode::OverlapAgent);

    // script source, keep only one move request at time
    if (m_PFComp->GetStatus() != EPathFollowingStatus::Idle)
    {
        m_PFComp->AbortMove(*m_NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
                            , FAIRequestID::AnyRequest,
                            bAlreadyAtGoal ? EPathFollowingVelocityMode::Reset : EPathFollowingVelocityMode::Keep);
    }

    if (m_PFComp->GetStatus() != EPathFollowingStatus::Idle)
    {
        m_PFComp->AbortMove(*m_NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest);
    }

    if (bAlreadyAtGoal)
    {
        m_PFComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Success);
    }
    else
    {
        const ANavigationData* NavData = m_NavSys->GetNavDataForProps(GetNavAgentPropertiesRef());
        if (NavData)
        {
            FPathFindingQuery Query(this, *NavData, GetNavAgentLocation(), goalLocation);
            FPathFindingResult Result = m_NavSys->FindPathSync(Query);
            if (Result.IsSuccessful())
            {
                FAIMoveRequest MoveReq(goalLocation);
                MoveReq.SetAcceptanceRadius(m_fMoveAcceptRadius);
                m_PFComp->RequestMove(MoveReq, Result.Path);
            }
            else if (m_PFComp->GetStatus() != EPathFollowingStatus::Idle)
            {
                m_PFComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Invalid);
            }
        }
    }
}

void AUnitPawn::MoveToActor(AActor* goalTarget)
{
    if (!goalTarget)
    {
        return;
    }


    const bool bAlreadyAtGoal = m_PFComp->HasReached(*goalTarget, EPathFollowingReachMode::OverlapAgent);

    // script source, keep only one move request at time
    if (m_PFComp->GetStatus() != EPathFollowingStatus::Idle)
    {
        m_PFComp->AbortMove(*m_NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
                            , FAIRequestID::AnyRequest,
                            bAlreadyAtGoal ? EPathFollowingVelocityMode::Reset : EPathFollowingVelocityMode::Keep);
    }

    if (m_PFComp->GetStatus() != EPathFollowingStatus::Idle)
    {
        m_PFComp->AbortMove(*m_NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest);
    }

    if (bAlreadyAtGoal)
    {
        m_PFComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Success);
    }
    else
    {
        const ANavigationData* NavData = m_NavSys->GetNavDataForProps(GetNavAgentPropertiesRef());
        if (NavData)
        {
            FPathFindingQuery Query(this, *NavData, GetNavAgentLocation(), goalTarget->GetActorLocation());
            FPathFindingResult Result = m_NavSys->FindPathSync(Query);
            if (Result.IsSuccessful())
            {
                Result.Path->SetGoalActorObservation(*goalTarget, 100.0f);
                FAIMoveRequest MoveReq(goalTarget);
                MoveReq.SetAcceptanceRadius(m_fMoveAcceptRadius);
                m_PFComp->RequestMove(MoveReq, Result.Path);
            }
            else if (m_PFComp->GetStatus() != EPathFollowingStatus::Idle)
            {
                m_PFComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Invalid);
            }
        }
    }
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
    float Per= GetHealth() / GetMaxHealth();
    Per=FMath::Clamp(Per,0.f,1.f);
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
    RemoveAllGameplayAbilities();

    GetCapsule()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMovementComponent()->SetActive(false);

    m_OnCharacterDied.Broadcast(this);

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
        PlayAnimMontage(m_DeathMontage);
    }
    else
    {
        Destroy();
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
        m_GEUnitStat, GetLevel(), EffectContext);

    FActiveGameplayEffectHandle ActiveGEHandle = m_AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
        *NewHandle.Data.Get(), m_AbilitySystemComponent);
}

void AUnitPawn::RemoveAllGameplayAbilities()
{
    TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;

    for (const FGameplayAbilitySpec& Spec : GetDiaAbilitySystem()->GetActivatableAbilities())
    {
        bool A = m_GrantedSkillAbilities.Contains(Spec.Ability->GetClass());
        //bool B =m_GrantedMasteryAbilities.Contains(Spec.Ability->GetClass());
        //  bool C =m_GrantedItemAbilities.Contains(Spec.Ability->GetClass());

        if ((Spec.SourceObject == this) && A)
        {
            AbilitiesToRemove.Add(Spec.Handle);
        }
    }

    for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
    {
        GetDiaAbilitySystem()->ClearAbility(AbilitiesToRemove[i]);
    }
}
