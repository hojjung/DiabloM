// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerVisual.h"



#include "ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SkinnedMeshComponent.h"

// Sets default values
APlayerVisual::APlayerVisual()
{
	SetCanBeDamaged(false);
	PrimaryActorTick.bCanEverTick = false;
	
	  static ConstructorHelpers::FObjectFinder<UAnimationAsset> FoundAnim(
	  	TEXT("AnimSequence'/Game/Animations/NoWeaponStance/StandingIdle_noWeaponAnim.StandingIdle_noWeaponAnim'"));
	auto* AnimSeq = FoundAnim.Object;

	//init skMesh
	CreateSkMeshComponent(&m_MeshBody,"SkMeshRoot");
	RootComponent=m_MeshBody;
	CreateSkMeshComponent(&m_MeshFace,"SkMesh01");
	CreateSkMeshComponent(&m_MeshHair,"SkMesh02");
	CreateSkMeshComponent(&m_MeshBelt,"SkMesh03");
	CreateSkMeshComponent(&m_MeshGlove,"SkMesh04");
	CreateSkMeshComponent(&m_MeshShoe,"SkMesh05");
	CreateSkMeshComponent(&m_MeshShoulderPad,"SkMesh06");
	CreateSkMeshComponent(&m_MeshHeadGear,"SkMesh07");
	//init stMesh
	m_MeshBackpack = CreateDefaultSubobject<UStaticMeshComponent>("StMeshBackpack");
	m_MeshBackpack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshBackpack->SetupAttachment(RootComponent,"Backpack");
	m_MeshRightHand = CreateDefaultSubobject<UStaticMeshComponent>("StMeshRightHand");
	m_MeshRightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshRightHand->SetupAttachment(RootComponent,"RightWeaponShield");
	m_MeshLeftHand = CreateDefaultSubobject<UStaticMeshComponent>("StMeshLeftHand");
	m_MeshLeftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshLeftHand->SetupAttachment(RootComponent,"LeftWeaponShield");
	//init Anim	
	m_MeshBody->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	m_MeshBody->SetAnimation(AnimSeq);
	//init MasterPose
	m_MeshFace->SetMasterPoseComponent(m_MeshBody);
	m_MeshHair->SetMasterPoseComponent(m_MeshBody);
	m_MeshGlove->SetMasterPoseComponent(m_MeshBody);
	m_MeshShoe->SetMasterPoseComponent(m_MeshBody);
	m_MeshHeadGear->SetMasterPoseComponent(m_MeshBody);
	m_MeshShoulderPad->SetMasterPoseComponent(m_MeshBody);
	m_MeshBelt->SetMasterPoseComponent(m_MeshBody);
	//
	m_CachedHairFullMesh=nullptr;
	m_CachedHairHalfMesh=nullptr;
}
void APlayerVisual::CreateSkMeshComponent(USkeletalMeshComponent** refSkComp,FName keyName)
{
	(*refSkComp) = CreateDefaultSubobject<USkeletalMeshComponent>(keyName);
	(*refSkComp)->bOwnerNoSee = false;
	(*refSkComp)->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	(*refSkComp)->bCastDynamicShadow = true;//chanage for mobile
	(*refSkComp)->bAffectDynamicIndirectLighting = true;
	(*refSkComp)->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	(*refSkComp)->SetupAttachment(RootComponent);
	(*refSkComp)->SetCollisionProfileName("CharacterMesh");
	(*refSkComp)->SetGenerateOverlapEvents(false);
	(*refSkComp)->SetCanEverAffectNavigation(false);
	(*refSkComp)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void APlayerVisual::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerVisual::OnMeshVisualChanged()
{
	//매니저에게서 받아와야겠지?
	//매니저에게서 GetCurrentChaData()로 받아오기
	//m_CachedHairMesh
	//m_CachedFull Mesh
	m_MeshFace->SetSkeletalMesh(UnitData->m_SkFace);
	
	SetHairMeshFull();//hair cache,if no helmet
	SetHairMeshHalf();
}


void APlayerVisual::SetHairMeshFull()
{
	m_MeshHair->SetSkeletalMesh(m_CachedHairFullMesh);
}

void APlayerVisual::SetHairMeshHalf()
{
	m_MeshHair->SetSkeletalMesh(m_CachedHairHalfMesh);
}
