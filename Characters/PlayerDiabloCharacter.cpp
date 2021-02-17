#include "PlayerDiabloCharacter.h"

#include "AbilitySystem/Task/PlayMontageAndWaitForEvent.h"
#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Managers/DiabloGameInstance.h"
#include "Datas/CharacterDataTable.h"
#include "Characters/DiabloPlayerController.h"
#include "Camera/CameraDissolve.h"
#include "Managers/StartMap/PlayerCreateManager.h"
#include "Objs/Interfaces/Interactable.h"
#include "Item/Weapon.h"
#include "AbilitySystem/Ability/DiabloAbility.h"
#include "AbilitySystem/Ability/PlayerAbility/BaseSkillTemplate/PlayerBaseAttack.h"
#include "AbilitySystem/Ability/PlayerAbility/PlayerHealthPotion.h"
#include "AbilitySystem/Ability//PlayerAbility/Regen/PlayerHpRegenAbility.h"
#include "AbilitySystem/Ability//PlayerAbility/Regen/PlayerManaRegenAbility.h"
#include "AbilitySystem/Ability//PlayerAbility/Regen/PlayerStaminaRegenAbility.h"
#include "AbilitySystem/Components/PlayerDiabloAbilitySystemComp.h"
#include "Logic/PlayerSensing.h"
#include "Characters/MonsterPawn.h"

APlayerDiabloCharacter::APlayerDiabloCharacter(const FObjectInitializer& objInit)
	: Super(objInit.SetDefaultSubobjectClass<UPlayerDiabloAttribute>("AttributeSet00")
	               .SetDefaultSubobjectClass<UPlayerDiabloAbilitySystemComp>("AbilitySystemComponent00"))
{
	m_Capsule->SetCapsuleSize(55,88);
	
	
	m_DissolveCam = CreateDefaultSubobject<UCameraDissolve>("CamDissolve00");
	m_DissolveCam->SetupAttachment(RootComponent);
	m_DissolveCam->SetRelativeRotation(FRotator(-42.f, 45.f, 0.f));
	m_DissolveCam->SetRelativeLocation(FVector(0,0,0.f));
	//
	m_TopCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera00");
	m_TopCamera->SetupAttachment(m_DissolveCam);
	m_TopCamera->FieldOfView = 60.f;
	//m_TopCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);

	m_fInteractRange = 300.f;
	//

	m_SkBody->bCastDynamicShadow = true;
	m_SkBody->CastShadow = true;
	m_SkBody->bReceiveMobileCSMShadows = false;
	m_SkBody->SetRelativeLocation(FVector(0,0,-80.f));
	m_SkBody->SetRelativeRotation(FRotator(0,-90.f,-0.f));

	m_fCurrentExp = 0.f;

	m_fMaxExp = 0.f;

	m_bIsDead = false;

	m_Movement->SetRVOAvoidanceWeight(0.5f);

	m_Movement->m_RotateSpeed = FRotator(0.f,650.f,0.f);
	
//Material'/Game/03_VisualEffect/M_Fog.M_Fog'

	static ConstructorHelpers::FClassFinder<UDiabloAbility> Found1(TEXT("Blueprint'/Game/Blueprints/Abilities/Player/PlayerRegen/GA_PlayerHpRegen.GA_PlayerHpRegen_C'"));
	static ConstructorHelpers::FClassFinder<UDiabloAbility> Found2(TEXT("Blueprint'/Game/Blueprints/Abilities/Player/PlayerRegen/GA_PlayerManaRegen.GA_PlayerManaRegen_C'"));
	static ConstructorHelpers::FClassFinder<UDiabloAbility> Found3(TEXT("Blueprint'/Game/Blueprints/Abilities/Player/PlayerRegen/GA_PlayerStaminaRegen.GA_PlayerStaminaRegen_C'"));
	static ConstructorHelpers::FClassFinder<UDiabloAbility> Found4(TEXT("Blueprint'/Game/Blueprints/Abilities/Player/PlayerRegen/GA_PlayerRageRegen.GA_PlayerRageRegen_C'"));
	static ConstructorHelpers::FClassFinder<UDiabloAbility> Found5(TEXT("Blueprint'/Game/Blueprints/Abilities/Player/GA_PlayerPotion.GA_PlayerPotion_C'"));
	static ConstructorHelpers::FClassFinder<UDiabloAbility> Found6(TEXT("Blueprint'/Game/Blueprints/Abilities/Player/GA_PlayerPortal.GA_PlayerPortal_C'"));

	m_GAPlayerHealthRegen = Found1.Class;
	m_GAPlayerManaRegen = Found2.Class;
	m_GAPlayerStaminaRegen = Found3.Class;
	m_GAPlayerRageRegen = Found4.Class;
	m_GAPlayerHealthPotion = Found5.Class;
	m_GAPlayerPortal = Found6.Class;
	
	m_AryTargetingObjectType.Reset();
	m_AryTargetingObjectType.Add(EObjectTypeQuery::ObjectTypeQuery3);
	//m_fInteractRange
}


