#include "PlayerDiabloCharacter.h"

#include "OtherPlayerPawn.h"
#include "Managers/DiabloGameInstance.h"
#include "Characters/DiabloPlayerController.h"
#include "Camera/CameraDissolve.h"
#include "Animations/MobAnimInstance.h"
#include "Characters/Logic/PlayerMovement.h"
#include "Characters/Logic/PlayerSensing.h"
#include "Engine/AssetManager.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Managers/EquipManager.h"

APlayerDiabloCharacter::APlayerDiabloCharacter(const FObjectInitializer& objInit)
	: Super(objInit.SetDefaultSubobjectClass<UPlayerMovement>(TEXT("Movement00")))
{
	m_fMaxRage = 100;
	m_fGainRagePer = 3;
	m_fCurrentRage = 0;
	
	m_bIsManualMove = false;

	m_Capsule->SetCapsuleSize(55, 88);

	m_DissolveCam = CreateDefaultSubobject<UCameraDissolve>(TEXT("CamDissolve00"));
	m_DissolveCam->SetupAttachment(RootComponent);
	m_DissolveCam->SetRelativeRotation(FRotator(-55.f, 45.f, 0.f)); //-50
	m_DissolveCam->SetRelativeLocation(FVector(0, 0, 0.f));
	m_DissolveCam->TargetArmLength = 1375.f; //1400
	//
	m_TopCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera00"));
	m_TopCamera->SetupAttachment(m_DissolveCam);
	m_TopCamera->FieldOfView = 35.f;
	m_TopCamera->SetRelativeLocation(FVector(-150, 0, -150.f));
	//m_TopCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	//
	m_PetComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("Child01"));
	m_PetComp->SetupAttachment(RootComponent);
	m_PetComp->SetRelativeLocation(FVector(0, 90, 150));
	m_PetComp->SetRelativeRotation(FRotator(0, -90, 0));
	//
	m_SkBody->bCastDynamicShadow = true;
	m_SkBody->CastShadow = true;
	m_SkBody->bReceiveMobileCSMShadows = false;
	m_SkBody->SetRelativeLocation(FVector(0, 0, -80.f));
	m_SkBody->SetRelativeRotation(FRotator(0, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundSkMesh(
		TEXT("SkeletalMesh'/Game/Models/ParagonMeshs/Greystone_SK.Greystone_SK'"));
	m_SkBody->SetSkeletalMesh(FoundSkMesh.Object);

	m_Movement->SetRVOAvoidanceWeight(1);

	m_Movement->m_RotateSpeed = FRotator(0.f, 650.f, 0.f);

	m_Movement->SetMoveSpeed(630.f);

	//Material'/Game/03_VisualEffect/M_Fog.M_Fog'

	m_AryTargetingObjectType.Reset();
	m_AryTargetingObjectType.Add(EObjectTypeQuery::ObjectTypeQuery3);
	//m_fInteractRange
	//SoundWave'/Game/Sound/Sword_Swing_1_1.Sword_Swing_1_1'

	// static ConstructorHelpers::FClassFinder<UUserWidget> FoundW(
	// TEXT("WidgetBlueprint'/Game/Blueprints/Widget/CommonElement/WB_TextPlayerName.WB_TextPlayerName_C'"));
	// m_NameCard = Create("asd",FoundW.Class);
	//m_NameCard->SetWidgetClass(FoundW.Class);
	m_NameCard = CreateDefaultSubobject<UFloatingTextWidgetComponent>(TEXT("NameCard"));
	m_NameCard->SetupAttachment(RootComponent);
	static ConstructorHelpers::FClassFinder<UUserWidget> FoundW(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/CommonElement/WB_TextPlayerName.WB_TextPlayerName_C'"));
	m_NameCard->SetWidgetClass(FoundW.Class);
	m_NameCard->SetRelativeLocation(FVector(0, 0, 100));
	m_NameCard->SetVisibility(false);
}

void APlayerDiabloCharacter::UpdateRage()
{
	m_OnRageChanged.Broadcast(m_fCurrentRage, m_fMaxRage);
}

FString APlayerDiabloCharacter::GetRageFormatStr()
{
	return FString::Printf(TEXT("분노 %d/%d"),(int)m_fCurrentRage,(int)m_fMaxRage);
}

float APlayerDiabloCharacter::GetRagePercent()
{
	return m_fCurrentRage / m_fMaxRage;
}

void APlayerDiabloCharacter::SetFSM_Enable(bool v)
{
	m_bUseFSM = v;

	if(!m_bUseFSM)
	{
		FocusTarget(nullptr);
	}
}

void APlayerDiabloCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_PlayerCon = Cast<ADiabloPlayerController>(GetController());
	//
	m_PlayerSense = NewObject<UPlayerSensing>(this, UPlayerSensing::StaticClass());
	m_PlayerSense->InitSense(this);
	//
	m_PlayerCon->SetViewTarget(this);

	m_DissolveCam->Init(m_TopCamera);
	
	m_AutoSkillUse=NewObject<UAutoSkillUse>(this, UAutoSkillUse::StaticClass());
	
	m_TickFSM = NewObject<UFSMTick>(this, UFSMTick::StaticClass());
	m_TickFSM->Init(this);
	m_bUseFSM = true;
	GetTickFSM()->m_OnMoveDone.AddUObject(UDiabloGameInstance::Get->GetPlCon(),
	                                      &ADiabloPlayerController::HideMoveIndicator);
	m_OnMove.AddUObject(UDiabloGameInstance::Get->GetPlCon(), &ADiabloPlayerController::HideMoveIndicator);

	m_PlUpgradeManager = UDiabloGameInstance::Get->m_PlayerUpgradeManager;

	m_EquipManager = UDiabloGameInstance::Get->m_EquipManager;

	//PlayerClassDataInject(m_EquipManager->m_AryPlayerSkin[m_EquipManager->m_nSelectedSkin]);
	m_EquipManager->ClearSelectedIndex();
	m_EquipManager->EquipAll();

	UpdateRage();
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

	FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();


	if(m_SkBody->SkeletalMesh)
	{
		StreamableManager.Unload(m_SkBody->SkeletalMesh);
	}

	if(m_SkBody->GetAnimInstance())
	{
		StreamableManager.Unload(m_SkBody->GetAnimInstance());
	}
	
	if(m_BaseAttackAnim)
	{
		StreamableManager.Unload(m_BaseAttackAnim);
	}
	
	auto* LoadedMesh =StreamableManager.LoadSynchronous(m_PlayerEntityData->m_PlayerData->m_PlayerSkinSoft, true);
	//
	auto LoadedAnim =StreamableManager.LoadSynchronous(m_PlayerEntityData->m_PlayerData->m_AnimBP, true);

	m_BaseAttackAnim =StreamableManager.LoadSynchronous(m_PlayerEntityData->m_PlayerData->m_BaseAttackAnim, true);
	//
	m_SkBody->SetSkeletalMesh(LoadedMesh);
	m_SkBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	m_SkBody->SetAnimInstanceClass(LoadedAnim);
	m_SkBody->SetForcedLOD(2);
	m_fAttackSpeed = m_PlayerEntityData->m_PlayerData->m_fAttackSpeedMultiple;

	
	
	m_fAttackCDConstant = 1.f / m_fAttackSpeed;

	m_OnMeshChanged.Broadcast(this);

	FTimerHandle Timer;
	
	UDiabloGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(
		Timer, UDiabloGameInstance::Get->GetPlCon(), &ADiabloPlayerController::ClientForceGarbageCollection, 2.2f, false);
}

void APlayerDiabloCharacter::WeaponDataInject(const FWeaponSpec& spec)
{
	if (!spec.m_EquipData)
	{
		PRINTF("DiaChar-NoWeaponSpec");
		return;
	}

	FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
	
	if (m_CreatedWeapon)
	{
		StreamableManager.Unload(m_CreatedWeapon->GetClass());
		FDetachmentTransformRules Rule(EDetachmentRule::KeepWorld, false);
		m_CreatedWeapon->DetachFromActor(Rule);
		m_CreatedWeapon->Destroy();
	}


	auto LoadedEquip =StreamableManager.LoadSynchronous(spec.m_EquipData->m_ClassVisualActor, true);
	//


	FActorSpawnParameters Param;

	Param.bNoFail = true;

	m_CreatedWeapon = GetWorld()->SpawnActor<AEquipmentActor>(LoadedEquip,Param);

	FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
	                               EAttachmentRule::KeepRelative, false);

	m_CreatedWeapon->AttachToComponent(m_SkBody, Rule, TEXT("RightHandBottom"));

	m_OnMeshChanged.Broadcast(this);
}

