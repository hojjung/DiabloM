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
#include "Managers/EquipManager.h"

APlayerDiabloCharacter::APlayerDiabloCharacter(const FObjectInitializer& objInit)
	: Super(objInit)

{
	m_fMaxRage = 100;
	m_fGainRagePer = 3;
	m_fCurrentRage = 0;

	m_nAccuracyLevel = 10;
	m_bIsManualMove = false;

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
	//
	m_PetComp = CreateDefaultSubobject<UChildActorComponent>("Child01");
	m_PetComp->SetupAttachment(RootComponent);
	m_PetComp->SetRelativeLocation(FVector(0, 90, 150));
	m_PetComp->SetRelativeRotation(FRotator(0, -90, 0));
	//
	m_SkBody->bCastDynamicShadow = true;
	m_SkBody->CastShadow = true;
	m_SkBody->bReceiveMobileCSMShadows = false;
	m_SkBody->SetRelativeLocation(FVector(0, 0, -80.f));
	m_SkBody->SetRelativeRotation(FRotator(0, -90.f, -0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundSkMesh(
		TEXT("SkeletalMesh'/Game/Models/ParagonMeshs/Greystone_SK.Greystone_SK'"));
	m_SkBody->SetSkeletalMesh(FoundSkMesh.Object);

	m_bIsDead = false;

	m_Movement->SetRVOAvoidanceWeight(0.5f);

	m_Movement->m_RotateSpeed = FRotator(0.f, 650.f, 0.f);

	m_Movement->SetMoveSpeed(630.f);

	//Material'/Game/03_VisualEffect/M_Fog.M_Fog'

	m_AryTargetingObjectType.Reset();
	m_AryTargetingObjectType.Add(EObjectTypeQuery::ObjectTypeQuery3);
	//m_fInteractRange
	//SoundWave'/Game/Sound/Sword_Swing_1_1.Sword_Swing_1_1'
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
	m_bUseFSM = true;

	m_PlUpgradeManager = UDiabloGameInstance::Get->m_PlayerUpgradeManager;

	m_EquipManager = UDiabloGameInstance::Get->m_EquipManager;

	//PlayerClassDataInject(m_EquipManager->m_AryPlayerSkin[m_EquipManager->m_nSelectedSkin]);
	m_EquipManager->ClearSelectedIndex();
	m_EquipManager->EquipAll();


	m_OnRageChanged.Broadcast(m_fCurrentRage, m_fMaxRage);

	//m_Movement->m_bUseRVO=true;
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




void APlayerDiabloCharacter::PlayerClassDataInject(const FPlayerClassSpec& spec)
{
	if (!spec.m_PlayerData)
	{
		PRINTF("DiaChar-NoSkinSpec");
		return;
	}
	m_PlayerEntityData = &spec;
	m_SkBody->SetSkeletalMesh(m_PlayerEntityData->m_PlayerData->m_PlayerSkin);
	m_SkBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	m_SkBody->SetAnimInstanceClass(m_PlayerEntityData->m_PlayerData->m_AnimBP);
	m_fAttackSpeed = m_PlayerEntityData->m_PlayerData->m_fAttackSpeedMultiple;
	m_BaseAttackAnim = m_PlayerEntityData->m_PlayerData->m_BaseAttackAnim;

	m_fAttackCDConstant = 1.f / m_fAttackSpeed;
}

void APlayerDiabloCharacter::WeaponDataInject(const FWeaponSpec& spec)
{
	if (!spec.m_EquipData)
	{
		PRINTF("DiaChar-NoWeaponSpec");
		return;
	}

	PRINTF("DiaChar-DataInject wPo");

	if (m_CreatedWeapon)
	{
		FDetachmentTransformRules Rule(EDetachmentRule::KeepWorld, false);
		m_CreatedWeapon->DetachFromActor(Rule);
		m_CreatedWeapon->Destroy();
	}

	if (!spec.m_EquipData->m_ClassVisualActor)
	{
		return;
	}

	FActorSpawnParameters Param;

	Param.bNoFail = true;

	m_CreatedWeapon = GetWorld()->SpawnActor<AEquipmentActor>(spec.m_EquipData->m_ClassVisualActor, GetActorLocation(),
	                                                          GetActorRotation(), Param);

	FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
	                               EAttachmentRule::KeepRelative, false);

	m_CreatedWeapon->AttachToComponent(m_SkBody, Rule, "Weapon");
}

void APlayerDiabloCharacter::WingDataInject(const FWingSpec& spec)
{
	if (!spec.m_WingData)
	{
		PRINTF("DiaChar-NoWingSpec");
		return;
	}

	PRINTF("DiaChar-DataInject Wing");

	if (m_CreatedWing)
	{
		FDetachmentTransformRules Rule(EDetachmentRule::KeepWorld, false);
		m_CreatedWing->DetachFromActor(Rule);
		m_CreatedWing->Destroy();
		m_Movement->m_fMoveSpeedMultiple = 1.f;
	}

	if (!spec.m_WingData->m_ClassVisualWingActor)
	{
		return;
	}

	FActorSpawnParameters Param;

	Param.bNoFail = true;

	m_CreatedWing = GetWorld()->SpawnActor<AEquipmentActor>(spec.m_WingData->m_ClassVisualWingActor, GetActorLocation(),
	                                                        GetActorRotation(), Param);

	FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
	                               EAttachmentRule::KeepRelative, false);

	m_CreatedWing->AttachToComponent(m_SkBody, Rule, "Wing");

	m_Movement->m_fMoveSpeedMultiple = spec.m_WingData->GetMoveSpdBonus();
}

void APlayerDiabloCharacter::AccessoryDataInject(const FAccessorySpec& spec)
{
	if (!spec.m_AccessoryData)
	{
		PRINTF("DiaChar-NoAccessorySpec");
		return;
	}

	PRINTF("DiaChar-DataInject Accesssory");
}

void APlayerDiabloCharacter::PetDataInject(const FPetSpec& spec)
{
	if (!spec.m_PetData)
	{
		PRINTF("DiaChar-NoPetSpec");
		return;
	}

	PRINTF("DiaChar-DataInject Pet");

	if (!spec.m_PetData->m_ClassPetSkin)
	{
		return;
	}

	m_PetComp->SetChildActorClass(spec.m_PetData->m_ClassPetSkin);
}


float APlayerDiabloCharacter::GetAttackSpeedMultiple()
{
	return m_fAttackSpeed;
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

		return;
	}

	AMonsterPawn* Unit = Cast<AMonsterPawn>(target);

	if (!Unit || target == m_FocusedEnemy) //캐스팅 실패하거나 이미 타겟팅 대상이면 스킵
	{
		return;
	}

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
}

