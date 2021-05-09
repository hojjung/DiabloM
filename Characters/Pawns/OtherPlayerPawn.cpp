// My First Hack n Slash


#include "OtherPlayerPawn.h"

#include "Animation/AnimSequence.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/AssetManager.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Managers/DiabloGameInstance.h"
#include "Characters/Logic/AutoSkillUse.h"

AOtherPlayerPawn::AOtherPlayerPawn(const FObjectInitializer& objInit): Super(objInit)
{
	m_CurrentCastingSkill = nullptr;

	PrimaryActorTick.bCanEverTick = true;

	m_bUseFSM = false;

	static ConstructorHelpers::FObjectFinder<UAnimSequence> FoundAnim(TEXT(
		"AnimSequence'/Game/AnimationBlueprint/Aurora/29_Frank_ActionRPG_Sword_Attack03.29_Frank_ActionRPG_Sword_Attack03'"));

	m_AnimSeq = FoundAnim.Object;
	//
	m_SkBody->bCastDynamicShadow = true;
	m_SkBody->CastShadow = true;
	m_SkBody->bReceiveMobileCSMShadows = false;
	m_SkBody->SetRelativeLocation(FVector(0, 0, -80.f));
	m_SkBody->SetRelativeRotation(FRotator(0, -90.f, 0.f));
	//init skMesh
	//
	m_Spring = CreateDefaultSubobject<USpringArmComponent>("Spring");
	m_Spring->SetupAttachment(RootComponent);
	m_Spring->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	m_Spring->SetRelativeLocation(FVector(0.f, 0.f, 30.f)); //30
	m_Spring->TargetArmLength = 300.f;

	m_Capture = CreateDefaultSubobject<USceneCaptureComponent2D>("Capture2D");
	m_Capture->SetupAttachment(m_Spring);
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> FoundTexture(
		TEXT("TextureRenderTarget2D'/Game/03_VisualEffect/T_OtherPlayer.T_OtherPlayer'"));
	m_CaptureTexture = FoundTexture.Object;
	m_Capture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	m_Capture->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR; //SCS_SceneColorHDR
	m_Capture->SetTickableWhenPaused(true);
	m_Capture->ProjectionType = ECameraProjectionMode::Orthographic;
	m_Capture->OrthoWidth = 200.f;
	//m_Capture->FOVAngle = 35.f;
	m_Capture->TextureTarget = m_CaptureTexture;
	//
	m_Capture->TextureTarget->TargetGamma = 1;
	//FReadSurfaceDataFlags ReadSurfaceDataFlags;
	//ReadSurfaceDataFlags.SetLinearToGamma(false);
	//RenderTargetResource->ReadPixels(Image, ReadSurfaceDataFlags);

	m_PetComp = CreateDefaultSubobject<UChildActorComponent>("Child01");
	m_PetComp->SetupAttachment(RootComponent);
	m_PetComp->SetRelativeLocation(FVector(0, 90, 150));
	m_PetComp->SetRelativeRotation(FRotator(0, -90, 0));
	//
	m_Capsule->SetCapsuleSize(55, 88);

	m_Movement->SetRVOAvoidanceWeight(1);

	m_Movement->m_RotateSpeed = FRotator(0.f, 650.f, 0.f);

	m_Movement->SetMoveSpeed(630.f);

	m_AryTargetingObjectType.Reset();
	m_AryTargetingObjectType.Add(EObjectTypeQuery::ObjectTypeQuery3);
	//
	m_fMaxRage = 100;
	m_fGainRagePer = 3;
	m_fCurrentRage = 0;

	m_Movement->m_bUseRVO = false;
	//
	m_NameCard = CreateDefaultSubobject<UFloatingTextWidgetComponent>("NameCard");
	m_NameCard->SetupAttachment(RootComponent);
	static ConstructorHelpers::FClassFinder<UUserWidget> FoundW(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/CommonElement/WB_TextOtherPlayerName.WB_TextOtherPlayerName_C'"));
	m_NameCard->SetWidgetClass(FoundW.Class);
	m_NameCard->SetRelativeLocation(FVector(0, 0, 100));
	m_NameCard->SetVisibility(false);
}

void AOtherPlayerPawn::UpdateRage()
{
	m_OnRageChanged.Broadcast(m_fCurrentRage, m_fMaxRage);
}

void AOtherPlayerPawn::SetPVPPlayerPawn(UPlayFabJsonObject* statObj, UPlayFabJsonObject* skillObj,
                                        UPlayFabJsonObject* equipObj)
{
	m_bUseFSM = true;

	m_AutoSkillUse = NewObject<UAutoSkillUse>(this, UAutoSkillUse::StaticClass());

	m_Capture->ShowOnlyActors.Add(this);

	m_SkBody->SetForcedLOD(0);
	//
	m_AryUpgradeSpec.Init(FUpgradeSpec(), (int)EAttackType::Length);
	m_AryUpgradeSpec[(int)EAttackType::BaseAttack].m_UpgradeData = UPlayerUpgradeManager::StatUpgradeTable->FindRow<
		FUpgradeDataRow>(TEXT("AtkDmg01"), ""); //
	m_AryUpgradeSpec[(int)EAttackType::Critical].m_UpgradeData = UPlayerUpgradeManager::StatUpgradeTable->FindRow<
		FUpgradeDataRow>(TEXT("AtkCri01"), "");
	m_AryUpgradeSpec[(int)EAttackType::CriticalDmg].m_UpgradeData = UPlayerUpgradeManager::StatUpgradeTable->FindRow<
		FUpgradeDataRow>(TEXT("AtkCDmg01"), "");
	m_AryUpgradeSpec[(int)EAttackType::SuperCritical].m_UpgradeData = UPlayerUpgradeManager::StatUpgradeTable->FindRow<
		FUpgradeDataRow>(TEXT("AtkCri02"), "");
	m_AryUpgradeSpec[(int)EAttackType::SuperCriticalDmg].m_UpgradeData = UPlayerUpgradeManager::StatUpgradeTable->
		FindRow<FUpgradeDataRow>(TEXT("AtkCDmg02"), "");
	m_AryUpgradeSpec[(int)EAttackType::MagicBomb].m_UpgradeData = UPlayerUpgradeManager::StatUpgradeTable->FindRow<
		FUpgradeDataRow>(TEXT("AtkMagic01"), "");
	m_AryUpgradeSpec[(int)EAttackType::MagicBombDmg].m_UpgradeData = UPlayerUpgradeManager::StatUpgradeTable->FindRow<
		FUpgradeDataRow>(TEXT("AtkMDmg01"), "");
	m_AryUpgradeSpec[(int)EAttackType::SuperMagicBomb].m_UpgradeData = UPlayerUpgradeManager::StatUpgradeTable->FindRow<
		FUpgradeDataRow>(TEXT("AtkMagic02"), "");
	m_AryUpgradeSpec[(int)EAttackType::SuperMagicBombDmg].m_UpgradeData = UPlayerUpgradeManager::StatUpgradeTable->
		FindRow<FUpgradeDataRow>(TEXT("AtkMDmg02"), "");
	//
	//
	m_ArySkillSpec.Init(FSkillSpec(), (int)ESkillType::Length);
	m_ArySkillSpec[(int)ESkillType::DeathBlow].m_SkillData = UPlayerUpgradeManager::SkillUpgradeTable->FindRow<
		FSkillUpgradeDataRow>(TEXT("Skill01"), "");
	m_ArySkillSpec[(int)ESkillType::MagicBlade].m_SkillData = UPlayerUpgradeManager::SkillUpgradeTable->FindRow<
		FSkillUpgradeDataRow>(TEXT("Skill02"), "");
	m_ArySkillSpec[(int)ESkillType::WhirlWind].m_SkillData = UPlayerUpgradeManager::SkillUpgradeTable->FindRow<
		FSkillUpgradeDataRow>(TEXT("Skill03"), "");
	m_ArySkillSpec[(int)ESkillType::EarthQuake].m_SkillData = UPlayerUpgradeManager::SkillUpgradeTable->FindRow<
		FSkillUpgradeDataRow>(TEXT("Skill04"), "");
	m_ArySkillSpec[(int)ESkillType::WindBlade].m_SkillData = UPlayerUpgradeManager::SkillUpgradeTable->FindRow<
		FSkillUpgradeDataRow>(TEXT("Skill05"), "");
	//

	//
	m_TickFSM = NewObject<UFSMTick>(this, UFSMTick::StaticClass());
	m_TickFSM->Init(this);
	//
	ShowMeshWithTick();

	auto* SkinData = UDiabloGameInstance::Get->m_EquipManager->m_ArySkinsTable[equipObj->GetNumberField(
		TEXT("EquippedSkin"))];

	FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();

	if (m_SkinMeshHandle.Get())
	{
		m_SkinMeshHandle.Get()->ReleaseHandle();
	}

	if (m_AnimHandle.Get())
	{
		m_AnimHandle.Get()->ReleaseHandle();
	}

	USkeletalMesh* LoadedMesh = StreamableManager.LoadSynchronous(SkinData->m_PlayerSkinSoft, true, &m_SkinMeshHandle);

	m_SkBody->SetSkeletalMesh(LoadedMesh);
	m_SkBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	TSubclassOf<UAnimInstance> LoadedAnimBP = StreamableManager.LoadSynchronous(SkinData->m_AnimBP, true, &m_AnimHandle);

	m_SkBody->SetAnimInstanceClass(LoadedAnimBP);
	//
	m_WeaponSpec = FWeaponSpec();
	m_WeaponSpec.m_EquipData = UDiabloGameInstance::Get->m_EquipManager->m_AryWeaponTable[equipObj->GetNumberField(
		TEXT("EquippedWeapon"))];
	m_WeaponSpec.SetLevel(equipObj->GetNumberField(TEXT("EquippedWeaponLevel")));

	if (m_WeaponSpec.m_EquipData)
	{
		if (m_WeaponActor)
		{
			m_Capture->ShowOnlyActors.Remove(m_WeaponActor);
			FDetachmentTransformRules Rule(EDetachmentRule::KeepWorld, false);
			m_WeaponActor->DetachFromActor(Rule);
			m_WeaponActor->Destroy();
		}

		if (m_WeaponHandle.Get())
		{
			m_WeaponHandle.Get()->ReleaseHandle();
		}

		auto LoadedEquipActor = StreamableManager.LoadSynchronous(m_WeaponSpec.m_EquipData->m_ClassVisualActor, true, &m_WeaponHandle);


		FActorSpawnParameters Param;

		Param.bNoFail = true;

		m_WeaponActor = GetWorld()->SpawnActor<AEquipmentActor>(LoadedEquipActor, Param);

		FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
		                               EAttachmentRule::KeepRelative, false);

		m_WeaponActor->AttachToComponent(m_SkBody, Rule, "RightHandBottom");

		m_Capture->ShowOnlyActors.Add(m_WeaponActor);
	}
	//
	m_PetSpec = FPetSpec();

	int EquipPetIndex = equipObj->GetNumberField(TEXT("EquippedPet"));

	if (EquipPetIndex >= 0)
	{
		m_PetSpec.m_PetData = UDiabloGameInstance::Get->m_EquipManager->m_AryPetTable[EquipPetIndex];
		m_PetSpec.SetLevel(equipObj->GetNumberField(TEXT("EquippedPetLevel")));

		if (m_PetSpec.m_PetData)
		{
			if (m_PetComp->GetChildActor())
			{
				m_Capture->ShowOnlyActors.Remove(m_PetComp->GetChildActor());
			}

			if (m_WeaponHandle.Get())
			{
				m_WeaponHandle.Get()->ReleaseHandle();
			}

			auto LoadedPetActor = StreamableManager.LoadSynchronous(m_PetSpec.m_PetData->m_ClassPetSkin, true, &m_PetHandle);

			m_PetComp->SetChildActorClass(LoadedPetActor);

			m_Capture->ShowOnlyActors.Add(m_PetComp->GetChildActor());
		}
	}

	m_AryUpgradeSpec[(int)EAttackType::BaseAttack].SetLevel(statObj->GetNumberField(TEXT("BaseAttack")));
	m_AryUpgradeSpec[(int)EAttackType::Critical].SetLevel(statObj->GetNumberField(TEXT("Critical")));
	m_AryUpgradeSpec[(int)EAttackType::CriticalDmg].SetLevel(statObj->GetNumberField(TEXT("CriticalDmg")));
	m_AryUpgradeSpec[(int)EAttackType::SuperCritical].SetLevel(statObj->GetNumberField(TEXT("SuperCritical")));
	m_AryUpgradeSpec[(int)EAttackType::SuperCriticalDmg].SetLevel(statObj->GetNumberField(TEXT("SuperCriticalDmg")));
	m_AryUpgradeSpec[(int)EAttackType::MagicBomb].SetLevel(statObj->GetNumberField(TEXT("MagicBomb")));
	m_AryUpgradeSpec[(int)EAttackType::MagicBombDmg].SetLevel(statObj->GetNumberField(TEXT("MagicBombDmg")));
	m_AryUpgradeSpec[(int)EAttackType::SuperMagicBomb].SetLevel(statObj->GetNumberField(TEXT("SuperMagicBomb")));
	m_AryUpgradeSpec[(int)EAttackType::SuperMagicBombDmg].SetLevel(
		statObj->GetNumberField(TEXT("SuperMagicBombDmg")));
	//0428 0713 스킬도 여기에 받아와야함

	m_ArySkillSpec[(int)ESkillType::DeathBlow].InitSkillSpec(skillObj->GetNumberField(TEXT("DeathBlow")),
	                                                         skillObj->GetNumberField(TEXT("DeathBlowEquipSlot")));
	m_ArySkillSpec[(int)ESkillType::MagicBlade].InitSkillSpec(skillObj->GetNumberField(TEXT("MagicBlade")),
	                                                          skillObj->GetNumberField(TEXT("MagicBladeEquipSlot")));
	m_ArySkillSpec[(int)ESkillType::WhirlWind].InitSkillSpec(skillObj->GetNumberField(TEXT("WhirlWind")),
	                                                         skillObj->GetNumberField(TEXT("WhirlWindEquipSlot")));
	m_ArySkillSpec[(int)ESkillType::EarthQuake].InitSkillSpec(skillObj->GetNumberField(TEXT("EarthQuake")),
	                                                          skillObj->GetNumberField(TEXT("EarthQuakeEquipSlot")));
	m_ArySkillSpec[(int)ESkillType::WindBlade].InitSkillSpec(skillObj->GetNumberField(TEXT("WindBlade")),
	                                                         skillObj->GetNumberField(TEXT("WindBladeEquipSlot")));
	//
	m_fAttackSpeed = SkinData->m_fAttackSpeedMultiple;
	m_BaseAttackAnim = SkinData->m_BaseAttackAnim;

	m_fAttackCDConstant = 1.f / m_fAttackSpeed;
	//

	
	m_ArySkillEquipped.Init(nullptr, 4);
	for (auto& SkillSpec : m_ArySkillSpec)
	{
		if (SkillSpec.m_nIndex >= 0)
		{
			m_ArySkillEquipped[SkillSpec.m_nIndex] = &SkillSpec;
		}
	}

	UpdateRage();
}

void AOtherPlayerPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (m_SkinMeshHandle.Get())
	{
		m_SkinMeshHandle.Get()->ReleaseHandle();
	}
	
	if (m_WeaponHandle.Get())
	{
		m_WeaponHandle.Get()->ReleaseHandle();
	}

	if (m_PetHandle.Get())
	{
		m_PetHandle.Get()->ReleaseHandle();
	}

	if (m_AnimHandle.Get())
	{
		m_AnimHandle.Get()->ReleaseHandle();
	}

	if (m_WingHandle.Get())
	{
		m_WingHandle.Get()->ReleaseHandle();
	}
}

void AOtherPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_bUseFSM)
	{
		return;
	}

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

	m_TickFSM->TickFSM();

	if (!m_CurrentCastingSkill)
	{
		//m_fAttackCD
		FSkillSpec* WantUseSkill = m_AutoSkillUse->GetUsableSkill(this, m_ArySkillEquipped, m_fAttackCD / 1.f);

		if (WantUseSkill)
		{
			PRINTF("EnemyUseSkill:%s", *WantUseSkill->m_SkillData->m_SkillShowName.ToString());

			m_CurrentCastingSkill = WantUseSkill;

			m_fSkillCastTime = m_CurrentCastingSkill->UseSkill(this);

			SpendRagePoint(m_CurrentCastingSkill->m_SkillData->m_fRageCost);
		}
	}

	m_fSkillCastTime -= DeltaTime;

	if (m_fSkillCastTime <= 0.f)
	{
		m_fSkillCastTime = -1;
		m_CurrentCastingSkill = nullptr;
	}
}

