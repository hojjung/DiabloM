// My First Hack n Slash


#include "OtherPlayerPawn.h"

#include "Animation/AnimSequence.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/AssetManager.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Managers/DiabloGameInstance.h"

AOtherPlayerPawn::AOtherPlayerPawn(const FObjectInitializer& objInit): Super(objInit)
{
	PrimaryActorTick.bCanEverTick = true;

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
	m_PetComp->SetRelativeLocation(FVector(0, 45, 75));
	m_PetComp->SetRelativeRotation(FRotator(0, 90, 0));
	m_PetComp->SetRelativeScale3D(FVector(0.5f));
}

void AOtherPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	m_Capture->ShowOnlyActors.Add(this);

	m_SkBody->SetForcedLOD(0);
	//
	m_AryBaseAtkUpgrade.Init(FUpgradeSpec(), (int)EAttackType::Length);
	m_AryBaseAtkUpgrade[(int)EAttackType::BaseAttack].m_UpgradeData = UPlayerUpgradeManager::StatUpgradeTable->FindRow<FUpgradeDataRow>(
		TEXT("AtkDmg01"), ""); //
	m_AryBaseAtkUpgrade[(int)EAttackType::Critical].m_UpgradeData = UPlayerUpgradeManager::StatUpgradeTable->FindRow<FUpgradeDataRow>(
		TEXT("AtkCri01"), "");
	m_AryBaseAtkUpgrade[(int)EAttackType::CriticalDmg].m_UpgradeData = UPlayerUpgradeManager::StatUpgradeTable->FindRow<FUpgradeDataRow>(
		TEXT("AtkCDmg01"), "");
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperCritical].m_UpgradeData =UPlayerUpgradeManager:: StatUpgradeTable->FindRow<FUpgradeDataRow>(
		TEXT("AtkCri02"), "");
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperCriticalDmg].m_UpgradeData = UPlayerUpgradeManager::StatUpgradeTable->FindRow<FUpgradeDataRow>(
		TEXT("AtkCDmg02"), "");
	m_AryBaseAtkUpgrade[(int)EAttackType::MagicBomb].m_UpgradeData =UPlayerUpgradeManager:: StatUpgradeTable->FindRow<FUpgradeDataRow>(
		TEXT("AtkMagic01"), "");
	m_AryBaseAtkUpgrade[(int)EAttackType::MagicBombDmg].m_UpgradeData = UPlayerUpgradeManager::StatUpgradeTable->FindRow<FUpgradeDataRow>(
		TEXT("AtkMDmg01"), "");
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperMagicBomb].m_UpgradeData = UPlayerUpgradeManager::StatUpgradeTable->FindRow<FUpgradeDataRow>(
		TEXT("AtkMagic02"), "");
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperMagicBombDmg].m_UpgradeData =UPlayerUpgradeManager:: StatUpgradeTable->FindRow<FUpgradeDataRow>(
		TEXT("AtkMDmg02"), "");

	//
	
	//
	m_AryUpgradeSkill.Reserve(5);
	m_AryUpgradeSkill.Add(UPlayerUpgradeManager::SkillUpgradeTable->FindRow<FSkillUpgradeDataRow>(TEXT("Skill01"), ""));
	m_AryUpgradeSkill.Add(UPlayerUpgradeManager::SkillUpgradeTable->FindRow<FSkillUpgradeDataRow>(TEXT("Skill02"), ""));
	m_AryUpgradeSkill.Add(UPlayerUpgradeManager::SkillUpgradeTable->FindRow<FSkillUpgradeDataRow>(TEXT("Skill03"), ""));
	m_AryUpgradeSkill.Add(UPlayerUpgradeManager::SkillUpgradeTable->FindRow<FSkillUpgradeDataRow>(TEXT("Skill04"), ""));
	m_AryUpgradeSkill.Add(UPlayerUpgradeManager::SkillUpgradeTable->FindRow<FSkillUpgradeDataRow>(TEXT("Skill05"), ""));
	//
	m_AryEquippedSkillSpec.Init(FSkillSpec(),4);
}