void APlayerDiabloCharacter::Init()
{
	m_PlayerCon = Cast<ADiabloPlayerController>(GetController());
	m_AryIgnoreActor.Add(this);
	m_AryIgnoreActor.Add(m_PlayerCon);
	//
	m_PlayerSense = NewObject<UPlayerSensing>(this, UPlayerSensing::StaticClass());
	m_PlayerSense->InitSense(this);
	m_PlayerSense->OnSeePawn.BindUObject(this, &APlayerDiabloCharacter::OnSeeTarget);
	m_PlayerSense->OnCantSeePawn.BindUObject(this, &APlayerDiabloCharacter::OnCantSeeTarget);
	m_PlayerSense->OnSeePawnBlocked.BindUObject(this, &APlayerDiabloCharacter::OnCanSeeTargetBlock);
	//
	m_PlayerAutoPlay=NewObject<UPlayerAutoPlayFSM>(this,UPlayerAutoPlayFSM::StaticClass());
	m_PlayerAutoPlay->Init(this);

	m_PlayerCon->SetViewTarget(this);

}

void APlayerDiabloCharacter::LateInit()
{
	m_DissolveCam->Init(m_TopCamera);
}


void APlayerDiabloCharacter::GrantHpRegenAbility()
{
	if (m_GAPlayerHealthRegen)
	{
		FGameplayAbilitySpec Spec = FGameplayAbilitySpec(m_GAPlayerHealthRegen, GetCharacterLevel(), -1, this);
		m_HpRegenHandle = GetDiaAbilitySystem()->GiveAbility(Spec);
	}
}

void APlayerDiabloCharacter::GrantResourceRegenAbility()
{
	auto* DiaAttri = GetPlayerAttribute();

	if (DiaAttri->GetMaxMana() && m_GAPlayerManaRegen)
	{
		FGameplayAbilitySpec Spec = FGameplayAbilitySpec(m_GAPlayerManaRegen, GetCharacterLevel(), -1, this);
		m_ResourceRegenHandle = GetDiaAbilitySystem()->GiveAbility(Spec);
	}
	else if (DiaAttri->GetMaxStamina() && m_GAPlayerStaminaRegen)
	{
		FGameplayAbilitySpec Spec = FGameplayAbilitySpec(m_GAPlayerStaminaRegen, GetCharacterLevel(), -1, this);
		m_ResourceRegenHandle = GetDiaAbilitySystem()->GiveAbility(Spec);
	}
	else if (DiaAttri->GetMaxRage() && m_GAPlayerRageRegen)
	{
		FGameplayAbilitySpec Spec = FGameplayAbilitySpec(m_GAPlayerRageRegen, GetCharacterLevel(), -1, this);
		m_ResourceRegenHandle = GetDiaAbilitySystem()->GiveAbility(Spec);
	}
}

void APlayerDiabloCharacter::GrantPortalAbility()
{
	if (m_GAPlayerPortal)
	{
		FGameplayAbilitySpec Spec = FGameplayAbilitySpec(m_GAPlayerPortal, GetCharacterLevel(),
		                                                 static_cast<int32>(m_GAPlayerPortal.GetDefaultObject()->
			                                                 m_AbilityInputID), this);
		m_PortalHandle = GetDiaAbilitySystem()->GiveAbility(Spec);
	}
}