void AOtherPlayerPawn::FocusTarget(AUnitPawn* target)
{
	Super::FocusTarget(target);

	AUnitPawn* Unit = Cast<AUnitPawn>(target);

	if (!Unit || target == m_FocusedEnemy) //캐스팅 실패하거나 이미 타겟팅 대상이면 스킵
	{
		return;
	}

	m_FocusedEnemy = Unit;
}

void AOtherPlayerPawn::StartBuff01(float sec)
{
	m_fBuff01MaxTime = sec;

	m_fBuff01DeltaCount = 0;
	//공격력 수치로 증가
	m_bnAdditionalSkillDmg = m_ArySkillSpec[(int)ESkillType::MagicBlade].m_Value;

	PRINTF("Buff01 Start");
}

bool AOtherPlayerPawn::IsBuff01Available()
{
	return m_fBuff01DeltaCount < m_fBuff01MaxTime;
}

void AOtherPlayerPawn::EndBuff01()
{
	m_fBuff01MaxTime = 0;

	m_fBuff01DeltaCount = 0;

	m_bnAdditionalSkillDmg = 0;

	PRINTF("Buff01 End");
}

void AOtherPlayerPawn::StartBuff02(float sec)
{
	m_fBuff02MaxTime = sec;

	m_fBuff02DeltaCount = 0;
	//공격속도 증가
	int Value = m_ArySkillSpec[(int)ESkillType::WindBlade].m_Value.ToInt();
	PRINTF("Value:%d", Value);
	m_fAdditionalAttackSpeed = Value / 100.f; //150%->1.5배

	PRINTF("Buff02 Start");
}

