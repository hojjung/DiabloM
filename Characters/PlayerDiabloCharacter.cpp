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
	m_fBonusDamage = 1.f;
	m_DissolveCam = CreateDefaultSubobject<UCameraDissolve>("CamDissolve00");
	m_DissolveCam->SetupAttachment(RootComponent);
	m_DissolveCam->SetRelativeRotation(FRotator(-50.f, 0.f, 0.f));
	//
	m_TopCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera00");
	m_TopCamera->SetupAttachment(m_DissolveCam);

	m_fInteractRange = 300.f;
	//
	CreateSkMeshComponent(m_SkBody, &m_SkFace, "SkMesh01");
	CreateSkMeshComponent(m_SkBody, &m_SkHair, "SkMesh02");
	CreateSkMeshComponent(m_SkBody, &m_SkBelt, "SkMesh03");
	CreateSkMeshComponent(m_SkBody, &m_SkGlove, "SkMesh04");
	CreateSkMeshComponent(m_SkBody, &m_SkShoe, "SkMesh05");
	CreateSkMeshComponent(m_SkBody, &m_SkShoulderPad, "SkMesh06");
	CreateSkMeshComponent(m_SkBody, &m_SkHeadGear, "SkMesh07");

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundMesh1(
		TEXT("SkeletalMesh'/Game/Models/ModularCharacter/Meshes/ModularBodyParts/Cloth01SK.Cloth01SK'"));
	m_DefaultBodyMesh = FoundMesh1.Object;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundMesh2(
		TEXT("SkeletalMesh'/Game/Models/ModularCharacter/Meshes/ModularBodyParts/Glove01SK.Glove01SK'"));
	m_DefaultGloveMesh = FoundMesh2.Object;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundMesh3(
		TEXT("SkeletalMesh'/Game/Models/ModularCharacter/Meshes/ModularBodyParts/Shoe01SK.Shoe01SK'"));
	m_DefaultShoeMesh = FoundMesh3.Object;

	m_SkBody->bCastDynamicShadow = false;
	m_SkBody->CastShadow = false;
	m_SkBody->bReceiveMobileCSMShadows = false;

	m_StBackpack = CreateDefaultSubobject<UStaticMeshComponent>("StMeshBackpack");
	m_StBackpack->CastShadow = false;
	m_StBackpack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_StBackpack->SetupAttachment(m_SkBody, "Backpack");

	m_StRightWeapon = CreateDefaultSubobject<UStaticMeshComponent>("StMeshRightHand");
	m_StRightWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_StRightWeapon->SetupAttachment(m_SkBody, "RightWeaponShield");
	m_StRightWeapon->CastShadow = true;

	m_StLeftWeapon = CreateDefaultSubobject<UStaticMeshComponent>("StMeshLeftHand");
	m_StLeftWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_StLeftWeapon->SetupAttachment(m_SkBody, "LeftWeaponShield");
	m_StLeftWeapon->CastShadow = true;

	m_fCurrentExp = 0.f;

	m_fMaxExp = 0.f;

	m_bIsDead = false;
}


