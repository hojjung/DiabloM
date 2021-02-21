#include "PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"
#include "Datas/CharacterDataTable.h"
#include "Characters/DiabloPlayerController.h"
#include "Camera/CameraDissolve.h"
#include "Logic/PlayerSensing.h"
#include "Characters/MonsterPawn.h"
#include "Animations/MobAnimInstance.h"
#include "Managers/PlayfabManager.h"

APlayerDiabloCharacter::APlayerDiabloCharacter(const FObjectInitializer& objInit)
	: Super(objInit)

{
	m_Capsule->SetCapsuleSize(55, 88);


	m_DissolveCam = CreateDefaultSubobject<UCameraDissolve>("CamDissolve00");
	m_DissolveCam->SetupAttachment(RootComponent);
	m_DissolveCam->SetRelativeRotation(FRotator(-50.f, 45.f, 0.f));
	m_DissolveCam->SetRelativeLocation(FVector(0, 0, 0.f));
	m_DissolveCam->TargetArmLength = 1800.f;
	//
	m_TopCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera00");
	m_TopCamera->SetupAttachment(m_DissolveCam);
	m_TopCamera->FieldOfView = 35.f;
	//m_TopCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);

	m_SkBody->bCastDynamicShadow = true;
	m_SkBody->CastShadow = true;
	m_SkBody->bReceiveMobileCSMShadows = false;
	m_SkBody->SetRelativeLocation(FVector(0, 0, -80.f));
	m_SkBody->SetRelativeRotation(FRotator(0, -90.f, -0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundSkMesh(
		TEXT("SkeletalMesh'/Game/Models/ParagonMeshs/Greystone_SK.Greystone_SK'"));
	m_SkBody->SetSkeletalMesh(FoundSkMesh.Object);

	//SkeletalMesh'/Game/Models/ParagonMeshs/Greystone_SK.Greystone_SK'

	m_fCurrentExp = 0.f;

	m_fMaxExp = 0.f;

	m_bIsDead = false;

	m_Movement->SetRVOAvoidanceWeight(0.5f);

	m_Movement->m_RotateSpeed = FRotator(0.f, 650.f, 0.f);

	m_Movement->SetMoveSpeed(610.f);

	//Material'/Game/03_VisualEffect/M_Fog.M_Fog'

	m_AryTargetingObjectType.Reset();
	m_AryTargetingObjectType.Add(EObjectTypeQuery::ObjectTypeQuery3);
	//m_fInteractRange
}

void APlayerDiabloCharacter::BeginPlay()
{
	Super::BeginPlay();

	Init();

	PlayerClassDataInject(UDiabloGameInstance::Get->m_PlayerClassManager->GetPlayerEntity());

	SetAutoPlay(true);
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
	//
	m_PlayerCon->SetViewTarget(this);

	m_DissolveCam->Init(m_TopCamera);

	m_TickFSM = NewObject<UFSMTick>(this, UFSMTick::StaticClass());
	m_TickFSM->Init(this);
}

void APlayerDiabloCharacter::PlayerClassDataInject(const FPlayerEntityTable* playerData)
{
	check(playerData);
	m_PlayerData = playerData;
	m_SkBody->SetSkeletalMesh(m_PlayerData->m_PlayerSkin);
	m_SkBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	m_SkBody->SetAnimInstanceClass(m_PlayerData->m_AnimBP);
	m_fAttackSpeed = m_PlayerData->m_fAttackSpeedMultiple;
	m_BaseAttackAnim = m_PlayerData->m_BaseAttackAnim;
}

void APlayerDiabloCharacter::SetBaseAttackData(float viewAngle, float viewRadius, float focusRange)
{
	m_PlayerSense->SetPeripheralVisionAngle(viewAngle);
	m_PlayerSense->SetViewRadius(viewRadius);
	m_PlayerSense->SetFocusRange(focusRange);
}

FVector APlayerDiabloCharacter::GetLastSeenLocation()
{
	return m_PlayerSense->m_LastSeenLocation;
}


void APlayerDiabloCharacter::OnAttackPressed()
{
	m_bIsAttackInputPressed = true;
}

void APlayerDiabloCharacter::OnAttackRelease()
{
	m_bIsAttackInputPressed = false;
}

void APlayerDiabloCharacter::EarnExp(float expEarned)
{
	PRINTF("ExpEarned:%f", expEarned);

	m_fCurrentExp += expEarned;

	float OverflowExp = m_fMaxExp - m_fCurrentExp;

	if (OverflowExp <= 0.f)
	{
		//		if (!SetCharacterLevel(m_nCharacterLevel + 1))
		{
			m_OnRemainExpChanged.Broadcast(0.f);
			m_OnExpGaugeChanged.Broadcast(0.f);
			return;
		}

		m_fCurrentExp = 0.f;
		//m_fMaxExp = GetPlayerAttribute()->GetMaxExpForLevelUp();

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

float APlayerDiabloCharacter::GetAttackSpeedMultiple()
{
	return m_fAttackSpeed;
}

void APlayerDiabloCharacter::ResetCombo()
{
	//GetBaseAttackInst()->ResetComboSection();
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
	if (!m_FocusOutlinePawn.Get())
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

	if (m_FocusedEnemy.Get() && !target)
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

	ShowOutlineOnTarget(Unit);

	m_OnFocusTarget.Broadcast(Unit);

	m_FocusedEnemy = Cast<AUnitPawn>(target);

	//m_FocusedTargetDie = m_FocusedEnemy->GetOnDied().AddUObject(this, &APlayerDiabloCharacter::ClearFocusedTarget);
}

void APlayerDiabloCharacter::OnSeeTarget(APawn* target)
{
	AMonsterPawn* Unit = Cast<AMonsterPawn>(target);

	if (m_FocusedEnemy.Get())
	{
		return;
	}

	FocusTarget(Unit);
}

void APlayerDiabloCharacter::OnCantSeeTarget(APawn* target)
{
	AMonsterPawn* Unit = Cast<AMonsterPawn>(target);

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


ADiabloPlayerController* APlayerDiabloCharacter::GetDiaController()
{
	return m_PlayerCon;
}

void APlayerDiabloCharacter::Die()
{
	
}

void APlayerDiabloCharacter::Revive()
{
	m_bIsDead = false;
	m_SkBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	// GrantBaseAttackAbility();
	GetCapsule()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMovementComponent()->SetActive(true);
	SetActorTickEnabled(true);
	m_PlayerSense->SetSensingUpdatesEnabled(true);
	m_OnRevived.Broadcast(this);
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


void APlayerDiabloCharacter::UpdateRegenAbility()
{
}

bool APlayerDiabloCharacter::IsAlive() const
{
	return !m_bIsDead || Super::IsAlive();
}

float APlayerDiabloCharacter::GetCastSpeed()
{
	return 1.f;
}

void APlayerDiabloCharacter::PlayColorEffect(const FLinearColor& colorWant, float effectLength) //애초에 사용된적이 없음
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
}

void APlayerDiabloCharacter::SetAutoPlay(bool useAuto)
{
	if (m_bUseFSM == useAuto)
	{
		return;
	}

	m_bUseFSM = useAuto;

	StopMove();

	if (m_bUseFSM)
	{
		PRINTF("UseAutoPlay");
	}
	else
	{
		PRINTF("NotuseAutoPlay");
	}
}

void APlayerDiabloCharacter::ApplyDamageToTarget()
{
	if(GetFocusedTarget())
	{
		GetFocusedTarget()->TakeDmg(12,this);
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

	if (m_Input.IsNearlyZero(0.1f)&&m_bUseFSM)
	{
		m_TickFSM->TickFSM();
	}
	else
	{
		if (m_FocusedEnemy.Get())
		{
			GetMovementComponent()->StopMovementImmediately();
			FocusTarget(nullptr);
		}
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
}
