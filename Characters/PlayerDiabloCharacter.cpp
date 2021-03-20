#include "PlayerDiabloCharacter.h"

#include "PlayerMovement.h"
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
	: Super(objInit.SetDefaultSubobjectClass<UPlayerMovement>("Movement00"))

{//UPlayerMovement
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
	m_DissolveCam->TargetArmLength = 1200.f;//1400
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

	m_Movement->SetRVOAvoidanceWeight(1);

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
	GetTickFSM()->m_OnMoveDone.AddUObject(UDiabloGameInstance::Get->GetPlCon(),&ADiabloPlayerController::HideMoveIndicator);
	m_OnMove.AddUObject(UDiabloGameInstance::Get->GetPlCon(),&ADiabloPlayerController::HideMoveIndicator);

	m_PlUpgradeManager = UDiabloGameInstance::Get->m_PlayerUpgradeManager;

	m_EquipManager = UDiabloGameInstance::Get->m_EquipManager;

	//PlayerClassDataInject(m_EquipManager->m_AryPlayerSkin[m_EquipManager->m_nSelectedSkin]);
	m_EquipManager->ClearSelectedIndex();
	m_EquipManager->EquipAll();

	m_OnRageChanged.Broadcast(m_fCurrentRage, m_fMaxRage);
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

	m_CreatedWeapon->AttachToComponent(m_SkBody, Rule, "RightHandBottom");
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

float APlayerDiabloCharacter::GetAttackSpeed()
{
	float As = m_fAttackSpeed;

	if(IsBuff02Available())
	{
		As *= m_fAdditionalAttackSpeed;
	}

	return As;
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

float APlayerDiabloCharacter::PlaySkillMontageSection(FName& nameID,int nSectionIndex,float& currentCD,float maxCD)
{
	float Len = PlayAttackMontage(currentCD,maxCD,&nameID);
	
	currentCD=maxCD;
	
	return Len;
}

void APlayerDiabloCharacter::TriggerSkill(const FName& name, TArray<FHitResult>* aryHits)
{
	bool IsAOE = aryHits;
	//m_QueDmgType.Empty();
	//
	if (name == "BaseAttack")
	{
		if (IsAOE)
		{
			ApplyDamageToTargets(*aryHits);
		}
		else
		{
			ApplyDamageToTarget();
		}
	}
	else if (name == "MagicBomb01") //작은 범위 공격
	{
		BigInt SkillDmg =m_PlUpgradeManager->GetAtkUp(EAttackType::MagicBombDmg).m_Value;
		
		ApplyDamageToTargets(*aryHits,&SkillDmg);
	}
	else if (name == "MagicBomb02") //작은 범위 공격
	{
		BigInt SkillDmg1 =m_PlUpgradeManager->GetAtkUp(EAttackType::MagicBombDmg).m_Value;
		BigInt SkillDmg2 =m_PlUpgradeManager->GetAtkUp(EAttackType::SuperMagicBombDmg).m_Value;

		BigInt Result = UPlayerUpgradeManager::MultiplePercent(SkillDmg1,SkillDmg2); 
		
		ApplyDamageToTargets(*aryHits,&Result);
	}
	else if (name == "Skill01") //작은 범위 공격
	{
		BigInt SkillDmg =m_PlUpgradeManager->GetSkillUp(ESkillType::MiniSlash).m_Value;
		
		ApplyDamageToTargets(*aryHits,&SkillDmg);
	}
	else if (name == "Skill02") //버프 공격
	{
		m_QueDmgType.Empty();
		StartBuff01(10);
	}
	else if (name == "Skill03") //휠윈드
	{
		BigInt SkillDmg =m_PlUpgradeManager->GetSkillUp(ESkillType::WhirlWind).m_Value;
		ApplyDamageToTargets(*aryHits,&SkillDmg);
	}
	else if (name == "Skill04") //데스블로우
	{
		BigInt SkillDmg =m_PlUpgradeManager->GetSkillUp(ESkillType::DeathBlow).m_Value;
		ApplyDamageToTargets(*aryHits,&SkillDmg);
	}
	else if (name == "Skill05") //버프 공속
	{
		m_QueDmgType.Empty();
		StartBuff02(10);
	}
	
}


float APlayerDiabloCharacter::PlayAttackMontage(float& currentCd,float maxCd,FName* sectionSkillName)
{
	
	FName SectionName = "Combo01";
	
	EDamageType DmgType = EDamageType::Base01;
	
	BigInt CriPercent100 = FMath::RandRange(0.f, 100.f);
	
	BigInt Cri01 = m_PlUpgradeManager->GetAtkUp(EAttackType::Critical).m_Value;
	
	if (CriPercent100 <= Cri01)//치명타 뜰때
	{
		SectionName = "Critical01";
		DmgType = EDamageType::Critical01;

		BigInt Cri2Percent100 = FMath::RandRange(0.f, 100.f);
	
		BigInt Cri02 = m_PlUpgradeManager->GetAtkUp(EAttackType::SuperCritical).m_Value;

		if (Cri2Percent100 <= Cri02)
		{
			SectionName = "Critical02";
			DmgType = EDamageType::Critical02;
		}
		m_QueDmgType.Enqueue(DmgType);
	}
	else//치명타 안뜸
	{
		if (FMath::RandBool())//비쥬얼만 다른 평타
		{
			SectionName = "Combo02";
			DmgType = EDamageType::Base02;
		}
		m_QueDmgType.Enqueue(DmgType);
	}
	//
	BigInt MagicPercent100 = FMath::RandRange(0.f, 100.f);
	
	BigInt Magic01 = m_PlUpgradeManager->GetAtkUp(EAttackType::MagicBomb).m_Value;

	bool bUseMagic = false;

	if(MagicPercent100<= Magic01)
	{
		bUseMagic=true;
		SectionName = "MagicBomb01";
		DmgType= EDamageType::Magic01;

		BigInt Magic2Percent100 = FMath::RandRange(0.f, 100.f);
	
		BigInt Magic02 = m_PlUpgradeManager->GetAtkUp(EAttackType::SuperMagicBomb).m_Value;

		if(Magic2Percent100<=Magic02)
		{
			SectionName = "MagicBomb02";
			DmgType= EDamageType::Magic02;
		}
	}

	PlayAnimMontage(m_BaseAttackAnim, 1 * GetAttackSpeed(),sectionSkillName? *sectionSkillName: SectionName);

	float AnimMongLen = m_BaseAttackAnim->GetSectionLength(sectionSkillName?m_BaseAttackAnim->GetSectionIndex(*sectionSkillName): (int)DmgType) / GetAttackSpeed();

	if(AnimMongLen>maxCd && !bUseMagic)
	{
		AnimMongLen=maxCd;
	}

	currentCd = AnimMongLen-0.1f;//-0.1f;
	//
	GetWorldTimerManager().ClearTimer(m_AttackTimer);
	//
	m_Movement->SetMoveSpeedRatio(0.1f);

	GetWorldTimerManager().SetTimer(m_AttackTimer, this, &APlayerDiabloCharacter::ApplyMoveSpeedToOrigin,
	                                AnimMongLen, false);

	return AnimMongLen;
}

float APlayerDiabloCharacter::TryAttack()
{
	if (UDiabloGameInstance::Get->m_PlayerUpgradeManager->IsSkillCasting())
	{
		return 1.f;
	}

	

	if (m_BaseAttackAnim && m_fAttackCD < 0.f)
	{
		PlayAttackMontage(m_fAttackCD,m_fAttackCDConstant);
	}

	return 1.f;
}

bool APlayerDiabloCharacter::GetDmg(BigInt& outDmg,EDamagePopup& pp)
{
	EDamageType Type;
	if (!m_QueDmgType.Dequeue(Type))
	{
		return false;
	}

	pp = EDamagePopup::NormalRight;

	outDmg = m_PlUpgradeManager->GetAtkUp(EAttackType::BaseAttack).m_Value;

	if(IsBuff01Available())
	{
		outDmg+=m_bnAdditionalSkillDmg;
	}

	outDmg = UPlayerUpgradeManager::MultiplePercent(outDmg,m_EquipManager->GetCurrentWeapon().m_Value);

	int Rand = FMath::RandRange(90, 110);

	outDmg = UPlayerUpgradeManager::MultiplePercent(outDmg,Rand);

	//150
	if (Type == EDamageType::Critical01) //치명타
	{
		BigInt CDmg01 = m_PlUpgradeManager->GetAtkUp(EAttackType::CriticalDmg).m_Value; //백기준으로 해야함,1.5배는  1
		
		outDmg = UPlayerUpgradeManager::MultiplePercent(outDmg,CDmg01);
		
		pp = EDamagePopup::CritcalRight;
	}
	else if (Type == EDamageType::Critical02) //슈퍼치명타
	{
		BigInt CDmg01 = m_PlUpgradeManager->GetAtkUp(EAttackType::CriticalDmg).m_Value; //백기준으로 해야함,1.5배는  1
		
		outDmg = UPlayerUpgradeManager::MultiplePercent(outDmg,CDmg01);
		
		BigInt SDmg02 = m_PlUpgradeManager->GetAtkUp(EAttackType::SuperCriticalDmg).m_Value; //백기준으로 해야함,1.5배는  1
		
		outDmg = UPlayerUpgradeManager::MultiplePercent(outDmg,SDmg02);

		pp = EDamagePopup::CritcalRight2;
	}

	return true;
}

void APlayerDiabloCharacter::StartBuff01(float sec)
{
	m_fBuff01MaxTime = sec;

	m_fBuff01DeltaCount = 0;
	//공격력 수치로 증가
	m_bnAdditionalSkillDmg = m_PlUpgradeManager->GetSkillUp(ESkillType::MagicBlade).m_Value;
	
	PRINTF("Buff01 Start");
}

bool APlayerDiabloCharacter::IsBuff01Available()
{
	return m_fBuff01DeltaCount<m_fBuff01MaxTime;
}

void APlayerDiabloCharacter::EndBuff01()
{
	m_fBuff01MaxTime = 0;

	m_fBuff01DeltaCount = 0;

	m_bnAdditionalSkillDmg=0;

	PRINTF("Buff01 End");
}

void APlayerDiabloCharacter::StartBuff02(float sec)
{
	m_fBuff02MaxTime = sec;

	m_fBuff02DeltaCount = 0;
	//공격속도 증가
	int Value = m_PlUpgradeManager->GetSkillUp(ESkillType::WindBlade).m_Value.ToInt();
	PRINTF("Value:%d",Value);
	m_fAdditionalAttackSpeed = Value / 100.f;//150%->1.5배

	PRINTF("Buff02 Start");
}

bool APlayerDiabloCharacter::IsBuff02Available()
{
	return m_fBuff02DeltaCount<m_fBuff02MaxTime;
}

void APlayerDiabloCharacter::EndBuff02()
{
	m_fBuff02MaxTime = 0;

	m_fBuff02DeltaCount = 0;

	m_fAdditionalAttackSpeed=0;
	
	PRINTF("Buff02 End");
}

void APlayerDiabloCharacter::ApplyDamageToTarget(const BigInt* additionalDmg)
{
	if (GetFocusedTarget())
	{
		BigInt FinalDmg;
		
		EDamagePopup Popup = EDamagePopup::NormalRight;

		if (!GetDmg(FinalDmg,Popup))
		{
			return;
		}

		if(additionalDmg)
		{
			FinalDmg = UPlayerUpgradeManager::MultiplePercent(FinalDmg,*additionalDmg);
			
		}

		ApplyDamage(GetFocusedTarget(), FinalDmg,Popup);

		m_QueDmgType.Empty();
	}
}


void APlayerDiabloCharacter::ApplyDamageToTargets(TArray<FHitResult>& aryTargets,const BigInt* additionalDmg)
{
	BigInt FinalDmg;

	EDamagePopup Popup = EDamagePopup::NormalRight;

	if (!GetDmg(FinalDmg,Popup))
	{
		return;
	}

	if(additionalDmg)
	{
		FinalDmg = UPlayerUpgradeManager::MultiplePercent(FinalDmg,*additionalDmg);
	}

	for (auto& Mob : aryTargets)
	{
		AUnitPawn* Pawn = Cast<AUnitPawn>(Mob.GetActor());

		if (Pawn)
		{
			ApplyDamage(Pawn, FinalDmg,Popup);
		}
	}
	m_QueDmgType.Empty();
}

void APlayerDiabloCharacter::ApplyDamage(AUnitPawn* target, const BigInt& finalDmg,EDamagePopup& pp)
{
	target->TakeDmg(finalDmg, this,pp);
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

		if (!IsBuff01Available())
		{
			EndBuff01();
		}
	}

	if (m_fBuff02MaxTime > 0)
	{
		m_fBuff02DeltaCount += DeltaTime;

		if (!IsBuff02Available())
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
			m_Movement->m_bUseRVO=false;
			GetMovementComponent()->StopMovementImmediately();
			m_TickFSM->ForceSetStateIdle();
			ApplyMoveSpeedToOrigin();
			FocusTarget(nullptr);
		}
	}

	if (bIsMoveInputZero && m_bUseFSM)
	{
		m_bIsManualMove = false;
		m_Movement->m_bUseRVO=true;
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
