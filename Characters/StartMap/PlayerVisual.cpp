
#include "PlayerVisual.h"
#include "ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SkinnedMeshComponent.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/StartMap/PlayerCreateManager.h"
#include "Animation/AnimSequence.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
// Sets default values
APlayerVisual::APlayerVisual()
{
    SetCanBeDamaged(false);
    
    PrimaryActorTick.bCanEverTick = false;
    
    static ConstructorHelpers::FObjectFinder<UAnimSequence> FoundAnim(TEXT("AnimSequence'/Game/AnimationBlueprint/BareHand/Idle_noWeaponAnim.Idle_noWeaponAnim'"));
    
    m_AnimSeq = FoundAnim.Object;

    //init skMesh
    RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
    
    CreateSkMeshComponent(&m_MeshBody, "SkMeshRoot",RootComponent);
    
    m_MeshBody->SetRelativeLocation(FVector(0,0,-80.f));
    m_MeshBody->SetRelativeRotation(FRotator(0,90.f,0.f));
    
    CreateSkMeshComponent(&m_MeshFace, "SkMesh01",m_MeshBody);
    CreateSkMeshComponent(&m_MeshHair, "SkMesh02",m_MeshBody);
    CreateSkMeshComponent(&m_MeshBelt, "SkMesh03",m_MeshBody);
    CreateSkMeshComponent(&m_MeshGlove, "SkMesh04",m_MeshBody);
    CreateSkMeshComponent(&m_MeshShoe, "SkMesh05",m_MeshBody);
    CreateSkMeshComponent(&m_MeshShoulderPad, "SkMesh06",m_MeshBody);
    CreateSkMeshComponent(&m_MeshHeadGear, "SkMesh07",m_MeshBody);
    //init stMesh
    m_MeshBackpack = CreateDefaultSubobject<UStaticMeshComponent>("StMeshBackpack");
    m_MeshBackpack->CastShadow=false;
    m_MeshBackpack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    m_MeshBackpack->SetupAttachment(RootComponent, "Backpack");
    
    m_MeshRightHand = CreateDefaultSubobject<UStaticMeshComponent>("StMeshRightHand");
    m_MeshRightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    m_MeshRightHand->SetupAttachment(RootComponent, "RightWeaponShield");
    m_MeshRightHand->CastShadow=false;
    
    m_MeshLeftHand = CreateDefaultSubobject<UStaticMeshComponent>("StMeshLeftHand");
    m_MeshLeftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    m_MeshLeftHand->SetupAttachment(RootComponent, "LeftWeaponShield");
    m_MeshLeftHand->CastShadow=false;
    //init Anim	
    m_MeshBody->SetAnimationMode(EAnimationMode::AnimationSingleNode);
  
    //init MasterPose
    m_MeshFace->SetMasterPoseComponent(m_MeshBody);
    m_MeshHair->SetMasterPoseComponent(m_MeshBody);
    m_MeshGlove->SetMasterPoseComponent(m_MeshBody);
    m_MeshShoe->SetMasterPoseComponent(m_MeshBody);
    m_MeshHeadGear->SetMasterPoseComponent(m_MeshBody);
    m_MeshShoulderPad->SetMasterPoseComponent(m_MeshBody);
    m_MeshBelt->SetMasterPoseComponent(m_MeshBody);
    //
    m_MeshBody->SetAnimation(m_AnimSeq);
    //
    m_PlCreateManager = nullptr;
    //
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundMesh1(TEXT("SkeletalMesh'/Game/Models/ModularCharacter/Meshes/ModularBodyParts/Cloth01SK.Cloth01SK'"));
    m_DefaultBodyMesh = FoundMesh1.Object;
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundMesh2(TEXT("SkeletalMesh'/Game/Models/ModularCharacter/Meshes/ModularBodyParts/Glove01SK.Glove01SK'"));
    m_DefaultGloveMesh= FoundMesh2.Object;
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundMesh3(TEXT("SkeletalMesh'/Game/Models/ModularCharacter/Meshes/ModularBodyParts/Shoe01SK.Shoe01SK'"));
    m_DefaultShoeMesh= FoundMesh3.Object;

    //
    m_Spring = CreateDefaultSubobject<USpringArmComponent>("Spring");
    m_Spring->SetupAttachment(RootComponent);
    m_Spring->TargetArmLength=260.f;
    m_Spring->SetRelativeRotation(FRotator(-2.5f,0.f,0.f));

    m_Capture = CreateDefaultSubobject<USceneCaptureComponent2D>("Capture2D");

    m_Capture->FOVAngle = 45.f;
    
    m_Capture->SetupAttachment(m_Spring);
    m_Capture->SetRelativeLocation(FVector(0.f,0.f,20.f));
    //TextureRenderTarget2D'/Game/03_VisualEffect/T_PlayerVisual.T_PlayerVisual'
    static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> FoundTexture(TEXT("TextureRenderTarget2D'/Game/03_VisualEffect/T_PlayerVisual.T_PlayerVisual'"));
    m_Capture->TextureTarget = FoundTexture.Object;
    
    m_Capture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives;

    m_Capture->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;

    m_Capture->MaxViewDistanceOverride =1000.f;

    //SetTickableWhenPaused(true);

    m_Capture->SetTickableWhenPaused(true);
}