void APlayerDiabloCharacter::Init()
{
	m_PlayerCon = Cast<ADiabloPlayerController>(GetController());
	m_AryIgnoreActor.Add(this);
	m_AryIgnoreActor.Add(m_PlayerCon);
	//
	m_FocusRenderer = NewObject<USkeletalMeshComponent>(this, USkeletalMeshComponent::StaticClass());
	m_FocusRenderer->RegisterComponent();
	m_FocusRenderer->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	m_FocusRenderer->SetHiddenInGame(true);
	//
	m_PlayerSense = NewObject<UPlayerSensing>(this, UPlayerSensing::StaticClass());
	m_PlayerSense->InitSense(this);
	m_PlayerSense->OnSeePawn.BindUObject(this, &APlayerDiabloCharacter::OnSeeTarget);
	m_PlayerSense->OnCantSeePawn.BindUObject(this, &APlayerDiabloCharacter::OnCantSeeTarget);
	m_PlayerSense->OnSeePawnBlocked.BindUObject(this, &APlayerDiabloCharacter::OnCanSeeTargetBlock);
	//
	m_SkFace->SetMasterPoseComponent(m_SkBody);
	m_SkHair->SetMasterPoseComponent(m_SkBody);
	m_SkGlove->SetMasterPoseComponent(m_SkBody);
	m_SkShoe->SetMasterPoseComponent(m_SkBody);
	m_SkHeadGear->SetMasterPoseComponent(m_SkBody);
	m_SkShoulderPad->SetMasterPoseComponent(m_SkBody);
	m_SkBelt->SetMasterPoseComponent(m_SkBody);
	//
	SetDefaultBodyMesh();
	SetDefaultGloveMesh();
	SetFullHairMesh();
	SetDefaultShoeMesh();
	//
	m_DissolveCam->Init(m_TopCamera);
	//
	m_PlayerAutoPlay=NewObject<UPlayerAutoPlayFSM>(this,UPlayerAutoPlayFSM::StaticClass());
	m_PlayerAutoPlay->Init(this);
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

	m_SkFace->SetSkeletalMesh(UPlayerCreateManager::Get->GetFace(loadedSaveData->m_IndexFace));
	m_DefaultFullHairMesh = UPlayerCreateManager::Get->GetHair(loadedSaveData->m_IndexHair, false);
	m_DefaultHalfHairMesh = UPlayerCreateManager::Get->GetHair(loadedSaveData->m_IndexHair, true);
	//
	m_SkHair->SetSkeletalMesh(m_DefaultFullHairMesh); //later equipment will doit
	m_TextUnitName = FText::FromString(loadedSaveData->m_TextName);
	//
	m_PlayerEntityData = UCharacterDataTable::GetPlayerEntityPtr(loadedSaveData->m_ClassName);

	m_GEUnitStat = m_PlayerEntityData->m_DefaultStatTable;

	m_DeathMontage = m_PlayerEntityData->m_DeathMontage;
	m_StunMontage = m_PlayerEntityData->m_StunMontage;
	m_TookHitMontage = m_PlayerEntityData->m_TookHitMontage;


	SetCharacterLevel(m_nCharacterLevel);
	LoadExp(loadedSaveData);
	SetGold(loadedSaveData->m_fGold);

	GrantHpRegenAbility();
	GrantHpPotionAbility();
	GrantResourceRegenAbility();
	GrantPortalAbility();
}

void APlayerDiabloCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (m_bIsManualInit)
	{
		Init();
	}
}

void APlayerDiabloCharacter::LoadExp(const USaveCharacterStatus* loadedSaveData)
{
	m_fMaxExp = GetPlayerAttribute()->GetMaxExpForLevelUp();
	m_fCurrentExp = loadedSaveData->m_fExp;
	float RemainExp = m_fMaxExp - m_fCurrentExp;
	m_OnRemainExpChanged.Broadcast(RemainExp);
	m_OnExpGaugeChanged.Broadcast(m_fCurrentExp / m_fMaxExp);
}

void APlayerDiabloCharacter::EquipMesh(const FItemInstance* meshItem, ESlotsEquipAry slotWant)
{
	TSubclassOf<AWeapon> ItemBP = nullptr;
	switch (slotWant)
	{
	case ESlotsEquipAry::Head:
		if (meshItem->m_ItemData)
		{
			m_SkHeadGear->SetSkeletalMesh(meshItem->m_ItemData->m_SkEquipment);
			SetHalfHairMesh();
		}
		else
		{
			m_SkHeadGear->SetSkeletalMesh(nullptr);
			SetFullHairMesh();
		}
		break;
	case ESlotsEquipAry::Torso:
		if (meshItem->m_ItemData)
		{
			m_SkBody->SetSkeletalMesh(meshItem->m_ItemData->m_SkEquipment);
		}
		else
		{
			SetDefaultBodyMesh();
		}

		break;
	case ESlotsEquipAry::Waist:
		m_SkBelt->SetSkeletalMesh(meshItem->m_ItemData ? meshItem->m_ItemData->m_SkEquipment : nullptr);
		break;
	case ESlotsEquipAry::Leg:
		if (meshItem->m_ItemData)
		{
			m_SkShoe->SetSkeletalMesh(meshItem->m_ItemData->m_SkEquipment);
		}
		else
		{
			SetDefaultShoeMesh();
		}
		break;
	case ESlotsEquipAry::Hand:
		if (meshItem->m_ItemData)
		{
			m_SkGlove->SetSkeletalMesh(meshItem->m_ItemData->m_SkEquipment);
		}
		else
		{
			SetDefaultGloveMesh();
		}
		break;
	case ESlotsEquipAry::Shoulder:
		m_SkShoulderPad->SetSkeletalMesh(meshItem->m_ItemData ? meshItem->m_ItemData->m_SkEquipment : nullptr);
		break;
	case ESlotsEquipAry::WeaponRight:

		if (CreateItemActor(meshItem, &m_RightWeapon, &m_StRightWeapon))
		{
			return;
		}

		m_StRightWeapon->SetStaticMesh(meshItem->m_ItemData ? meshItem->m_ItemData->m_StEquipment : nullptr);
		//"RightWeaponShield"
		break;
	case ESlotsEquipAry::WeaponLeft:

		if (CreateItemActor(meshItem, &m_LeftWeapon, &m_StLeftWeapon))
		{
			return;
		}

		m_StLeftWeapon->SetStaticMesh(meshItem->m_ItemData ? meshItem->m_ItemData->m_StEquipment : nullptr);

		break;
	default:
		;
	}
}

