#include "PlayerVisual.h"
#include "ConstructorHelpers.h"
#include "InstalledPlatformInfo.h"
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

	static ConstructorHelpers::FObjectFinder<UAnimSequence> FoundAnim(TEXT(
		"AnimSequence'/Game/AnimationBlueprint/Barbarian/Locomotion/DefaultUEAnim_Idle_2.DefaultUEAnim_Idle_2'"));

	m_AnimSeq = FoundAnim.Object;

	//init skMesh
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	CreateSkMeshComponent(&m_MeshBody, "SkMeshRoot", RootComponent);

	m_MeshBody->SetRelativeLocation(FVector(0, 0, -80.f));
	m_MeshBody->SetRelativeRotation(FRotator(0, 90.f, 0.f));
	//init Anim	
	m_MeshBody->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	//init MasterPose
	//
	m_MeshBody->SetAnimation(m_AnimSeq);
	//
	m_PlCreateManager = nullptr;
	//
	m_Spring = CreateDefaultSubobject<USpringArmComponent>("Spring");
	m_Spring->SetupAttachment(RootComponent);
	m_Spring->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	m_Spring->SetRelativeLocation(FVector(0.f, 0.f, 30.f));
	m_Spring->TargetArmLength = 500.f;

	m_Capture = CreateDefaultSubobject<USceneCaptureComponent2D>("Capture2D");
	m_Capture->SetupAttachment(m_Spring);
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> FoundTexture(
		TEXT("TextureRenderTarget2D'/Game/03_VisualEffect/T_PlayerVisual.T_PlayerVisual'"));
	m_CaptureTexture = FoundTexture.Object;
	m_Capture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives;
	m_Capture->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR; //SCS_SceneColorHDR
	m_Capture->MaxViewDistanceOverride = 1000.f;
	m_Capture->SetTickableWhenPaused(true);
	m_Capture->ProjectionType = ECameraProjectionMode::Orthographic;
	m_Capture->OrthoWidth = 150.f;

	m_Capture->TextureTarget = m_CaptureTexture;
}

void APlayerVisual::CreateSkMeshComponent(USkeletalMeshComponent** refSkComp, FName keyName, USceneComponent* root)
{
	(*refSkComp) = CreateDefaultSubobject<USkeletalMeshComponent>(keyName);
	(*refSkComp)->bOwnerNoSee = false;
	(*refSkComp)->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	(*refSkComp)->bCastDynamicShadow = true; //chanage for mobile
	(*refSkComp)->bAffectDynamicIndirectLighting = true;
	(*refSkComp)->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	(*refSkComp)->SetupAttachment(root);
	(*refSkComp)->SetCollisionProfileName("CharacterMesh");
	(*refSkComp)->SetGenerateOverlapEvents(false);
	(*refSkComp)->SetCanEverAffectNavigation(false);
	(*refSkComp)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	(*refSkComp)->CastShadow = true;
	(*refSkComp)->SetTickableWhenPaused(true);
}

void APlayerVisual::BeginPlay()
{
	Super::BeginPlay();

	m_AryVisualEquipment.Init(nullptr, (int)ESlotsEquipAry::Length);

	m_PlCreateManager = GetGameInstance<UDiabloGameInstance>()->GetPlCreateManager();

	Handle1 = m_PlCreateManager->m_OnVisualChange.AddUObject(this, &APlayerVisual::OnMeshVisualChanged);

	m_PlCreateManager->m_OnStartCreation.BindUObject(this, &APlayerVisual::ShowMesh);

	m_Capture->PostProcessBlendWeight = 0.f;

	m_Capture->ShowOnlyActorComponents(this);

	HideMesh();
}

void APlayerVisual::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	m_PlCreateManager->m_OnVisualChange.Remove(Handle1);
	m_PlCreateManager->m_OnStartCreation.Unbind();
}

void APlayerVisual::OnMeshVisualChanged(const FCurrentCharData& charData)
{
	m_MeshBody->SetSkeletalMesh(charData.m_CurrentSkin);
	m_AnimSeq = charData.m_CoolIdle;
	m_MeshBody->SetAnimation(m_AnimSeq);
	m_MeshBody->Play(true); //body change = need animation update

	int Iter = (int)ESlotsEquipAry::Length;

	for (int i = 0; i < Iter; i++)
	{
		//이미 없다
		const FItemData* Data = charData.m_AryEquipItemData[i];

		if (m_AryVisualEquipment[i])
		{
			m_Capture->RemoveShowOnlyActorComponents(m_AryVisualEquipment[i]);
			m_AryVisualEquipment[i]->SetHidden(true);
			m_AryVisualEquipment[i]->Destroy();
			m_AryVisualEquipment[i] = nullptr;
		}
		
		if (Data&&Data->m_EquipmentBP) //new item need spawn
		{
			m_AryVisualEquipment[i] = AEquipmentActor::SpawnToMesh(m_MeshBody, Data->m_EquipmentBP,
			                                                       Data->m_EquipSocketName);
			m_Capture->ShowOnlyActorComponents(m_AryVisualEquipment[i]);
		}
	}
}


void APlayerVisual::ShowMesh()
{
	m_MeshBody->SetVisibility(true);

	m_MeshBody->SetComponentTickEnabled(true);

	PRINTF("ShowMesh");
}

void APlayerVisual::HideMesh()
{
	m_MeshBody->SetVisibility(false);

	m_MeshBody->SetComponentTickEnabled(false);

	PRINTF("HideMesh");
}

void APlayerVisual::ShowMeshWithTick()
{
	m_MeshBody->SetVisibility(true);

	m_MeshBody->SetComponentTickEnabled(true);


	PRINTF("ShowMeshWithTick");
}

void APlayerVisual::HideMeshWithTick()
{
	m_MeshBody->SetVisibility(false);

	m_MeshBody->SetComponentTickEnabled(false);

	PRINTF("HideMeshWithTick");
}