void APlayerDiabloCharacter::WingDataInject(const FWingSpec& spec)
{
	if (!spec.m_WingData)
	{
		PRINTF("DiaChar-NoWingSpec");
		return;
	}

	FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();

	if (m_CreatedWing)
	{
		StreamableManager.Unload(m_CreatedWing->GetClass());
		FDetachmentTransformRules Rule(EDetachmentRule::KeepWorld, false);
		m_CreatedWing->DetachFromActor(Rule);
		m_CreatedWing->Destroy();
		m_Movement->m_fMoveSpeedMultiple = 1.f;
	}


	auto LoadedEquip =StreamableManager.LoadSynchronous(spec.m_WingData->m_ClassVisualWingActor, true);
	//


	FActorSpawnParameters Param;

	Param.bNoFail = true;

	m_CreatedWing = GetWorld()->SpawnActor<AEquipmentActor>(LoadedEquip, GetActorLocation(),
	                                                        FRotator(0.f), Param);

	FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
	                               EAttachmentRule::KeepRelative, false);

	m_CreatedWing->AttachToComponent(m_SkBody, Rule, TEXT("Wing"));
	
	m_CreatedWing->SetActorRelativeLocation(FVector(-10,10,0));
	
	m_CreatedWing->SetActorRelativeRotation(FRotator(-90,0,0));
	
	m_Movement->m_fMoveSpeedMultiple = spec.m_WingData->GetMoveSpdBonus();

	m_OnMeshChanged.Broadcast(this);
}