bool AOtherPlayerPawn::IsBuff02Available()
{
	return m_fBuff02DeltaCount < m_fBuff02MaxTime;
}

void AOtherPlayerPawn::EndBuff02()
{
	m_fBuff02MaxTime = 0;

	m_fBuff02DeltaCount = 0;

	m_fAdditionalAttackSpeed = 0;

	PRINTF("Buff02 End");
}

bool AOtherPlayerPawn::GetDmg(BigInt& outDmg, EDamagePopup& pp)
{
	EDamageType Type;
	if (!m_QueDmgType.Dequeue(Type))
	{
		return false;
	}

	pp = EDamagePopup::NormalRight;

	outDmg = m_AryUpgradeSpec[(int)EAttackType::BaseAttack].m_Value;

	if (IsBuff01Available())
	{
		outDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(outDmg, m_bnAdditionalSkillDmg);
	}

	// if(UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce02).Level>0)
	// {
	// 	outDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(outDmg,UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce02).m_Value);
	// }
	//
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

	outDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(outDmg, m_WeaponSpec.m_Value);

	int Rand = FMath::RandRange(90, 110);

	outDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(outDmg, Rand);

	//150
	if (Type == EDamageType::Critical01) //치명타
	{
		BigInt CDmg01 = m_AryUpgradeSpec[(int)EAttackType::CriticalDmg].m_Value; //백기준으로 해야함,1.5배는  1

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
		BigInt CDmg01 = m_AryUpgradeSpec[(int)EAttackType::CriticalDmg].m_Value; //백기준으로 해야함,1.5배는  1

		outDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(outDmg, CDmg01);

		if (UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce03).Level > 0)
		{
			outDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(
				outDmg, UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce03).m_Value);
		}

		BigInt SDmg02 = m_AryUpgradeSpec[(int)EAttackType::SuperCriticalDmg].m_Value; //백기준으로 해야함,1.5배는  1

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