void APlayerDiabloCharacter::RemoveAllEffect()
{
	FGameplayEffectQuery Query;
	Query.EffectSource = this;
	GetDiaAbilitySystem()->RemoveActiveEffects(Query);

	PRINTF("RemoveAllEffect");
}


void APlayerDiabloCharacter::GrantBaseAttackAbility()
{
	m_BaseAttackHandle = GetDiaAbilitySystem()->GiveAbility(
		FGameplayAbilitySpec(m_PlayerBaseAttack, GetCharacterLevel(),
		                     static_cast<int32>(m_PlayerBaseAttack.GetDefaultObject()->m_AbilityInputID),
		                     this));
}

void APlayerDiabloCharacter::SetBaseAttackAbility(const FAnimStance* animStance)
{
	if (m_BaseAttackHandle.IsValid())
	{
		GetDiaAbilitySystem()->ClearAbility(m_BaseAttackHandle);
		m_PlayerBaseAttack = nullptr;
	}

	if (IsValid(animStance->m_BaseAttackAbility))
	{
		m_PlayerBaseAttack = animStance->m_BaseAttackAbility;

		GrantBaseAttackAbility();
	}

	m_AlreadyHittenForIgnore.Reset();
}

void APlayerDiabloCharacter::SetBaseAttackData(float viewAngle, float viewRadius, float focusRange)
{
	m_PlayerSense->SetPeripheralVisionAngle(viewAngle);
	m_PlayerSense->SetViewRadius(viewRadius);
	m_PlayerSense->SetFocusRange(focusRange);
}

void APlayerDiabloCharacter::SetBonusDamage(float v)
{
	m_fBonusDamage = v;
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
	FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
	                                                           EAttachmentRule::SnapToTarget,
	                                                           EAttachmentRule::SnapToTarget, false);
	m_StRightWeapon->AttachToComponent(m_SkBody, Rule, "RightWeaponShield");
	m_StLeftWeapon->AttachToComponent(m_SkBody, Rule, "LeftWeaponShield");

	//should Seprated
	SetBaseAttackAbility(animStance);
	SetBaseAttackData(animStance->m_fViewAngle, animStance->m_fViewRadius, animStance->m_fFocusRange);
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
	}
	m_FocusOutlinePawn = Unit;

	m_FocusRenderer->SetHiddenInGame(false);
	m_FocusRenderer->AttachToComponent(Unit->GetBodyMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	m_FocusRenderer->SetSkeletalMesh(Unit->GetBodyMesh()->SkeletalMesh);

	for (int i = 0; i < m_FocusRenderer->GetMaterials().Num(); i++)
	{
		m_FocusRenderer->SetMaterial(i, m_OutLineMat);
	}

	m_FocusRenderer->SetMasterPoseComponent(Unit->GetBodyMesh(), true);
	PRINTF("ShowOutlineOnTarget");
}