void APlayerVisual::CreateSkMeshComponent(USkeletalMeshComponent** refSkComp, FName keyName,USceneComponent* root)
{
    (*refSkComp) = CreateDefaultSubobject<USkeletalMeshComponent>(keyName);
    (*refSkComp)->bOwnerNoSee = false;
    (*refSkComp)->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
    (*refSkComp)->bCastDynamicShadow = false; //chanage for mobile
    (*refSkComp)->bAffectDynamicIndirectLighting = true;
    (*refSkComp)->PrimaryComponentTick.TickGroup = TG_PrePhysics;
    (*refSkComp)->SetupAttachment(root);
    (*refSkComp)->SetCollisionProfileName("CharacterMesh");
    (*refSkComp)->SetGenerateOverlapEvents(false);
    (*refSkComp)->SetCanEverAffectNavigation(false);
    (*refSkComp)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    (*refSkComp)->CastShadow=false;
    (*refSkComp)->SetTickableWhenPaused(true);
}

void APlayerVisual::BeginPlay()
{
    Super::BeginPlay();
    m_PlCreateManager = GetGameInstance<UDiabloGameInstance>()->GetPlCreateManager();
    m_PlCreateManager->m_OnVisualChange.AddUObject(this, &APlayerVisual::OnMeshVisualChanged);
    m_PlCreateManager->m_OnStartCreation.BindUObject(this, &APlayerVisual::ShowMesh);
    
    HideMesh();

    m_Capture->ShowOnlyActorComponents(this);
}

void APlayerVisual::SetBodyAnim()
{
    m_MeshBody->SetAnimation(m_AnimSeq);
    m_MeshBody->Play(true);//body change = need animation update
}