void AOtherPlayerPawn::ShowMesh()
{
	m_SkBody->SetVisibility(true);

	m_SkBody->SetComponentTickEnabled(true);

	m_SkBody->SetAnimation(m_AnimSeq);

	PRINTF("ShowMesh");
}

void AOtherPlayerPawn::HideMesh()
{
	m_SkBody->SetVisibility(false);

	m_SkBody->SetComponentTickEnabled(false);

	PRINTF("HideMesh");
}

void AOtherPlayerPawn::ShowMeshWithTick()
{
	m_SkBody->SetVisibility(true);

	m_SkBody->SetComponentTickEnabled(true);

	m_Capture->SetComponentTickEnabled(true);

	PRINTF("ShowMeshWithTick");
}

void AOtherPlayerPawn::HideMeshWithTick()
{
	m_SkBody->SetVisibility(false);

	m_SkBody->SetComponentTickEnabled(false);

	m_Capture->SetComponentTickEnabled(false);

	PRINTF("HideMeshWithTick");
}

float AOtherPlayerPawn::TryAttack()
{
	if (m_CurrentCastingSkill)
	{
		return 1.f;
	}


	if (m_BaseAttackAnim && m_fAttackCD < 0.f)
	{
		PlayAttackMontage(m_fAttackCD, m_fAttackCDConstant);
	}

	return 1.f;
}