void APlayerDiabloCharacter::AccessoryDataInject(const FAccessorySpec& spec)
{
	if (!spec.m_AccessoryData)
	{
		PRINTF("DiaChar-NoAccessorySpec");
		return;
	}


	m_OnMeshChanged.Broadcast(this);
}

void APlayerDiabloCharacter::PetDataInject(const FPetSpec& spec)
{
	if (!spec.m_PetData)
	{
		PRINTF("DiaChar-NoPetSpec");
		return;
	}

	FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();


	if (!spec.m_PetData->m_ClassPetSkin)
	{
		return;
	}

	if(m_PetComp->GetChildActorTemplate())
	{
		StreamableManager.Unload(m_PetComp->GetChildActorTemplate());
	}

	auto LoadedEquip =StreamableManager.LoadSynchronous(spec.m_PetData->m_ClassPetSkin, true);
	//
	

	m_PetComp->SetChildActorClass(LoadedEquip);

	m_OnMeshChanged.Broadcast(this);
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

	if (IsBuff02Available())
	{
		As *= m_fAdditionalAttackSpeed;
	}

	if (UDiabloGameInstance::Get->m_ShopManager->GetPackagePurchased(0))
	{
		As *= 1.5f;
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

	AUnitPawn* Unit = Cast<AUnitPawn>(target);

	if (!Unit || target == m_FocusedEnemy) //캐스팅 실패하거나 이미 타겟팅 대상이면 스킵a
	{
		return;
	}

	m_FocusedEnemy = Unit;

	ShowOutlineOnTarget(m_FocusedEnemy.Get());

	m_OnFocusTarget.Broadcast(m_FocusedEnemy.Get());
	//m_FocusedTargetDie = m_FocusedEnemy->GetOnDied().AddUObject(this, &APlayerDiabloCharacter::ClearFocusedTarget);
}


ADiabloPlayerController* APlayerDiabloCharacter::GetDiaController()
{
	return m_PlayerCon;
}

void APlayerDiabloCharacter::Die()
{
}

void APlayerDiabloCharacter::OnDeathAnimEnd()
{
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
	return true;
}

void APlayerDiabloCharacter::PlayColorEffect(const FLinearColor& colorWant, float effectLength) //애초에 사용된적이 없음
{
	FVector ColorV = UKismetMathLibrary::Conv_LinearColorToVector(colorWant);
	FName ColorParamName = TEXT("EffectColor");
	FName TimeParamName = TEXT("StartTime");
	FName EffectLengthParamName = TEXT("EffectLength");

	float TimeSec = UGameplayStatics::GetTimeSeconds(GetWorld());

	m_SkBody->SetVectorParameterValueOnMaterials(ColorParamName, ColorV);
	m_SkBody->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);
	m_SkBody->SetScalarParameterValueOnMaterials(EffectLengthParamName, effectLength);
}

