#include "UnitPawn.h"
#include "NavigationPath.h"
#include "NavigationData.h"
#include "NavigationSystem.h"
#include "Datas/CharacterDataTable.h"
#include "Datas/ItemDataTable.h"
#include "Managers/DiabloGameInstance.h"



AUnitPawn::AUnitPawn(const FObjectInitializer& objInit):Super(objInit)
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

	CreateSkMeshComponent(&m_SkMesh,"SkMesh00");

	m_nCharacterLevel = 1;
	m_AbilitySystemComponent = CreateDefaultSubobject<UDiabloAbilitySystemComp>("AbilitySystemComponent00");
	m_AbilitySystemComponent->SetIsReplicated(true);
	m_AttributeSet = CreateDefaultSubobject<UBaseDiabloAttribute>("AttributeSet00");

	m_PFComp = CreateDefaultSubobject<UPathFollowingComponent>(TEXT("PathFollowingComponent"));
	m_PFComp->SetMovementComponent(m_Movement);

	m_NavSys = nullptr;

	m_fMoveAcceptRadius = 100.f;
}
void AUnitPawn::CreateSkMeshComponent(USkeletalMeshComponent** refSkComp,FName keyName)
{
	(*refSkComp) = CreateDefaultSubobject<USkeletalMeshComponent>(keyName);
	(*refSkComp)->bOwnerNoSee = false;
	(*refSkComp)->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	(*refSkComp)->bCastDynamicShadow = true;//chanage for mobile
	(*refSkComp)->bAffectDynamicIndirectLighting = true;
	(*refSkComp)->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	(*refSkComp)->SetupAttachment(RootComponent);
	(*refSkComp)->SetCollisionProfileName("CharacterMesh");
	(*refSkComp)->SetGenerateOverlapEvents(false);
	(*refSkComp)->SetCanEverAffectNavigation(false);
}
// Called when the game starts or when spawned
void AUnitPawn::BeginPlay()
{
	Super::BeginPlay();
	m_NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	if (m_NameUnitID != NAME_None)
	{
		SetUnit(m_NameUnitID);
	}
}