void APlayerDiabloCharacter::SetLoadedData(const USaveCharacterStatus* loadedSaveData)
{
	m_nCharacterLevel = loadedSaveData->m_nLevel;

	if (m_nCharacterLevel == 0)
	{
		m_nCharacterLevel = 1;
	}

	m_TextUnitName = FText::FromString(loadedSaveData->m_TextName);
	//
	m_PlayerEntityData = UCharacterDataTable::GetPlayerEntityPtr(loadedSaveData->m_ClassName);

	m_GEUnitStat = m_PlayerEntityData->m_DefaultStatTable;

	m_DeathMontage = m_PlayerEntityData->m_DeathMontage;
	m_TookHitMontage = m_PlayerEntityData->m_TookHitMontage;


	SetCharacterLevel(m_nCharacterLevel);
	LoadExp(loadedSaveData);
	SetGold(loadedSaveData->m_fGold);

	GrantHpRegenAbility();
	GrantHpPotionAbility();
	GrantResourceRegenAbility();
	GrantPortalAbility();

	if(m_PlayerEntityData->m_AryPlayerSkin.IsValidIndex(loadedSaveData->m_IndexSkin))
	{
		m_SkBody->SetSkeletalMesh(m_PlayerEntityData->m_AryPlayerSkin[loadedSaveData->m_IndexSkin]);
	}
	else
	{
		m_SkBody->SetSkeletalMesh(m_PlayerEntityData->m_AryPlayerSkin[0]);
		PRINTF("PlayerSkinIndex Wrong,Zero Base Set");
	}
}

void APlayerDiabloCharacter::BeginPlay()
{
	Super::BeginPlay();

	Cast<ADiabloPlayerController>( GetController())->InitPlCtrlAndWidget();
}

void APlayerDiabloCharacter::LoadExp(const USaveCharacterStatus* loadedSaveData)
{
	m_fMaxExp = GetPlayerAttribute()->GetMaxExpForLevelUp();
	m_fCurrentExp = loadedSaveData->m_fExp;
	float RemainExp = m_fMaxExp - m_fCurrentExp;
	m_OnRemainExpChanged.Broadcast(RemainExp);
	m_OnExpGaugeChanged.Broadcast(m_fCurrentExp / m_fMaxExp);
}

void APlayerDiabloCharacter::RemoveAllEffect()
{
	FGameplayEffectQuery Query;
	Query.EffectSource = this;
	GetDiaAbilitySystem()->RemoveActiveEffects(Query);

	PRINTF("RemoveAllEffect");
}


void APlayerDiabloCharacter::SetBaseAttackData(float viewAngle, float viewRadius, float focusRange)
{
	m_PlayerSense->SetPeripheralVisionAngle(viewAngle);
	m_PlayerSense->SetViewRadius(viewRadius);
	m_PlayerSense->SetFocusRange(focusRange);
}

void APlayerDiabloCharacter::OnAttackPressed()
{
	m_bIsAttackInputPressed = true;
}

void APlayerDiabloCharacter::OnAttackRelease()
{
	m_bIsAttackInputPressed = false;
}

void APlayerDiabloCharacter::SetAnimStance(const FAnimStance* animStance)
{
	m_AnimStance = animStance;
	m_SkBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	m_SkBody->SetAnimInstanceClass(m_AnimStance->m_StanceAnimation);
	
	m_AlreadyHittenForIgnore.Reset();
}

void APlayerDiabloCharacter::EarnExp(float expEarned)
{
	PRINTF("ExpEarned:%f", expEarned);

	m_fCurrentExp += expEarned;

	float OverflowExp = m_fMaxExp - m_fCurrentExp;

	if (OverflowExp <= 0.f)
	{
		if (!SetCharacterLevel(m_nCharacterLevel + 1))
		{
			m_OnRemainExpChanged.Broadcast(0.f);
			m_OnExpGaugeChanged.Broadcast(0.f);
			return;
		}

		m_fCurrentExp = 0.f;
		m_fMaxExp = GetPlayerAttribute()->GetMaxExpForLevelUp();

		PRINTF("Next Exp Is: %f", m_fMaxExp);

		EarnExp(FMath::Abs(OverflowExp));
	}

	m_OnRemainExpChanged.Broadcast(m_fMaxExp - m_fCurrentExp);
	m_OnExpGaugeChanged.Broadcast(m_fCurrentExp / m_fMaxExp);
}

void APlayerDiabloCharacter::EarnGold(float goldEarned)
{
	m_fCurrentGold += goldEarned;
	m_fCurrentGold = FMath::Clamp(m_fCurrentGold, m_fCurrentGold,MAXVALUE);
	m_OnGoldChanged.Broadcast(m_fCurrentGold);
	PRINTF("GoldGained:%f", goldEarned);
	PRINTF("TotalGold:%f", m_fCurrentGold);
}

void APlayerDiabloCharacter::SetGold(float goldEarned)
{
	m_fCurrentGold = goldEarned;
	m_fCurrentGold = FMath::Clamp(m_fCurrentGold, m_fCurrentGold,MAXVALUE);
	m_OnGoldChanged.Broadcast(m_fCurrentGold);
	PRINTF("LoadedTotalGold:%f", m_fCurrentGold);
}