bool APlayerDiabloCharacter::IsAlive() const
{
	return !m_bIsDead || Super::IsAlive();
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

void APlayerDiabloCharacter::TriggerSkill(const FName& name, TArray<FHitResult>* aryHits)
{
	bool IsAOE = aryHits;
	char DmgType = 0;
	//TArray<FHitResult> 
	//
	if (name == "BaseAttack")
	{
		m_QueDmgType.Enqueue(DmgType);

		if (IsAOE)
		{
			ApplyDamageToTargets(*aryHits);
		}
		else
		{
			ApplyDamageToTarget();
		}
	}
	else if (name == "Skill01") //작은 범위 공격
	{
		m_QueDmgType.Enqueue(DmgType);

		ApplyDamageToTargets(*aryHits);
	}
	else if (name == "Skill02") //버프 이속
	{
		m_QueDmgType.Enqueue(DmgType);
		StartBuff01(10);
	}
	else if (name == "Skill03") //휠윈드
	{
		m_QueDmgType.Enqueue(DmgType);
	}
	else if (name == "Skill01") //큰 범위 공격
	{
		m_QueDmgType.Enqueue(DmgType);
	}
	else if (name == "Buff01") //버프 공속
	{
		m_QueDmgType.Enqueue(DmgType);
		StartBuff02(10);
	}
}


float APlayerDiabloCharacter::TryAttack()
{
	if (UDiabloGameInstance::Get->m_PlayerUpgradeManager->IsSkillCasting())
	{
		return 1.f;
	}

	m_Movement->SetMoveSpeedRatio(0.1f);

	if (m_BaseAttackAnim && m_fAttackCD < 0.f)
	{
		FName SectionName = "Combo01";
		char DmgType = 0;
		//치명타인지

		//마력폭발인지
		//일반공격인지
		BigInt CriPercent100 = FMath::RandRange(0.f, 100.f);
		BigInt Cri01 = m_PlUpgradeManager->GetAtkUp(EAttackType::Critical).m_Value;
		//마력폭발 일어나면 더위에

		if (CriPercent100 <= Cri01)
		{
			SectionName = "MagicBomb";
			DmgType = 3;
		} //SectionName = "MagicBomb";
		else
		{
			if (FMath::RandBool())
			{
				SectionName = "Combo02";
				DmgType = 1;
			}
		}

		m_QueDmgType.Enqueue(DmgType);

		PlayAnimMontage(m_BaseAttackAnim, 1 * m_fAttackSpeed, SectionName);

		float AnimMongLen = m_BaseAttackAnim->GetSectionLength(DmgType);

		AnimMongLen /= m_fAttackSpeed;

		m_fAttackCD = m_fAttackCDConstant;

		float SpeedDelay = AnimMongLen - 0.1f;

		if (SpeedDelay < m_fAttackCDConstant)
		{
			SpeedDelay = m_fAttackCDConstant;
		}

		GetWorldTimerManager().SetTimer(m_AttackTimer, this, &APlayerDiabloCharacter::ApplyMoveSpeedToOrigin,
		                                SpeedDelay, false);
	}

	return 1.f;
}

bool APlayerDiabloCharacter::GetDmg(BigInt& outDmg)
{
	char Type;
	if (!m_QueDmgType.Dequeue(Type))
	{
		return false;
	}

	outDmg = m_PlUpgradeManager->GetAtkUp(EAttackType::BaseAttack).m_Value;

	outDmg.Multiply(100);
	outDmg.Multiply(m_EquipManager->GetCurrentWeapon().m_Value);
	outDmg.Divide(10000);

	int Rand = FMath::RandRange(90, 110);

	outDmg.Multiply(100);
	outDmg.Multiply(Rand);
	outDmg.Divide(10000);

	//150
	if (Type == 3) //치명타
	{
		BigInt CDmg01 = m_PlUpgradeManager->GetAtkUp(EAttackType::CriticalDmg).m_Value; //백기준으로 해야함,1.5배는  1
		outDmg.Multiply(100);
		outDmg.Multiply(CDmg01);
		outDmg.Divide(10000);
	}
	else if (Type == 4) //슈퍼치명타
	{
		BigInt SDmg01 = m_PlUpgradeManager->GetAtkUp(EAttackType::SuperCritical).m_Value; //백기준으로 해야함,1.5배는  1
		outDmg.Multiply(100);
		outDmg.Multiply(SDmg01);
		outDmg.Divide(10000);
	}
	else if (Type == 5) //폭발
	{
		BigInt SDmg02 = m_PlUpgradeManager->GetAtkUp(EAttackType::MagicBomb).m_Value; //백기준으로 해야함,1.5배는  1
		outDmg.Multiply(100);
		outDmg.Multiply(SDmg02);
		outDmg.Divide(10000);
	}
	else if (Type == 6) //슈퍼폭발
	{
		BigInt SDmg03 = m_PlUpgradeManager->GetAtkUp(EAttackType::SuperMagicBomb).m_Value; //백기준으로 해야함,1.5배는  1
		outDmg.Multiply(100);
		outDmg.Multiply(SDmg03);
		outDmg.Divide(10000);
	}
	else if (Type == 7) //휠윈드
	{
		BigInt SDmg04 = m_PlUpgradeManager->GetSkillUp(ESkillType::WhirlWind).m_Value; //백기준으로 해야함,1.5배는  1
		outDmg.Multiply(100);
		outDmg.Multiply(SDmg04);
		outDmg.Divide(10000);
	}
	else if (Type == 8) //스매시
	{
		BigInt SDmg05 = m_PlUpgradeManager->GetSkillUp(ESkillType::MiniSlash).m_Value; //백기준으로 해야함,1.5배는  1
		outDmg.Multiply(100);
		outDmg.Multiply(SDmg05);
		outDmg.Divide(10000);
	}
	else if (Type == 9) //데스블로우
	{
		BigInt SDmg06 = m_PlUpgradeManager->GetSkillUp(ESkillType::DeathBlow).m_Value; //백기준으로 해야함,1.5배는  1
		outDmg.Multiply(100);
		outDmg.Multiply(SDmg06);
		outDmg.Divide(10000);
	}

	return true;
}

void APlayerDiabloCharacter::StartBuff01(float sec)
{
	m_fBuff01MaxTime = sec;

	m_fBuff01DeltaCount = 0;
	//
}

void APlayerDiabloCharacter::EndBuff01()
{
	m_fBuff01MaxTime = 0;

	m_fBuff01DeltaCount = 0;
}

void APlayerDiabloCharacter::StartBuff02(float sec)
{
	m_fBuff02MaxTime = sec;

	m_fBuff02DeltaCount = 0;
	
}

void APlayerDiabloCharacter::EndBuff02()
{
	m_fBuff02MaxTime = 0;

	m_fBuff02DeltaCount = 0;
}

void APlayerDiabloCharacter::ApplyDamageToTarget()
{
	if (GetFocusedTarget())
	{
		BigInt FinalDmg;

		if (!GetDmg(FinalDmg))
		{
			return;
		}

		ApplyDamage(GetFocusedTarget(), FinalDmg);
	}
}


void APlayerDiabloCharacter::ApplyDamageToTargets(TArray<FHitResult>& aryTargets)
{
	BigInt FinalDmg;

	if (!GetDmg(FinalDmg))
	{
		return;
	}

	for (auto& Mob : aryTargets)
	{
		AUnitPawn* Pawn = Cast<AUnitPawn>(Mob.GetActor());

		if (Pawn)
		{
			ApplyDamage(Pawn, FinalDmg);
		}
	}
}

void APlayerDiabloCharacter::ApplyDamage(AUnitPawn* target, const BigInt& finalDmg)
{
	target->TakeDmg(finalDmg, this);
}

void APlayerDiabloCharacter::ApplyMoveSpeedToOrigin()
{
	m_Movement->SetMoveSpeedRatio(1.f);
}

int APlayerDiabloCharacter::GetAccuLevel()
{
	return m_EquipManager->GetCurrentWeapon().m_nAccuracy;
}

void APlayerDiabloCharacter::SetManualMoveLocation(FVector goalLocation)
{
	m_TickFSM->SetManualMove(goalLocation);
}

void APlayerDiabloCharacter::GainRagePoint()
{
	m_fCurrentRage += m_fGainRagePer;

	m_fCurrentRage = FMath::Clamp(m_fCurrentRage, 0.f, m_fMaxRage);

	m_OnRageChanged.Broadcast(m_fCurrentRage, m_fMaxRage);
}

bool APlayerDiabloCharacter::SpendRagePoint(float rage)
{
	if (GetRage() < rage)
	{
		return false;
	}

	m_fCurrentRage -= rage;

	m_fCurrentRage = FMath::Clamp(m_fCurrentRage, 0.f, m_fMaxRage);

	m_OnRageChanged.Broadcast(m_fCurrentRage, m_fMaxRage);


	return true;
}


void APlayerDiabloCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_fBuff01MaxTime > 0)
	{
		m_fBuff01DeltaCount += DeltaTime;

		if (m_fBuff01DeltaCount > m_fBuff01MaxTime)
		{
			EndBuff01();
		}
	}

	if (m_fBuff02MaxTime > 0)
	{
		m_fBuff02DeltaCount += DeltaTime;

		if (m_fBuff02DeltaCount > m_fBuff02MaxTime)
		{
			EndBuff02();
		}
	}

	bool bIsMoveInputZero = m_Input.IsNearlyZero(0.1f);

	if (m_FocusedEnemy.Get())
	{
		DrawDebugLine(GetWorld(), GetActorLocation(), m_FocusedEnemy->GetActorLocation(), FColor::Red, false, -1, 1,
		              5.f);

		if (!m_bIsManualMove && !bIsMoveInputZero)
		{
			m_bIsManualMove = true;
			//m_Movement->m_bUseRVO=false;
			GetMovementComponent()->StopMovementImmediately();
			m_TickFSM->ForceSetStateIdle();
			ApplyMoveSpeedToOrigin();
			FocusTarget(nullptr);
		}
	}

	if (bIsMoveInputZero && m_bUseFSM)
	{
		m_bIsManualMove = false;
		//m_Movement->m_bUseRVO=true;
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
}