float AOtherPlayerPawn::PlayAttackMontage(float& currentCd, float maxCd, FName* sectionSkillName)
{
	FName SectionName = TEXT("Combo01");

	EDamageType DmgType = EDamageType::Base01;

	BigInt Cri01 = m_AryUpgradeSpec[(int)EAttackType::Critical].m_Value;

	BigInt CriPercent100 = FMath::RandRange(0.f, 100.f);

	if (Cri01 > 0 && CriPercent100 <= Cri01) //치명타 뜰때
	{
		SectionName = TEXT("Critical01");
		DmgType = EDamageType::Critical01;

		BigInt Cri2Percent100 = FMath::RandRange(0.f, 100.f);

		BigInt Cri02 = m_AryUpgradeSpec[(int)EAttackType::SuperCritical].m_Value;

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
	BigInt Magic01 = m_AryUpgradeSpec[(int)EAttackType::MagicBomb].m_Value;

	bool bUseMagic = false;

	BigInt MagicPercent100 = FMath::RandRange(0.f, 100.f);

	if (Magic01 > 0 && MagicPercent100 <= Magic01)
	{
		bUseMagic = true;
		SectionName = TEXT("MagicBomb01");
		DmgType = EDamageType::Magic01;

		BigInt Magic2Percent100 = FMath::RandRange(0.f, 100.f);

		BigInt Magic02 = m_AryUpgradeSpec[(int)EAttackType::SuperMagicBomb].m_Value;

		if (Magic2Percent100 <= Magic02)
		{
			SectionName = TEXT("MagicBomb02");
			DmgType = EDamageType::Magic02;
		}
	}

	PlayAnimMontage(m_BaseAttackAnim, 1 * GetAttackSpeed(), sectionSkillName ? *sectionSkillName : SectionName);

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

	GetWorldTimerManager().SetTimer(m_AttackTimer, this, &AOtherPlayerPawn::ApplyMoveSpeedToOrigin,
	                                AnimMongLen, false);

	return AnimMongLen;
}

void AOtherPlayerPawn::SetPetPositionForVisual()
{
	m_PetComp->SetRelativeLocation(FVector(0, 45, 75));
	m_PetComp->SetRelativeRotation(FRotator(0, 270, 0));
	m_PetComp->SetRelativeScale3D(FVector(0.5f));
}

void AOtherPlayerPawn::SetPetPositionForBattle()
{
	m_PetComp->SetRelativeLocation(FVector(0, 90, 150));
	m_PetComp->SetRelativeRotation(FRotator(0, -90, 0));
}

void AOtherPlayerPawn::TakeDmg(BigInt amount, AUnitPawn* attacker, EDamagePopup pp)
{
	if (Cast<APlayerDiabloCharacter>(attacker))
	{
		UDiabloGameInstance::Get->m_PVPManager->AddPlayerTotalDamage(amount);
	}
}

float AOtherPlayerPawn::PlaySkillMontageSection(FName& nameID, int nSectionIndex, float& currentCD, float maxCD)
{
	float Len = PlayAttackMontage(currentCD, maxCD, &nameID);

	currentCD = maxCD;

	return Len;
}

void AOtherPlayerPawn::ApplyDamageToTarget(const BigInt* additionalDmg)
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

void AOtherPlayerPawn::ApplyDamageToTargets(TArray<FHitResult>& aryTargets, const BigInt* additionalDmg)
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

void AOtherPlayerPawn::ApplyDamage(AUnitPawn* target, const BigInt& finalDmg, EDamagePopup& pp)
{
	target->TakeDmg(finalDmg, this, pp);
}

void AOtherPlayerPawn::ShowNameCard(const FString& name)
{
	m_NameCard->SetVisibility(true);
	m_NameCard->SetFloatingText(FText::FromString(name));
}

void AOtherPlayerPawn::ApplyMoveSpeedToOrigin()
{
	m_Movement->SetMoveSpeedRatio(1.f);
}

void AOtherPlayerPawn::GainRagePoint()
{
	float GainRage = m_fGainRagePer;

	// if(UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce07).Level>0)
	// {
	// 	GainRage *= UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce07).m_fFloatValue;
	// }

	m_fCurrentRage += GainRage;

	m_fCurrentRage = FMath::Clamp(m_fCurrentRage, 0.f, m_fMaxRage);

	m_OnRageChanged.Broadcast(m_fCurrentRage, m_fMaxRage);
}

bool AOtherPlayerPawn::SpendRagePoint(float rage)
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

void AOtherPlayerPawn::TriggerSkill(const FName& name, TArray<FHitResult>* aryHits)
{
	bool IsAOE = aryHits;
	//m_QueDmgType.Empty();
	//
	if (name == "BaseAttack")
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
	else if (name == "MagicBomb01") //작은 범위 공격
	{
		BigInt SkillDmg = m_AryUpgradeSpec[(int)EAttackType::MagicBombDmg].m_Value;

		if (UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce05).Level > 0)
		{
			SkillDmg = UDiaBlueprintFunctionLibrary::MultiplePercent(
				SkillDmg, UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce05).m_Value);
		}

		ApplyDamageToTargets(*aryHits, &SkillDmg);
	}
	else if (name == "MagicBomb02") //작은 범위 공격
	{
		BigInt SkillDmg1 = m_AryUpgradeSpec[(int)EAttackType::MagicBombDmg].m_Value;
		BigInt SkillDmg2 = m_AryUpgradeSpec[(int)EAttackType::SuperMagicBombDmg].m_Value;
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
	else if (name == "Skill01") //작은 범위 공격
	{
		BigInt SkillDmg = m_ArySkillSpec[(int)ESkillType::DeathBlow].m_Value;

		ApplyDamageToTargets(*aryHits, &SkillDmg);
	}
	else if (name == "Skill02") //버프 공격
	{
		m_QueDmgType.Empty();
		StartBuff01(20);
	}
	else if (name == "Skill03") //휠윈드
	{
		BigInt SkillDmg = m_ArySkillSpec[(int)ESkillType::WhirlWind].m_Value;
		ApplyDamageToTargets(*aryHits, &SkillDmg);
	}
	else if (name == "Skill04") //데스블로우
	{
		BigInt SkillDmg = m_ArySkillSpec[(int)ESkillType::EarthQuake].m_Value;
		ApplyDamageToTargets(*aryHits, &SkillDmg);
	}
	else if (name == "Skill05") //버프 공속
	{
		m_QueDmgType.Empty();
		StartBuff02(17);
	}
}
