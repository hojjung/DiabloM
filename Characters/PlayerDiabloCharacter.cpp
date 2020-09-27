#include "PlayerDiabloCharacter.h"
#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Managers/DiabloGameInstance.h"
#include "Datas/CharacterDataTable.h"
#include "Characters/DiabloPlayerController.h"
#include "Camera/CameraDissolve.h"
#include "Managers/StartMap/PlayerCreateManager.h"
#include "Objs/Interfaces/Interactable.h"

APlayerDiabloCharacter::APlayerDiabloCharacter(const FObjectInitializer& objInit)
    : Super(objInit.SetDefaultSubobjectClass<UPlayerDiabloAttribute>("AttributeSet00"))
{
    m_DissolveCam = CreateDefaultSubobject<UCameraDissolve>("CamDissolve00");
    m_DissolveCam->SetupAttachment(RootComponent);
    m_DissolveCam->SetRelativeRotation(FRotator(-50.f, 0.f, 0.f));
    //
    m_TopCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera00");
    m_TopCamera->SetupAttachment(m_DissolveCam);

    m_FocusedInteractable = nullptr;

    m_fInteractRange = 300.f;
    //
    CreateSkMeshComponent(m_SkBody, &m_SkFace, "SkMesh01");
    CreateSkMeshComponent(m_SkBody, &m_SkHair, "SkMesh02");
    CreateSkMeshComponent(m_SkBody, &m_SkBelt, "SkMesh03");
    CreateSkMeshComponent(m_SkBody, &m_SkGlove, "SkMesh04");
    CreateSkMeshComponent(m_SkBody, &m_SkShoe, "SkMesh05");
    CreateSkMeshComponent(m_SkBody, &m_SkShoulderPad, "SkMesh06");
    CreateSkMeshComponent(m_SkBody, &m_SkHeadGear, "SkMesh07");
    CreateSkMeshComponent(m_SkBody, &m_SkShadow, "SkMesh08");

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundMesh1(
        TEXT("SkeletalMesh'/Game/Models/ModularCharacter/Meshes/ModularBodyParts/Cloth01SK.Cloth01SK'"));
    m_DefaultBodyMesh = FoundMesh1.Object;
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundMesh2(
        TEXT("SkeletalMesh'/Game/Models/ModularCharacter/Meshes/ModularBodyParts/Glove01SK.Glove01SK'"));
    m_DefaultGloveMesh = FoundMesh2.Object;
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundMesh3(
        TEXT("SkeletalMesh'/Game/Models/ModularCharacter/Meshes/ModularBodyParts/Shoe01SK.Shoe01SK'"));
    m_DefaultShoeMesh = FoundMesh3.Object;
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundMesh4(
        TEXT("SkeletalMesh'/Game/Models/ModularCharacter/Meshes/OneMeshCharacters/ApprenticeSK.ApprenticeSK'"));

    m_SkBody->bCastDynamicShadow = false;
    m_SkBody->CastShadow = false;
    m_SkBody->bReceiveMobileCSMShadows = false;

    m_SkShadow->SetSkeletalMesh(FoundMesh4.Object);
    m_SkShadow->bCastDynamicShadow = true;
    m_SkShadow->CastShadow = true;
    m_SkShadow->bCastHiddenShadow = true;
    m_SkShadow->SetVisibility(false);

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

    m_fAttackCoolDown = 1.0f;
}

void APlayerDiabloCharacter::LoadExp(const USaveCharacterStatus* loadedSaveData)
{
    m_fMaxExp = Cast<UPlayerDiabloAttribute>(m_AttributeSet)->GetMaxExpForLevelUp();
    m_fCurrentExp = loadedSaveData->m_fExp;
    float RemainExp = m_fMaxExp - m_fCurrentExp;
    m_OnRemainExpChanged.Broadcast(RemainExp);
}