void AOtherPlayerPawn::SetPVPPlayerPawn(UPlayFabJsonObject* statObj, UPlayFabJsonObject* equipObj)
{
	ShowMeshWithTick();
	
	auto* SkinData = UDiabloGameInstance::Get->m_EquipManager->m_ArySkinsTable[equipObj->GetNumberField(TEXT("EquippedSkin"))];

	FStreamableManager& StreamableManager =  UAssetManager::Get().GetStreamableManager();

	if(m_SkinMeshHandle.Get())
	{
		m_SkinMeshHandle.Get()->ReleaseHandle();
	}
	
	auto* LoadedMesh= StreamableManager.LoadSynchronous(SkinData->m_PlayerSkinSoft,true,&m_SkinMeshHandle);

	USkeletalMesh* MeshLoadd = Cast<USkeletalMesh>(LoadedMesh);
	//
	m_SkBody->SetSkeletalMesh(MeshLoadd);
	m_SkBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	m_SkBody->SetAnimInstanceClass(SkinData->m_AnimBP);
	//
	m_WeaponSpec = FWeaponSpec();
	m_WeaponSpec.m_EquipData =UDiabloGameInstance::Get->m_EquipManager->m_AryWeaponTable[equipObj->GetNumberField(TEXT("EquippedWeapon"))];
	m_WeaponSpec.SetLevel(equipObj->GetNumberField(TEXT("EquippedWeaponLevel")));
	//
	m_PetSpec = FPetSpec();
	
	int EquipPetIndex = equipObj->GetNumberField(TEXT("EquippedPet"));

	if(EquipPetIndex>=0)
	{
		m_PetSpec.m_PetData =UDiabloGameInstance::Get->m_EquipManager->m_AryPetTable[EquipPetIndex]; 
		m_PetSpec.SetLevel(equipObj->GetNumberField(TEXT("EquippedPetLevel")));	
	}
	
	m_AryBaseAtkUpgrade[(int)EAttackType::BaseAttack].SetLevel(statObj->GetNumberField(TEXT("BaseAttack")));
	m_AryBaseAtkUpgrade[(int)EAttackType::Critical].SetLevel(statObj->GetNumberField(TEXT("Critical")));
	m_AryBaseAtkUpgrade[(int)EAttackType::CriticalDmg].SetLevel(statObj->GetNumberField(TEXT("CriticalDmg")));
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperCritical].SetLevel(statObj->GetNumberField(TEXT("SuperCritical")));
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperCriticalDmg].SetLevel( statObj->GetNumberField(TEXT("SuperCriticalDmg")));
	m_AryBaseAtkUpgrade[(int)EAttackType::MagicBomb].SetLevel(statObj->GetNumberField(TEXT("MagicBomb")));
	m_AryBaseAtkUpgrade[(int)EAttackType::MagicBombDmg].SetLevel( statObj->GetNumberField(TEXT("MagicBombDmg")));
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperMagicBomb].SetLevel( statObj->GetNumberField(TEXT("SuperMagicBomb")));
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperMagicBombDmg].SetLevel( statObj->GetNumberField(TEXT("SuperMagicBombDmg")));
	//
	int EquipSkillIndex01 = equipObj->GetNumberField(TEXT("EquippedSkill01"));
	m_AryEquippedSkillSpec[0] = FSkillSpec();
	if(EquipSkillIndex01>=0)
	{
		m_AryEquippedSkillSpec[0].m_SkillData = m_AryUpgradeSkill[EquipSkillIndex01];
		m_AryEquippedSkillSpec[0].InitSkillSpec(equipObj->GetNumberField(TEXT("EquippedSkill01Level")),0);
	}
	//
	int EquipSkillIndex02 = equipObj->GetNumberField(TEXT("EquippedSkill02"));
	m_AryEquippedSkillSpec[1] = FSkillSpec();
	if(EquipSkillIndex02>=0)
	{
		m_AryEquippedSkillSpec[1].m_SkillData = m_AryUpgradeSkill[EquipSkillIndex02];
		m_AryEquippedSkillSpec[1].InitSkillSpec(equipObj->GetNumberField(TEXT("EquippedSkill02Level")),1);
	}
	//
	int EquipSkillIndex03 = equipObj->GetNumberField(TEXT("EquippedSkill03"));
	m_AryEquippedSkillSpec[2] = FSkillSpec();
	if(EquipSkillIndex03>=0)
	{
		m_AryEquippedSkillSpec[2].m_SkillData = m_AryUpgradeSkill[EquipSkillIndex03];
		m_AryEquippedSkillSpec[2].InitSkillSpec(equipObj->GetNumberField(TEXT("EquippedSkill03Level")),2);
	}
	//
	int EquipSkillIndex04 = equipObj->GetNumberField(TEXT("EquippedSkill04"));
	m_AryEquippedSkillSpec[3] = FSkillSpec();
	if(EquipSkillIndex04>=0)
	{
		m_AryEquippedSkillSpec[3].m_SkillData = m_AryUpgradeSkill[EquipSkillIndex04];
		m_AryEquippedSkillSpec[3].InitSkillSpec(equipObj->GetNumberField(TEXT("EquippedSkill04Level")),3);
	}
	//
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
