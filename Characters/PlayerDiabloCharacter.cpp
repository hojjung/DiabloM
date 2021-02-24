#include "PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"
#include "Datas/CharacterDataTable.h"
#include "Characters/DiabloPlayerController.h"
#include "Camera/CameraDissolve.h"
#include "Logic/PlayerSensing.h"
#include "Characters/MonsterPawn.h"
#include "Animations/MobAnimInstance.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Managers/PlayfabManager.h"

APlayerDiabloCharacter::APlayerDiabloCharacter(const FObjectInitializer& objInit)
	: Super(objInit)

{
	m_bIsManualMove=false;
	
	m_Capsule->SetCapsuleSize(55, 88);

	m_DissolveCam = CreateDefaultSubobject<UCameraDissolve>("CamDissolve00");
	m_DissolveCam->SetupAttachment(RootComponent);
	m_DissolveCam->SetRelativeRotation(FRotator(-50.f, 45.f, 0.f));
	m_DissolveCam->SetRelativeLocation(FVector(0, 0, 0.f));
	m_DissolveCam->TargetArmLength = 1400.f;
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

	m_Movement->SetMoveSpeed(710.f);

	//Material'/Game/03_VisualEffect/M_Fog.M_Fog'

	m_AryTargetingObjectType.Reset();
	m_AryTargetingObjectType.Add(EObjectTypeQuery::ObjectTypeQuery3);
	//m_fInteractRange
}

void APlayerDiabloCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_PlayerCon = Cast<ADiabloPlayerController>(GetController());
	m_AryIgnoreActor.Add(this);
	m_AryIgnoreActor.Add(m_PlayerCon);
	//
	m_PlayerSense = NewObject<UPlayerSensing>(this, UPlayerSensing::StaticClass());
	m_PlayerSense->InitSense(this);
	//
	m_PlayerCon->SetViewTarget(this);

	m_DissolveCam->Init(m_TopCamera);

	m_TickFSM = NewObject<UFSMTick>(this, UFSMTick::StaticClass());
	m_TickFSM->Init(this);

	m_PlUpgradeManager = UDiabloGameInstance::Get->m_PlayerUpgradeManager;

	PlayerClassDataInject(UDiabloGameInstance::Get->m_PlayerClassManager->GetPlayerEntity());

	SetAutoPlay(true);
}

void APlayerDiabloCharacter::PlayerClassDataInject(const FPlayerEntityTable* playerData)
{
	//check(playerData);
	if(!playerData)
	{
		PRINTF("DiaChar-NoPlData");
		return;
	}
	m_PlayerData = playerData;
	m_SkBody->SetSkeletalMesh(m_PlayerData->m_PlayerSkin);
	m_SkBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	m_SkBody->SetAnimInstanceClass(m_PlayerData->m_AnimBP);
	m_fAttackSpeed = m_PlayerData->m_fAttackSpeedMultiple;
	m_BaseAttackAnim = m_PlayerData->m_BaseAttackAnim;

	m_fAttackCDConstant = 1.f / m_fAttackSpeed;

	m_nAccuracyLevel = 10;
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

float APlayerDiabloCharacter::TryAttack()
{
	m_Movement->SetMoveSpeedRatio(0.1f);
	
	if(m_BaseAttackAnim&&m_fAttackCD<0.f)
	{
		float AnimMongLen = PlayAnimMontage(m_BaseAttackAnim,1*m_fAttackSpeed,NAME_None);

		m_fAttackCD =m_fAttackCDConstant;

		GetWorldTimerManager().SetTimer(m_AttackTimer, this, &APlayerDiabloCharacter::ApplyMoveSpeedToOrigin,m_fAttackCDConstant,false);
	}

	return 1.f;
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
		HideOutlineOnTarget();
	}

	m_FocusOutlinePawn = Unit;
	m_FocusOutlinePawn->GetSkMeshComp()->SetCustomDepthStencilValue(2);
	m_FocusOutlinePawn->GetSkMeshComp()->SetRenderCustomDepth(true);

}

void APlayerDiabloCharacter::HideOutlineOnTarget()
{
	if (!m_FocusOutlinePawn.Get())
	{
		return;
	}

	m_FocusOutlinePawn->GetSkMeshComp()->SetCustomDepthStencilValue(0);
	m_FocusOutlinePawn->GetSkMeshComp()->SetRenderCustomDepth(false);

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
		BigInt FinalDmg = m_PlUpgradeManager->GetPlAtkDmg01();

		BigInt CriPercent100 = FMath::RandRange(0.f,100.f);

		BigInt Cri01 = m_PlUpgradeManager->GetPlAtkCri01();

		BigInt CDmg01 = m_PlUpgradeManager->GetPlAtkCDmg01();//백기준으로 해야함,1.5배는  1
		//150
		if(CriPercent100 <= Cri01)
		{
			FinalDmg.Multiply(100);
			FinalDmg.Multiply(CDmg01);
			FinalDmg.Divide(10000);
			PRINTF("CriticalDamage:%s",*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(FinalDmg,2));
		}
		
		GetFocusedTarget()->TakeDmg(FinalDmg,this);
	}
}

void APlayerDiabloCharacter::ApplyMoveSpeedToOrigin()
{
	m_Movement->SetMoveSpeedRatio(1.f);
}

void APlayerDiabloCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool bIsMoveInputZero = m_Input.IsNearlyZero(0.1f);
	
	if (m_FocusedEnemy.Get())
	{
		DrawDebugLine(GetWorld(),GetActorLocation(),m_FocusedEnemy->GetActorLocation(),FColor::Red,false,-1,1,5.f);
		
		if (!m_bIsManualMove&&!bIsMoveInputZero)
		{
			m_bIsManualMove = true;
			GetMovementComponent()->StopMovementImmediately();
			ApplyMoveSpeedToOrigin();
		}

	}

	if (bIsMoveInputZero&&m_bUseFSM)
	{
		m_bIsManualMove = false;
		m_TickFSM->TickFSM();
	}

	
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

}