float APlayerDiabloCharacter::PlaySkillMontageSection(FName& nameID, int nSectionIndex, float& currentCD, float maxCD)
{
	float Len = PlayAttackMontage(currentCD, maxCD, &nameID);

	currentCD = maxCD;

	return Len;
}

void APlayerDiabloCharacter::TriggerSkill(const FName& name, TArray<FHitResult>* aryHits)
{
	bool IsAOE = aryHits;
	//m_QueDmgType.Empty();
	//
	if (name == TEXT("BaseAttack"))
	{
		GainRagePoint();

		if (IsAOE)
		{
			ApplyDamageToTargets(*aryHits);
		}
		else
		{
			ApplyDamageToTarget();
		}
	}
	else if (name == TEXT("MagicBomb01")) //작은 범위 공격
	{
		BigInt SkillDmg = m_PlUpgradeManager->GetAtkUp(EAttackType::MagicBombDmg).m_Value;

		if (UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce05).Level > 0)
		{
			SkillDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(
				SkillDmg, UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce05).m_Value);
		}

		ApplyDamageToTargets(*aryHits, &SkillDmg);
	}
	else if (name == TEXT("MagicBomb02")) //작은 범위 공격
	{
		BigInt SkillDmg1 = m_PlUpgradeManager->GetAtkUp(EAttackType::MagicBombDmg).m_Value;
		BigInt SkillDmg2 = m_PlUpgradeManager->GetAtkUp(EAttackType::SuperMagicBombDmg).m_Value;
		BigInt Result = UDiaBlueprintFunctionLibrary::MultiplePercent(SkillDmg1, SkillDmg2);

		if (UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce05).Level > 0)
		{
			Result = UDiaBlueprintFunctionLibrary::MultiplePercent(
				Result, UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce05).m_Value);
		}

		if (UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce06).Level > 0)
		{
			Result = UDiaBlueprintFunctionLibrary::MultiplePercent(
				Result, UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce06).m_Value);
		}

		ApplyDamageToTargets(*aryHits, &Result);
	}
	else if (name == TEXT("Skill01")) //작은 범위 공격
	{
		BigInt SkillDmg = m_PlUpgradeManager->GetSkillUp(ESkillType::DeathBlow).m_Value;

		ApplyDamageToTargets(*aryHits, &SkillDmg);
	}
	else if (name == TEXT("Skill02")) //버프 공격
	{
		m_QueDmgType.Empty();
		StartBuff01(20);
	}
	else if (name == TEXT("Skill03")) //휠윈드
	{
		BigInt SkillDmg = m_PlUpgradeManager->GetSkillUp(ESkillType::WhirlWind).m_Value;
		ApplyDamageToTargets(*aryHits, &SkillDmg);
	}
	else if (name == TEXT("Skill04")) //데스블로우
	{
		BigInt SkillDmg = m_PlUpgradeManager->GetSkillUp(ESkillType::EarthQuake).m_Value;
		ApplyDamageToTargets(*aryHits, &SkillDmg);
	}
	else if (name == TEXT("Skill05")) //버프 공속
	{
		m_QueDmgType.Empty();
		StartBuff02(17);
	}
}

void APlayerDiabloCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
}


float APlayerDiabloCharacter::PlayAttackMontage(float& currentCd, float maxCd, FName* sectionSkillName)
{
	FName SectionName = TEXT("Combo01");

	EDamageType DmgType = EDamageType::Base01;

	BigInt Cri01 = m_PlUpgradeManager->GetAtkUp(EAttackType::Critical).m_Value;

	BigInt CriPercent100 = FMath::RandRange(0.f, 100.f);

	if (Cri01 > 0 && CriPercent100 <= Cri01) //치명타 뜰때
	{
		SectionName = TEXT("Critical01");
		DmgType = EDamageType::Critical01;

		BigInt Cri2Percent100 = FMath::RandRange(0.f, 100.f);

		BigInt Cri02 = m_PlUpgradeManager->GetAtkUp(EAttackType::SuperCritical).m_Value;

		if (Cri02 > 0 && Cri2Percent100 <= Cri02)
		{
			SectionName = TEXT("Critical02");
			DmgType = EDamageType::Critical02;
		}
		m_QueDmgType.Enqueue(DmgType);
	}
	else //치명타 안뜸
	{
		if (FMath::RandBool()) //비쥬얼만 다른 평타
		{
			SectionName = TEXT("Combo02");
			DmgType = EDamageType::Base02;
		}
		m_QueDmgType.Enqueue(DmgType);
	}
	//
	BigInt Magic01 = m_PlUpgradeManager->GetAtkUp(EAttackType::MagicBomb).m_Value;

	bool bUseMagic = false;

	BigInt MagicPercent100 = FMath::RandRange(0.f, 100.f);

	if (Magic01 > 0 && MagicPercent100 <= Magic01)
	{
		bUseMagic = true;
		SectionName = TEXT("MagicBomb01");
		DmgType = EDamageType::Magic01;

		BigInt Magic2Percent100 = FMath::RandRange(0.f, 100.f);

		BigInt Magic02 = m_PlUpgradeManager->GetAtkUp(EAttackType::SuperMagicBomb).m_Value;

		if (Magic2Percent100 <= Magic02)
		{
			SectionName = TEXT("MagicBomb02");
			DmgType = EDamageType::Magic02;
		}
	}

	if (sectionSkillName)
	{
		float AttackSpeed = 1.f;

		if (*sectionSkillName != TEXT("Skill03"))
		{
			AttackSpeed = 1 * GetAttackSpeed();
		}
		PlayAnimMontage(m_BaseAttackAnim, AttackSpeed, *sectionSkillName);
	}
	else
	{
		PlayAnimMontage(m_BaseAttackAnim, 1 * GetAttackSpeed(), SectionName);
	}


	float AnimMongLen = m_BaseAttackAnim->GetSectionLength(
		sectionSkillName ? m_BaseAttackAnim->GetSectionIndex(*sectionSkillName) : (int)DmgType) / GetAttackSpeed();

	if (AnimMongLen > maxCd && !bUseMagic)
	{
		AnimMongLen = maxCd;
	}

	currentCd = AnimMongLen - 0.1f; //-0.1f;
	//
	GetWorldTimerManager().ClearTimer(m_AttackTimer);
	//
	m_Movement->SetMoveSpeedRatio(0.1f);

	GetWorldTimerManager().SetTimer(m_AttackTimer, this, &APlayerDiabloCharacter::ApplyMoveSpeedToOrigin,
	                                AnimMongLen, false);

	return AnimMongLen;
}