void AUnitPawn::MoveToLocation(FVector goalLocation)
{
	const bool bAlreadyAtGoal = m_PFComp->HasReached(goalLocation, EPathFollowingReachMode::OverlapAgent);

	// script source, keep only one move request at time
	if (m_PFComp->GetStatus() != EPathFollowingStatus::Idle)
	{
		m_PFComp->AbortMove(*m_NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
			, FAIRequestID::AnyRequest, bAlreadyAtGoal ? EPathFollowingVelocityMode::Reset : EPathFollowingVelocityMode::Keep);
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

void AUnitPawn::MoveToActor(AActor * goalTarget)
{
	const bool bAlreadyAtGoal = m_PFComp->HasReached(*goalTarget, EPathFollowingReachMode::OverlapAgent);

	// script source, keep only one move request at time
	if (m_PFComp->GetStatus() != EPathFollowingStatus::Idle)
	{
		m_PFComp->AbortMove(*m_NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
			, FAIRequestID::AnyRequest, bAlreadyAtGoal ? EPathFollowingVelocityMode::Reset : EPathFollowingVelocityMode::Keep);
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

// Called every frame
void AUnitPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


UPawnMovementComponent * AUnitPawn::GetMovementComponent() const
{
	return m_Movement;
}

void AUnitPawn::MoveForward(float AxisValue)
{
	m_Input.X = AxisValue;

	if (m_PlayerCon && (AxisValue != 0.0f))
	{
		const FRotator Rotation = m_PlayerCon->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

void AUnitPawn::MoveRight(float AxisValue)
{
	m_Input.Y = AxisValue;

	if (m_PlayerCon && (AxisValue != 0.0f))
	{
		const FRotator Rotation = m_PlayerCon->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, AxisValue);
	}
}

void AUnitPawn::GetCapsuleSize(float & height, float & radius)
{
	height = m_Capsule->GetScaledCapsuleHalfHeight();
	radius = m_Capsule->GetScaledCapsuleRadius();
}

UAbilitySystemComponent * AUnitPawn::GetAbilitySystemComponent() const
{
	return m_AbilitySystemComponent;
}

void AUnitPawn::PrintStats()
{
	m_AttributeSet->PrintStats();
}

void AUnitPawn::SetUnit(FName unitID)
{
	m_NameUnitID = unitID;

	const FEntityTable* const UnitData = GetGameInstance<UDiabloGameInstance>()->GetMonsterUnitPtr(m_NameUnitID);

	m_SkMesh->SetSkeletalMesh(UnitData->m_Mesh);
	m_SkMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	m_SkMesh->SetAnimInstanceClass(UnitData->m_AnimBP);

	FGameplayEffectContextHandle EffectContext = m_AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = m_AbilitySystemComponent->MakeOutgoingSpec(UnitData->m_DefaultStatTable, GetLevel(), EffectContext);

	if (!NewHandle.IsValid())
	{
		PRINTF("Invalid Handle");
	}

	FActiveGameplayEffectHandle ActiveGEHandle = m_AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), m_AbilitySystemComponent);

	//현재 레벨 기본 스텟

	//기본 캐릭터 패시브 스킬

	//배운 스킬

	//아이템 옵션

	//기본 차스텟

	//

	//차스텟 보너스를 기본 스탯에
}

float AUnitPawn::GetHealth() const
{
	return m_AttributeSet->GetHealth();
}

float AUnitPawn::GetHpPercentOne() const
{
	return GetHealth() / GetMaxHealth();
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
	
	if(NewLevel> MAXLEVEL)
		return false;

	if (m_nCharacterLevel != NewLevel && NewLevel > 0)
	{
		// Our level changed so we need to refresh abilities
		//레벨업으로 업데이트시킬 스킬이 있나?
		//기본스텟있음
		RemoveStartupGameplayAbilities();
		m_nCharacterLevel = NewLevel;
		AddStartupGameplayAbilities();

	}
		return true;
}

bool AUnitPawn::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation)
{
	if (m_AbilitySystemComponent)
	{
		return m_AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, bAllowRemoteActivation);
	}

	return false;
}

void AUnitPawn::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UDiabloAbility*>& ActiveAbilities)
{
	//FGameplayAbilitySpecHandle* FoundHandle = SlottedAbilities.Find(ItemSlot);

	//if (FoundHandle && AbilitySystemComponent)
	//{
	//	FGameplayAbilitySpec* FoundSpec = AbilitySystemComponent->FindAbilitySpecFromHandle(*FoundHandle);

	//	if (FoundSpec)
	//	{
	//		TArray<UGameplayAbility*> AbilityInstances = FoundSpec->GetAbilityInstances();

	//		// Find all ability instances executed from this slot
	//		for (UGameplayAbility* ActiveAbility : AbilityInstances)
	//		{
	//			ActiveAbilities.Add(Cast<URPGGameplayAbility>(ActiveAbility));
	//		}
	//	}
	//}
}

bool AUnitPawn::GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float & TimeRemaining, float & CooldownDuration)
{
	return false;
}

void AUnitPawn::HandleDamage(float DamageAmount, const FHitResult & HitInfo, const FGameplayTagContainer & DamageTags, AUnitPawn * InstigatorCharacter, AActor * DamageCauser)
{
}

void AUnitPawn::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer & EventTags)
{
}

void AUnitPawn::HandleManaChanged(float DeltaValue, const FGameplayTagContainer & EventTags)
{
}

void AUnitPawn::HandleMoveSpeedChanged(float DeltaValue, const FGameplayTagContainer & EventTags)
{
}

void AUnitPawn::AddStartupGameplayAbilities()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		for (TSubclassOf<UDiabloAbility>& StartupAbility : m_StartGameplayAbilities)
		{
			m_AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetLevel(), INDEX_NONE, this));
		}

		for (TSubclassOf<UGameplayEffect>& GameplayEffect : m_PassiveGameplayEffects)
		{
			FGameplayEffectContextHandle EffectContext = m_AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = m_AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetLevel(), EffectContext);
			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = m_AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), m_AbilitySystemComponent);
			}
		}

		//AddSlottedGameplayAbilities();
	}
}

void AUnitPawn::RemoveStartupGameplayAbilities()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
		for (const FGameplayAbilitySpec& Spec : m_AbilitySystemComponent->GetActivatableAbilities())
		{
			if ((Spec.SourceObject == this) && m_StartGameplayAbilities.Contains(Spec.Ability->GetClass()))
			{
				AbilitiesToRemove.Add(Spec.Handle);
			}
		}

		for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
		{
			m_AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
		}

		FGameplayEffectQuery Query;
		Query.EffectSource = this;
		m_AbilitySystemComponent->RemoveActiveEffects(Query);

		//RemoveSlottedGameplayAbilities(true);
	}
}