void APlayerDiabloCharacter::SetLoadedData(const USaveCharacterStatus* loadedSaveData)
{
    //
    m_SkFace->SetMasterPoseComponent(m_SkBody);
    m_SkHair->SetMasterPoseComponent(m_SkBody);
    m_SkGlove->SetMasterPoseComponent(m_SkBody);
    m_SkShoe->SetMasterPoseComponent(m_SkBody);
    m_SkHeadGear->SetMasterPoseComponent(m_SkBody);
    m_SkShoulderPad->SetMasterPoseComponent(m_SkBody);
    m_SkBelt->SetMasterPoseComponent(m_SkBody);
    m_SkShadow->SetMasterPoseComponent(m_SkBody);
    //
    SetDefaultBodyMesh();
    SetDefaultGloveMesh();
    SetFullHairMesh();
    SetDefaultShoeMesh();
    //
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
    SetUnitStat(loadedSaveData->m_ClassName, m_nCharacterLevel);

    LoadExp(loadedSaveData);


    if (m_fMaxExp <= 0.f)
    {
        PRINTF("MaxExp Is 0 !");
    }
}

void APlayerDiabloCharacter::EquipMesh(const FItemData* meshItem, ESlotsEquipAry slotWant)
{
    switch (slotWant)
    {
    case ESlotsEquipAry::Head:
        if (meshItem)
        {
            m_SkHeadGear->SetSkeletalMesh(meshItem->m_SkEquipment);
            SetHalfHairMesh();
        }
        else
        {
            m_SkHeadGear->SetSkeletalMesh(nullptr);
            SetFullHairMesh();
        }
        break;
    case ESlotsEquipAry::Torso:
        if (meshItem)
        {
            m_SkBody->SetSkeletalMesh(meshItem->m_SkEquipment);
        }
        else
        {
            SetDefaultBodyMesh();
        }

        break;
    case ESlotsEquipAry::Waist:
        m_SkBelt->SetSkeletalMesh(meshItem ? meshItem->m_SkEquipment : nullptr);
        break;
    case ESlotsEquipAry::Leg:
        if (meshItem)
        {
            m_SkShoe->SetSkeletalMesh(meshItem->m_SkEquipment);
        }
        else
        {
            SetDefaultShoeMesh();
        }
        break;
    case ESlotsEquipAry::Hand:
        if (meshItem)
        {
            m_SkGlove->SetSkeletalMesh(meshItem->m_SkEquipment);
        }
        else
        {
            SetDefaultGloveMesh();
        }
        break;
    case ESlotsEquipAry::Shoulder:
        m_SkShoulderPad->SetSkeletalMesh(meshItem ? meshItem->m_SkEquipment : nullptr);
        break;
    case ESlotsEquipAry::WeaponRight:
        m_StRightWeapon->SetStaticMesh(meshItem ? meshItem->m_StEquipment : nullptr);
        break;
    case ESlotsEquipAry::WeaponLeft:
        m_StLeftWeapon->SetStaticMesh(meshItem ? meshItem->m_StEquipment : nullptr);
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


void APlayerDiabloCharacter::SetUnitStat(FName unitID, int level)
{
    PRINTF("CharacterLevel:%d", level);
    
    m_NameUnitID = unitID;
    const FPlayerEntityTable* const UnitData = GetGameInstance<UDiabloGameInstance>()->GetPlayerUnitPtr(m_NameUnitID);
    m_GEUnitStat = UnitData->m_DefaultStatTable;
    
    SetCharacterLevel(level);

    SetAttackSpeed(1.0f);
}

void APlayerDiabloCharacter::SetAnimStance(const FAnimStance* animStance)
{
    m_SkBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    m_SkBody->SetAnimInstanceClass(animStance->m_StanceAnimation);
}


void APlayerDiabloCharacter::BeginPlay()
{
    Super::BeginPlay();
    m_PlayerCon = Cast<ADiabloPlayerController>(GetController());
}

void APlayerDiabloCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void APlayerDiabloCharacter::ShowDamageNumber(const float local_damage_done, AUnitPawn* unit_pawn)
{
}

void APlayerDiabloCharacter::EarnExp(float expEarned)
{
    m_fCurrentExp += expEarned;

    PRINTF("ExpEarned:%f", expEarned);

    float OverflowExp = m_fMaxExp - m_fCurrentExp;

    if (OverflowExp <= 0.f)
    {
        if (!SetCharacterLevel(m_nCharacterLevel + 1))
        {
            m_OnRemainExpChanged.Broadcast(0.f);
            return;
        }

        m_fCurrentExp = 0.f;
        m_fMaxExp = Cast<UPlayerDiabloAttribute>(m_AttributeSet)->GetMaxExpForLevelUp();
        
        PRINTF("Next Exp Is: %f", m_fMaxExp);
        
        EarnExp(FMath::Abs(OverflowExp));
    }
    
    m_OnRemainExpChanged.Broadcast(m_fMaxExp - m_fCurrentExp);
}

bool APlayerDiabloCharacter::SetCharacterLevel(int NewLevel)
{
    if (NewLevel > MAXLEVEL || NewLevel <= 0)
    {
        return false;
    }
    
    PRINTF("LevelUp: %d -> %d", m_nCharacterLevel, NewLevel);
    RemoveStartupGameplayAbilities();
    m_nCharacterLevel = NewLevel;
    AddStartupGameplayAbilities();
    m_OnLevelChanged.Broadcast(m_nCharacterLevel);

    return true;
}

void APlayerDiabloCharacter::TryCheckInteractable()
{
    FVector TraceStart = m_SkBody->GetComponentLocation();
    FVector TraceEnd = TraceStart + GetCapsule()->GetForwardVector() * m_fInteractRange;

    FHitResult OutHit;
    TArray<AActor*> IgnoreActors; //like weapon or ally

    if (!UKismetSystemLibrary::SphereTraceSingle(
            GetWorld(),
            TraceStart, TraceEnd, 15.f,
            ETraceTypeQuery::TraceTypeQuery3, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHit, true)
        || !OutHit.GetActor())
    {
        m_FocusedInteractable.SetInterface(nullptr);
        m_FocusedInteractable.SetObject(nullptr);
        return;
    }


    IInteractable* FoundIntract = Cast<IInteractable>(OutHit.GetActor());

    if (!FoundIntract)
    {
        if (m_FocusedInteractable)
        {
            m_FocusedInteractable.SetInterface(nullptr);
            m_FocusedInteractable.SetObject(nullptr);
        }

        return;
    }

    if (!m_FocusedInteractable)
    {
        m_FocusedInteractable.SetInterface(FoundIntract);
        m_FocusedInteractable.SetObject(OutHit.GetActor());

        OnInteractFound();

        return;
    }

    if (m_FocusedInteractable != FoundIntract)
    {
        m_FocusedInteractable.SetInterface(FoundIntract);
        m_FocusedInteractable.SetObject(OutHit.GetActor());

        OnInteractFound();
    }
}

void APlayerDiabloCharacter::InteractWithTarget()
{
    if (!m_FocusedInteractable)
        return;

    m_FocusedInteractable->Interact(this);
    m_FocusedInteractable = nullptr;
}

void APlayerDiabloCharacter::OnInteractFound() //npc,item , its not monster
{
    PRINTF("Found Something");
}

void APlayerDiabloCharacter::AutoPlayTick()
{
}

void APlayerDiabloCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TryCheckInteractable();
}


void APlayerDiabloCharacter::AttackInput(float pressed)
{
    PRINTF("Attacking");
}


void APlayerDiabloCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    m_PlayerCon = Cast<ADiabloPlayerController>(GetController());
    InputComponent->BindAxis("MoveForward", this, &AUnitPawn::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AUnitPawn::MoveRight);
    PlayerInputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this,
                                     &APlayerDiabloCharacter::InteractWithTarget);
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

void APlayerDiabloCharacter::SetAttackSpeed(float get_attack_speed)
{
    Super::SetAttackSpeed(get_attack_speed);

    m_OnAttackPerSecChanged.Broadcast(1 / m_fAttackCoolDown);
}