void APlayerDiabloCharacter::TakeDmg(BigInt amount, AUnitPawn* attacker, EDamagePopup pp)
{
	if (Cast<AOtherPlayerPawn>(attacker))
	{
		UDiabloGameInstance::Get->m_PVPManager->AddOtherPlayerTotalDamage(amount);
	}
}

float APlayerDiabloCharacter::TryAttack()
{
	if (UDiabloGameInstance::Get->m_PlayerUpgradeManager->IsSkillCasting())
	{
		return 1.f;
	}


	if (m_BaseAttackAnim && m_fAttackCD < 0.f)
	{
		PlayAttackMontage(m_fAttackCD, m_fAttackCDConstant);
	}

	return 1.f;
}

bool APlayerDiabloCharacter::GetDmg(BigInt& outDmg, EDamagePopup& pp)
{
	EDamageType Type;
	if (!m_QueDmgType.Dequeue(Type))
	{
		return false;
	}

	pp = EDamagePopup::NormalRight;

	outDmg = m_PlUpgradeManager->GetAtkUp(EAttackType::BaseAttack).m_Value;

	if (IsBuff01Available())
	{
		outDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(outDmg, m_bnAdditionalSkillDmg);
	}

	if(UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce02).Level>0)
	{
		outDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(outDmg,UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce02).m_Value);
	}
	
	if (UDiabloGameInstance::Get->m_ShopManager->GetPackagePurchased(0))
	{
		int MultipleFactor = 5;

		if (UDiabloGameInstance::Get->m_ShopManager->GetPackagePurchased(1))
		{
			MultipleFactor = 10;

			if (UDiabloGameInstance::Get->m_ShopManager->GetPackagePurchased(2))
			{
				MultipleFactor = 30;

				if (UDiabloGameInstance::Get->m_ShopManager->GetPackagePurchased(3))
				{
					MultipleFactor = 100;

					if (UDiabloGameInstance::Get->m_ShopManager->GetPackagePurchased(4))
					{
						MultipleFactor = 1000;
					}
				}
			}
		}

		outDmg.Multiply(MultipleFactor);
	}

	outDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(outDmg, m_EquipManager->GetCurrentWeapon().m_Value);

	int Rand = FMath::RandRange(90, 110);

	outDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(outDmg, Rand);

	//150
	if (Type == EDamageType::Critical01) //치명타
	{
		BigInt CDmg01 = m_PlUpgradeManager->GetAtkUp(EAttackType::CriticalDmg).m_Value; //백기준으로 해야함,1.5배는  1

		outDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(outDmg, CDmg01);

		if (UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce03).Level > 0)
		{
			outDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(
				outDmg, UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce03).m_Value);
		}

		pp = EDamagePopup::CritcalRight;
	}
	else if (Type == EDamageType::Critical02) //슈퍼치명타
	{
		BigInt CDmg01 = m_PlUpgradeManager->GetAtkUp(EAttackType::CriticalDmg).m_Value; //백기준으로 해야함,1.5배는  1

		outDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(outDmg, CDmg01);

		if (UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce03).Level > 0)
		{
			outDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(
				outDmg, UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce03).m_Value);
		}

		BigInt SDmg02 = m_PlUpgradeManager->GetAtkUp(EAttackType::SuperCriticalDmg).m_Value; //백기준으로 해야함,1.5배는  1

		outDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(outDmg, SDmg02);

		if (UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce04).Level > 0)
		{
			outDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(
				outDmg, UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce04).m_Value);
		}

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
	return m_fBuff01DeltaCount < m_fBuff01MaxTime;
}

void APlayerDiabloCharacter::EndBuff01()
{
	m_fBuff01MaxTime = 0;

	m_fBuff01DeltaCount = 0;

	m_bnAdditionalSkillDmg = 0;

	PRINTF("Buff01 End");
}

void APlayerDiabloCharacter::StartBuff02(float sec)
{
	m_fBuff02MaxTime = sec;

	m_fBuff02DeltaCount = 0;
	//공격속도 증가
	int Value = m_PlUpgradeManager->GetSkillUp(ESkillType::WindBlade).m_Value.ToInt();
	PRINTF("Value:%d", Value);
	m_fAdditionalAttackSpeed = Value / 100.f; //150%->1.5배

	PRINTF("Buff02 Start");
}