bool APlayerDiabloCharacter::SpendGold(float goldSpend)
{
	float ValueResult = m_fCurrentGold - goldSpend;

	if (ValueResult < 0)
	{
		return false;
	}

	m_fCurrentGold = ValueResult;
	m_OnGoldChanged.Broadcast(m_fCurrentGold);
	return true;
}

bool APlayerDiabloCharacter::SetCharacterLevel(int NewLevel)
{
	if (NewLevel > MAXLEVEL || NewLevel <= 0)
	{
		return false;
	}

	PRINTF("LevelUp: %d -> %d", m_nCharacterLevel, NewLevel);
	m_nCharacterLevel = NewLevel;
	SetUnitStatEffect();

	if (m_HpRegenHandle.IsValid())
	{
		GetDiaAbilitySystem()->ClearAbility(m_HpRegenHandle);
		GrantHpRegenAbility();
	}

	m_OnLevelChanged.Broadcast(m_nCharacterLevel);

	return true;
}

UPlayerBaseAttack* APlayerDiabloCharacter::GetBaseAttackInst()
{
	auto* BaseAbiliSpec = GetDiaAbilitySystem()->FindAbilitySpecFromHandle(m_BaseAttackHandle);
	return Cast<UPlayerBaseAttack>(BaseAbiliSpec->GetPrimaryInstance());
}

void APlayerDiabloCharacter::ResetCombo()
{
	GetBaseAttackInst()->ResetComboSection();
}

void APlayerDiabloCharacter::ShowOutlineOnTarget(AUnitPawn* Unit)
{
	if (m_FocusOutlinePawn.Get())
	{
		if (Unit == m_FocusOutlinePawn.Get())
		{
			return;
		}
		
		HideOutlineOnTarget();
	}
	
	m_FocusOutlinePawn = Unit;
	m_FocusOutlinePawn->GetSkMeshComp()->SetCustomDepthStencilValue(2);
	m_FocusOutlinePawn->GetSkMeshComp()->SetRenderCustomDepth(true);

	PRINTF("ShowOutlineOnTarget");
}

void APlayerDiabloCharacter::HideOutlineOnTarget()
{
	if(!m_FocusOutlinePawn.Get())
	{
		return;	
	}
	
	m_FocusOutlinePawn->GetSkMeshComp()->SetCustomDepthStencilValue(0);
	m_FocusOutlinePawn->GetSkMeshComp()->SetRenderCustomDepth(false);

	PRINTF("HideOutlineOnTarget");
}

void APlayerDiabloCharacter::FocusTarget(AUnitPawn* target)
{
	Super::FocusTarget(target);

	if (m_FocusedEnemy.Get()&&!target)
	{
		
		m_FocusedEnemy = nullptr;
		m_OnFocusTarget.Broadcast(nullptr);
		HideOutlineOnTarget();
		m_FocusedTargetDie.Reset();
		
		return;
	}

	AMonsterPawn* Unit = Cast<AMonsterPawn>(target);

	if (!Unit || target == m_FocusedEnemy) //캐스팅 실패하거나 이미 타겟팅 대상이면 스킵
	{
		return;
	}
	m_FocusedTargetDie.Reset();

	if (!Unit->IsStatusBarActive())
	{
		Unit->ShowStatusBar();
	}

	ShowOutlineOnTarget(Unit);

	m_OnFocusTarget.Broadcast(Unit);

	m_FocusedEnemy = Cast<AUnitPawn>(target);

	m_FocusedTargetDie = m_FocusedEnemy->GetOnDied().AddUObject(this, &APlayerDiabloCharacter::ClearFocusedTarget);

}

void APlayerDiabloCharacter::OnSeeTarget(APawn* target)
{
	AMonsterPawn* Unit = Cast<AMonsterPawn>(target);

	if (!Unit->IsStatusBarActive())
	{
		Unit->ShowStatusBar();
	}

	if (m_FocusedEnemy.Get())
	{
		return;
	}

	FocusTarget(Unit);
}

void APlayerDiabloCharacter::OnCantSeeTarget(APawn* target)
{
	AMonsterPawn* Unit = Cast<AMonsterPawn>(target);

	if (Unit->IsStatusBarActive())
	{
		Unit->HideStatusBar();
	}

	if (!m_FocusedEnemy.Get())
	{
		return;
	}

	if (m_FocusedEnemy != target)
	{
		return;
	}

	FocusTarget(nullptr);
}