void APlayerVisual::OnMeshVisualChanged(const FCurrentCharData& charData)
{
    //만일 장비칸이 없다면 이대로면 빈칸은 없는 부위로 변함.
    //기본 메쉬 만들어놓고 장비가 있을때만 바꿔주고 없으면 기본메쉬로 변경해야함
    //
    m_MeshFace->SetSkeletalMesh(charData.m_CurrentFace);
    m_MeshHair->SetSkeletalMesh(charData.m_CurrentHair);
    m_MeshHeadGear->SetSkeletalMesh(charData.m_CurrentHelmet?charData.m_CurrentHelmet->m_SkEquipment:nullptr);
    //

    const FItemData* TorsoArmor = charData.m_CurrentBody;

    if(TorsoArmor && m_MeshBody->SkeletalMesh != TorsoArmor->m_SkEquipment)
    {
        m_MeshBody->SetSkeletalMesh(TorsoArmor->m_SkEquipment);
        SetBodyAnim();
    }
    else if(!TorsoArmor)
    {
        //갑옷없음
        SetDefaultBodyMesh();
       
    }
    //
    if(charData.m_CurrentGlove)
    {
        m_MeshGlove->SetSkeletalMesh(charData.m_CurrentGlove->m_SkEquipment);
    }
    else
    {
        SetDefaultGloveMesh();
    }
    
    if(charData.m_CurrentShoe)
    {
        m_MeshShoe->SetSkeletalMesh(charData.m_CurrentShoe->m_SkEquipment);
    }
    else
    {
        SetDefaultShoeMesh();
    }
    
    m_MeshShoulderPad->SetSkeletalMesh(charData.m_CurrentShoulder ? charData.m_CurrentShoulder->m_SkEquipment : nullptr);
    
    m_MeshBelt->SetSkeletalMesh(charData.m_CurrentBelt ? charData.m_CurrentBelt->m_SkEquipment : nullptr);
    //m_MeshBackpack->SetStaticMesh(charData.m_CurrentBackpack);
    m_MeshRightHand->SetStaticMesh(charData.m_CurrentRightWeapon?charData.m_CurrentRightWeapon->m_StEquipment:nullptr);
    m_MeshLeftHand->SetStaticMesh(charData.m_CurrentLeftWeapon?charData.m_CurrentLeftWeapon->m_StEquipment :nullptr);
    //
}

void APlayerVisual::SetDefaultBodyMesh()
{
    m_MeshBody->SetSkeletalMesh(m_DefaultBodyMesh);
    SetBodyAnim();
}

void APlayerVisual::SetDefaultShoeMesh()
{
    m_MeshShoe->SetSkeletalMesh(m_DefaultShoeMesh);
}

void APlayerVisual::SetDefaultGloveMesh()
{
    m_MeshGlove->SetSkeletalMesh(m_DefaultGloveMesh);
}

void APlayerVisual::ShowMesh()
{
    m_MeshBody->SetVisibility(true);
    m_MeshFace->SetVisibility(true);
    m_MeshBelt->SetVisibility(true);
    m_MeshGlove->SetVisibility(true);
    m_MeshShoe->SetVisibility(true);
    m_MeshHeadGear->SetVisibility(true);
    m_MeshShoulderPad->SetVisibility(true);
    m_MeshHair->SetVisibility(true);
    m_MeshBackpack->SetVisibility(true);
    m_MeshRightHand->SetVisibility(true);
    m_MeshLeftHand->SetVisibility(true);

    PRINTF("ShowMesh");
}

void APlayerVisual::HideMesh()
{
    m_MeshBody->SetVisibility(false);
    m_MeshGlove->SetVisibility(false);
    m_MeshShoe->SetVisibility(false);
    m_MeshFace->SetVisibility(false);
    m_MeshHair->SetVisibility(false);
    m_MeshBelt->SetVisibility(false);
    m_MeshBelt->SetSkeletalMesh(nullptr);
    m_MeshHeadGear->SetVisibility(false);
    m_MeshHeadGear->SetSkeletalMesh(nullptr);
    m_MeshShoulderPad->SetVisibility(false);
    m_MeshShoulderPad->SetSkeletalMesh(nullptr);
    m_MeshBackpack->SetVisibility(false);
    m_MeshBackpack->SetStaticMesh(nullptr);
    m_MeshRightHand->SetVisibility(false);
    m_MeshRightHand->SetStaticMesh(nullptr);
    m_MeshLeftHand->SetVisibility(false);
    m_MeshLeftHand->SetStaticMesh(nullptr);


    SetDefaultBodyMesh();
    SetDefaultGloveMesh();
    SetDefaultShoeMesh();

    PRINTF("HideMesh");
}