void APlayerDiabloCharacter::HideOutlineOnTarget()
{
	m_FocusOutlinePawn = nullptr;

	m_FocusRenderer->SetHiddenInGame(true);
	m_FocusRenderer->SetSkeletalMesh(nullptr);
	m_FocusRenderer->GetMaterials().Reset();
	m_FocusRenderer->AttachToComponent(GetBodyMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	PRINTF("HideOutlineOnTarget");
}

void APlayerDiabloCharacter::FocusTarget(AUnitPawn* target)
{
	Super::FocusTarget(target);

	if (!target)
	{
		Cast<ADiabloPlayerController>(GetController())->HideFocusStatusWidget();
		m_FocusedEnemy = nullptr;
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

	Cast<ADiabloPlayerController>(GetController())->ShowFocusStatusWidget(Unit);

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
	if (m_bIsDead) //Sometime call manytime
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
	GetDiaAbilitySystem()->ClearAllAbilities();

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

			GetWorldTimerManager().SetTimer(TimerHandle_OnTimer, this, &APlayerDiabloCharacter::OnDeathAnimEnd,
			                                AnimLength,
			                                false);
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
	GrantBaseAttackAbility();
	GrantHpPotionAbility();
	GrantResourceRegenAbility();
	GrantPortalAbility();
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
	FocusTarget(nullptr);
}

void APlayerDiabloCharacter::EndAttack()
{
	Super::EndAttack();
	m_fBonusDamage = 1.f;
}

FVector APlayerDiabloCharacter::GetLastSeenLocation()
{
	return m_PlayerSense->m_LastSeenLocation;
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

void APlayerDiabloCharacter::DrinkPotion()
{
	GetDiaAbilitySystem()->TryActivateAbility(m_PotionHandle);
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

void APlayerDiabloCharacter::PlayColorEffect(const FLinearColor& colorWant)//애초에 사용된적이 없음
{
	FVector ColorV = UKismetMathLibrary::Conv_LinearColorToVector(colorWant);
	FName ColorParamName = "EffectColor";
	FName TimeParamName = "StartTime";
	float TimeSec =UGameplayStatics::GetTimeSeconds(GetWorld());

	m_SkBody->SetVectorParameterValueOnMaterials(ColorParamName, ColorV);
	m_SkBody->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);
	
	m_SkFace->SetVectorParameterValueOnMaterials(ColorParamName, ColorV);
	m_SkFace->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);

	m_SkHair->SetVectorParameterValueOnMaterials(ColorParamName, ColorV);
	m_SkHair->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);

	m_SkGlove->SetVectorParameterValueOnMaterials(ColorParamName, ColorV);
	m_SkGlove->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);

	m_SkShoe->SetVectorParameterValueOnMaterials(ColorParamName, ColorV);
	m_SkShoe->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);

	m_SkHeadGear->SetVectorParameterValueOnMaterials(ColorParamName, ColorV);
	m_SkHeadGear->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);

	m_SkShoulderPad->SetVectorParameterValueOnMaterials(ColorParamName, ColorV);
	m_SkShoulderPad->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);

	m_SkBelt->SetVectorParameterValueOnMaterials(ColorParamName, ColorV);
	m_SkBelt->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);

	m_StBackpack->SetVectorParameterValueOnMaterials(ColorParamName, ColorV);
	m_StBackpack->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);
	
	m_StRightWeapon->SetVectorParameterValueOnMaterials(ColorParamName, ColorV);
	m_StRightWeapon->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);
	
	m_StLeftWeapon->SetVectorParameterValueOnMaterials(ColorParamName, ColorV);
	m_StLeftWeapon->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);
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


void APlayerDiabloCharacter::SetFullHairMesh()
{
	m_SkHair->SetSkeletalMesh(m_DefaultFullHairMesh);
}

void APlayerDiabloCharacter::SetHalfHairMesh()
{
	m_SkHair->SetSkeletalMesh(m_DefaultHalfHairMesh);
}

void APlayerDiabloCharacter::SetDefaultBodyMesh()
{
	m_SkBody->SetSkeletalMesh(m_DefaultBodyMesh);
}

void APlayerDiabloCharacter::SetDefaultShoeMesh()
{
	m_SkShoe->SetSkeletalMesh(m_DefaultShoeMesh);
}

void APlayerDiabloCharacter::SetDefaultGloveMesh()
{
	m_SkGlove->SetSkeletalMesh(m_DefaultGloveMesh);
}


bool APlayerDiabloCharacter::CreateItemActor(const FItemInstance* itemInst, AWeapon** wantCachePointer,
                                             UStaticMeshComponent** attachRoot)
{
	if ((*wantCachePointer))
	{
		m_AryIgnoreActor.Remove((*wantCachePointer));
		(*wantCachePointer)->RemoveWeapon(this);
		(*wantCachePointer)->Destroy();
		(*wantCachePointer) = nullptr;
	}

	if (itemInst->IsEmpty())
	{
		return false;
	}

	auto ItemBP = itemInst->m_ItemData->m_ItemType.GetRow<FItemType>("")->m_EquipmentBP;

	if (!ItemBP)
	{
		return false;
	}

	(*attachRoot)->SetStaticMesh(nullptr);

	FActorSpawnParameters Params;

	Params.Template = Cast<AActor>(ItemBP->GetDefaultObject());

	FTransform Trans;

	AWeapon* Weapon = Cast<AWeapon>(GetWorld()->SpawnActor(ItemBP, &Trans, Params));

	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
	                                                            EAttachmentRule::SnapToTarget,
	                                                            EAttachmentRule::SnapToTarget, false);

	Weapon->InitWeapon(this, itemInst);

	Weapon->AttachToComponent((*attachRoot), Rules);

	(*wantCachePointer) = Weapon;

	m_AryIgnoreActor.Add((*wantCachePointer));

	return true;
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
