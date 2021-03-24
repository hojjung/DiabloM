#include "PlayerVisual.h"
#include "ConstructorHelpers.h"
#include "InstalledPlatformInfo.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SkinnedMeshComponent.h"
#include "Managers/DiabloGameInstance.h"
#include "Animation/AnimSequence.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
// Sets default values
APlayerVisual::APlayerVisual()
{
	SetCanBeDamaged(false);

	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UAnimSequence> FoundAnim(TEXT(
		"AnimSequence'/Game/AnimationBlueprint/Aurora/29_Frank_ActionRPG_Sword_Attack03.29_Frank_ActionRPG_Sword_Attack03'"));

	m_AnimSeq = FoundAnim.Object;

	//init skMesh
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	CreateSkMeshComponent(&m_MeshBody, "SkMeshRoot", RootComponent);

	m_MeshBody->SetRelativeLocation(FVector(0, 0, -80.f));//-80
	m_MeshBody->SetRelativeRotation(FRotator(0, 90.f, 0));
	//init Anim	
	m_MeshBody->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	//init MasterPose
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundSkMesh(
        TEXT("SkeletalMesh'/Game/BakeMeshFinal/Aurora_GlacialEmpress_SK.Aurora_GlacialEmpress_SK'"));
	m_MeshBody->SetSkeletalMesh(FoundSkMesh.Object);
	m_MeshBody->SetAnimation(m_AnimSeq);
	//
	m_Spring = CreateDefaultSubobject<USpringArmComponent>("Spring");
	m_Spring->SetupAttachment(RootComponent);
	m_Spring->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	m_Spring->SetRelativeLocation(FVector(0.f, 0.f, 30.f));//30
	m_Spring->TargetArmLength = 300.f;

	m_Capture = CreateDefaultSubobject<USceneCaptureComponent2D>("Capture2D");
	m_Capture->SetupAttachment(m_Spring);
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> FoundTexture(
		TEXT("TextureRenderTarget2D'/Game/03_VisualEffect/T_PlayerVisual.T_PlayerVisual'"));
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

	m_Capture->ShowOnlyActors.Add(this);

	UDiabloGameInstance::Get->GetPlChar()->m_OnMeshChanged.AddUObject(this,&APlayerVisual::OnMeshChanged);

	m_MeshBody->SetForcedLOD(0);
	//OnMeshChanged(UDiabloGameInstance::Get->GetPlChar());
}

void APlayerVisual::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
//	m_PlCreateManager->m_OnVisualChange.Remove(Handle1);
	//m_PlCreateManager->m_OnStartCreation.Unbind();
}

void APlayerVisual::ShowMesh()
{
	m_MeshBody->SetVisibility(true);

	m_MeshBody->SetComponentTickEnabled(true);

	m_MeshBody->SetAnimation(m_AnimSeq);

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

	m_Capture->SetComponentTickEnabled(true);

	PRINTF("ShowMeshWithTick");
}

void APlayerVisual::HideMeshWithTick()
{
	m_MeshBody->SetVisibility(false);

	m_MeshBody->SetComponentTickEnabled(false);

	m_Capture->SetComponentTickEnabled(false);

	PRINTF("HideMeshWithTick");
}

void APlayerVisual::OnMeshChanged(APlayerDiabloCharacter* charDia)
{
	if(charDia->GetPlayerEntityData())
	{
		const FPlayerSkinTable* EntityData = charDia->GetPlayerEntityData()->m_PlayerData; 
		m_MeshBody->SetSkeletalMesh(EntityData->m_PlayerSkin);
		m_MeshBody->PlayAnimation(EntityData->m_VisualIdleAnim,true);
	}
	
	if(charDia->GetCreatedWeapon())
	{
		if(m_WeaponActor)
		{
			m_Capture->ShowOnlyActors.Remove(m_WeaponActor);
			FDetachmentTransformRules Rule(EDetachmentRule::KeepWorld, false);
			m_WeaponActor->DetachFromActor(Rule);
			m_WeaponActor->Destroy();
		}
		
		
		FActorSpawnParameters Param;

		Param.bNoFail = true;
		Param.Template = charDia->GetCreatedWeapon();

		m_WeaponActor = GetWorld()->SpawnActor<AEquipmentActor>(Param);

		FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
                                       EAttachmentRule::KeepRelative, false);

		m_WeaponActor->AttachToComponent(m_MeshBody, Rule, "RightHandBottom");

		m_Capture->ShowOnlyActors.Add(m_WeaponActor);
	}

	if(charDia->GetCreatedWing())
	{
		//const FPlayerSkinTable* EntityData = charDia->GetPlayerEntityData()->m_PlayerData; 
		//m_MeshBody->SetSkeletalMesh(EntityData->m_PlayerSkin);
		//m_MeshBody->PlayAnimation(EntityData->m_VisualIdleAnim,true);
	}

	if(charDia->GetPetComponent())
	{
		m_PetComp->SetChildActorClass(charDia->GetPetComponent()->GetChildActorClass());
	}
}