void APlayerDiabloCharacter::OnCanSeeTargetBlock(APawn* target)
{
	AMonsterPawn* Unit = Cast<AMonsterPawn>(target);

	if (Unit->IsStatusBarActive())
	{
		Unit->HideStatusBar();
	}
}

ADiabloPlayerController* APlayerDiabloCharacter::GetDiaController()
{
	return m_PlayerCon;
}

void APlayerDiabloCharacter::Die()
{
	if (m_bIsDead)//Sometime call manytime
	{
		return;
	}
	
	m_bIsDead = true;

	m_bUseFSM = false;

	SetActorTickEnabled(false);

	m_AttributeSet->SetHealth(0.f);

	GetCapsule()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMovementComponent()->SetActive(false);

	m_PlayerSense->SetSensingUpdatesEnabled(false);

	GetDiaAbilitySystem()->CancelAbilities();
	
	//GetDiaAbilitySystem()->ClearAllAbilities();

	FGameplayTagContainer EffectTagsToRemove;
	
	EffectTagsToRemove.AddTag(m_TagEffectRemoveOnDeath);
	
	int32 NumEffectsRemoved = GetDiaAbilitySystem()->RemoveActiveEffectsWithTags(EffectTagsToRemove);

	GetDiaAbilitySystem()->AddLooseGameplayTag(m_TagDead);

	m_OnCharacterDied.Broadcast(this);

	if (m_DeathMontage)
	{
		float AnimLength = PlayAnim(m_DeathMontage) - 0.2f;

		if (GEngine->GetNetMode(GetWorld()) < NM_Client)
		{
			FTimerHandle TimerHandle_OnTimer;

			GetWorldTimerManager().SetTimer(TimerHandle_OnTimer, this, &APlayerDiabloCharacter::OnDeathAnimEnd,AnimLength,false);
		}
	}
	else
	{
		OnDeathAnimEnd();
		//Destroy();
	}
}

void APlayerDiabloCharacter::Revive()
{
	m_bIsDead = false;
	m_SkBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	GetDiaAbilitySystem()->RemoveLooseGameplayTag(m_TagDead);
	//
	GrantHpRegenAbility();
	GrantPortalAbility();
	GrantHpPotionAbility();
	GrantResourceRegenAbility();
	
	// GrantBaseAttackAbility();
	//
	GetCapsule()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMovementComponent()->SetActive(true);
	SetActorTickEnabled(true);
	m_PlayerSense->SetSensingUpdatesEnabled(true);
	m_AttributeSet->SetHealth(m_AttributeSet->GetMaxHealth());
	m_OnRevived.Broadcast(this);
	m_AttributeSet->m_OnStatChanged.Broadcast(this);
}


void APlayerDiabloCharacter::OnDeathAnimEnd()
{
	PRINTF("Game Over!");

	//HideUI? it can be broad cast
}

void APlayerDiabloCharacter::ClearFocusedTarget(AUnitPawn* target) //wrapper
{
	HideOutlineOnTarget();
	m_FocusedEnemy = nullptr;
	m_OnFocusTarget.Broadcast(nullptr);
	
	m_FocusedTargetDie.Reset();
}


FVector APlayerDiabloCharacter::GetLastSeenLocation()
{
	return m_PlayerSense->m_LastSeenLocation;
}

void APlayerDiabloCharacter::UpdateRegenAbility()
{
	if(m_ResourceRegenHandle.IsValid())
	{
		GetDiaAbilitySystem()->CancelAbilityHandle(m_ResourceRegenHandle);
	}
	GrantResourceRegenAbility();
}

bool APlayerDiabloCharacter::IsAlive() const
{
	return !m_bIsDead || Super::IsAlive();
}

void APlayerDiabloCharacter::GrantHpPotionAbility()
{
	if (m_GAPlayerHealthPotion)
	{
		FGameplayAbilitySpec Spec = FGameplayAbilitySpec(m_GAPlayerHealthPotion, GetCharacterLevel(),
		                                                 static_cast<int32>(m_GAPlayerHealthPotion.GetDefaultObject()->
			                                                 m_AbilityInputID), this);
		m_PotionHandle = GetDiaAbilitySystem()->GiveAbility(Spec);
	}
}