bool APlayerDiabloCharacter::IsBuff02Available()
{
	return m_fBuff02DeltaCount < m_fBuff02MaxTime;
}

void APlayerDiabloCharacter::EndBuff02()
{
	m_fBuff02MaxTime = 0;

	m_fBuff02DeltaCount = 0;

	m_fAdditionalAttackSpeed = 0;

	PRINTF("Buff02 End");
}

void APlayerDiabloCharacter::ApplyDamageToTarget(const BigInt* additionalDmg)
{
	if (GetFocusedTarget())
	{
		BigInt FinalDmg;

		EDamagePopup Popup = EDamagePopup::NormalRight;

		if (!GetDmg(FinalDmg, Popup))
		{
			return;
		}

		if (additionalDmg)
		{
			FinalDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(FinalDmg, *additionalDmg);
		}

		ApplyDamage(GetFocusedTarget(), FinalDmg, Popup);

		m_QueDmgType.Empty();
	}
}


void APlayerDiabloCharacter::ApplyDamageToTargets(TArray<FHitResult>& aryTargets, const BigInt* additionalDmg)
{
	BigInt FinalDmg;

	EDamagePopup Popup = EDamagePopup::NormalRight;

	if (!GetDmg(FinalDmg, Popup))
	{
		return;
	}

	if (additionalDmg)
	{
		FinalDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(FinalDmg, *additionalDmg);
	}

	for (auto& Mob : aryTargets)
	{
		AUnitPawn* Pawn = Cast<AUnitPawn>(Mob.GetActor());

		if (Pawn)
		{
			ApplyDamage(Pawn, FinalDmg, Popup);
		}
	}
	m_QueDmgType.Empty();
}

void APlayerDiabloCharacter::ApplyDamage(AUnitPawn* target, const BigInt& finalDmg, EDamagePopup& pp)
{
	target->TakeDmg(finalDmg, this, pp);
}

void APlayerDiabloCharacter::ApplyMoveSpeedToOrigin()
{
	m_Movement->SetMoveSpeedRatio(1.f);
}


void APlayerDiabloCharacter::SetManualMoveLocation(FVector goalLocation)
{
	m_TickFSM->SetManualMove(goalLocation);
}

void APlayerDiabloCharacter::GainRagePoint()
{
	float GainRage = m_fGainRagePer;

	if(UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce07).Level>0)
	{
		GainRage *= UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce07).m_fFloatValue;
	}

	m_fCurrentRage += GainRage;

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

	if (!m_bIsManualMove && !bIsMoveInputZero)
	{
		m_bIsManualMove = true;
		m_Movement->m_bUseRVO = false;
		GetMovementComponent()->StopMovementImmediately();
		m_TickFSM->ForceSetStateIdle();

		ApplyMoveSpeedToOrigin();
		FocusTarget(nullptr);
	}
	else if (bIsMoveInputZero && m_bUseFSM)
	{
		if (m_bIsManualMove)
		{
			m_TickFSM->ResetStartPosition(GetActorLocation());
		}
		m_bIsManualMove = false;
		m_Movement->m_bUseRVO = true;
		m_TickFSM->TickFSM();
	}

	if(UDiabloGameInstance::Get->m_ShopManager->GetUseAutoSkill())
	{
		FSkillSpec* WantUseSkill =m_AutoSkillUse->GetUsableSkill(this,UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetAryEquippedSkill(),m_fAttackCD / 1.f);

		if (WantUseSkill)
		{
			UDiabloGameInstance::Get->m_PlayerUpgradeManager->UseSkill(WantUseSkill->m_nIndex);
		}
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

void APlayerDiabloCharacter::ShowNameCard(const FString& name)
{
	m_NameCard->SetVisibility(true);
	m_NameCard->SetFloatingText(FText::FromString(name));
}