FGameplayAbilitySpec*  APlayerDiabloCharacter::DrinkPotion()
{
	if (!m_PotionHandle.IsValid())
	{
		return nullptr;
	}

	if (GetDiaAbilitySystem()->TryActivateAbility(m_PotionHandle))
	{
		return GetDiaAbilitySystem()->FindAbilitySpecFromHandle(m_PotionHandle);
	}

	return nullptr;
}

void APlayerDiabloCharacter::UsePortal()
{
	GetDiaAbilitySystem()->TryActivateAbility(m_PortalHandle);
}

void APlayerDiabloCharacter::CancelPortal()
{
	GetDiaAbilitySystem()->CancelAbilityHandle(m_PortalHandle);
}

float APlayerDiabloCharacter::GetCastSpeed()
{
	return GetPlayerAttribute()->GetCastingSpeed();
}

UPlayerDiabloAttribute* APlayerDiabloCharacter::GetPlayerAttribute()
{
	return Cast<UPlayerDiabloAttribute>(GetAttributeSet());
}

void APlayerDiabloCharacter::PlayColorEffect(const FLinearColor& colorWant,float effectLength)//애초에 사용된적이 없음
{
	FVector ColorV = UKismetMathLibrary::Conv_LinearColorToVector(colorWant);
	FName ColorParamName = "EffectColor";
	FName TimeParamName = "StartTime";
	FName EffectLengthParamName = "EffectLength";
	
	float TimeSec = UGameplayStatics::GetTimeSeconds(GetWorld());

	m_SkBody->SetVectorParameterValueOnMaterials(ColorParamName, ColorV);
	m_SkBody->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);
	m_SkBody->SetScalarParameterValueOnMaterials(EffectLengthParamName, effectLength);
}


void APlayerDiabloCharacter::InteractWithTarget()
{
	if (!m_FocusedInteractable.IsValid())
		return;

	m_FocusedInteractable->Interact(this);
	m_FocusedInteractable.Clear();
}

void APlayerDiabloCharacter::SetAutoPlay(bool useAuto)
{
	if(m_bUseFSM == useAuto)
	{
		return;
	}
	
	m_bUseFSM = useAuto;

	StopMove();
	
	if(m_bUseFSM)
	{
		PRINTF("UseAutoPlay");
	}
	else
	{
		PRINTF("NotuseAutoPlay");
		
	}
	
}


void APlayerDiabloCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_FocusedEnemy.Get())
	{
		if (!m_FocusedEnemy.Get()->IsAlive() || !m_PlayerSense->HasLineOfSightTo(m_FocusedEnemy.Get()))
		{
			FocusTarget(nullptr);
		}
	}

	m_PlayerSense->TickTryFoundInteraction();

	if (m_bUseFSM)
	{
		m_PlayerAutoPlay->TickFSM(DeltaTime);
	}


	TickAttack();
}


void APlayerDiabloCharacter::TickAttack()
{
	if (!m_bIsAttackInputPressed)
	{
		return;
	}

	HomingRotateToTarget();
	
	DoBaseAttack();
}

void APlayerDiabloCharacter::MoveForward(float AxisValue)
{
	m_Input.X = AxisValue;

	if (m_PlayerCon && (AxisValue != 0.0f))
	{
		m_OnMove.Broadcast();
		
		const FRotator Rotation = m_PlayerCon->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

void APlayerDiabloCharacter::MoveRight(float AxisValue)
{
	m_Input.Y = AxisValue;

	if (m_PlayerCon && (AxisValue != 0.0f))
	{
		m_OnMove.Broadcast();
		
		const FRotator Rotation = m_PlayerCon->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, AxisValue);
	}
}


void APlayerDiabloCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	m_PlayerCon = Cast<ADiabloPlayerController>(GetController());
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerDiabloCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerDiabloCharacter::MoveRight);
	PlayerInputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this,
	                                 &APlayerDiabloCharacter::InteractWithTarget);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &APlayerDiabloCharacter::OnAttackPressed);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Released, this,
	                                 &APlayerDiabloCharacter::OnAttackRelease);

	BindASCInput();
}

void APlayerDiabloCharacter::BindASCInput()
{
	if (GetDiaAbilitySystem() && IsValid(InputComponent))
	{
		GetDiaAbilitySystem()->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(
			                                                             FString("ConfirmTarget"),
			                                                             FString("CancelTarget"),
			                                                             FString("EAbilityInputID"),
			                                                             static_cast<int32>(EAbilityInputID::Confirm),
			                                                             static_cast<int32>(EAbilityInputID::Cancel)));
	}
}
