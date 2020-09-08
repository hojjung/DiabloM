// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerVisual.h"
#include "ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SkinnedMeshComponent.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/StartMap/PlayerCreateManager.h"
#include "Animation/AnimSequence.h"
// Sets default values
APlayerVisual::APlayerVisual()
{
    SetCanBeDamaged(false);
    
    PrimaryActorTick.bCanEverTick = false;
    
    static ConstructorHelpers::FObjectFinder<UAnimSequence> FoundAnim(TEXT("AnimSequence'/Game/Animations/NoWeaponStance/StandingIdle_noWeaponAnim.StandingIdle_noWeaponAnim'"));
    
    m_AnimSeq = FoundAnim.Object;

    //init skMesh
    CreateSkMeshComponent(&m_MeshBody, "SkMeshRoot");
    RootComponent = m_MeshBody;
    CreateSkMeshComponent(&m_MeshFace, "SkMesh01");
    CreateSkMeshComponent(&m_MeshHair, "SkMesh02");
    CreateSkMeshComponent(&m_MeshBelt, "SkMesh03");
    CreateSkMeshComponent(&m_MeshGlove, "SkMesh04");
    CreateSkMeshComponent(&m_MeshShoe, "SkMesh05");
    CreateSkMeshComponent(&m_MeshShoulderPad, "SkMesh06");
    CreateSkMeshComponent(&m_MeshHeadGear, "SkMesh07");
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
}

void APlayerVisual::CreateSkMeshComponent(USkeletalMeshComponent** refSkComp, FName keyName)
{
    (*refSkComp) = CreateDefaultSubobject<USkeletalMeshComponent>(keyName);
    (*refSkComp)->bOwnerNoSee = false;
    (*refSkComp)->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
    (*refSkComp)->bCastDynamicShadow = false; //chanage for mobile
    (*refSkComp)->bAffectDynamicIndirectLighting = true;
    (*refSkComp)->PrimaryComponentTick.TickGroup = TG_PrePhysics;
    (*refSkComp)->SetupAttachment(RootComponent);
    (*refSkComp)->SetCollisionProfileName("CharacterMesh");
    (*refSkComp)->SetGenerateOverlapEvents(false);
    (*refSkComp)->SetCanEverAffectNavigation(false);
    (*refSkComp)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    (*refSkComp)->CastShadow=false;
}

void APlayerVisual::BeginPlay()
{
    Super::BeginPlay();
    m_PlCreateManager = GetGameInstance<UDiabloGameInstance>()->m_PlCreateManager;
    m_PlCreateManager->m_OnVisualChange.AddUObject(this, &APlayerVisual::OnMeshVisualChanged);
    HideMesh();
    m_PlCreateManager->m_OnStartCreation.BindUObject(this, &APlayerVisual::ShowMesh);
    
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
    auto* BodyMesh=charData.m_CurrentBody;

    
    if(BodyMesh &&m_MeshBody->SkeletalMesh != BodyMesh->m_SkEquipment)
    {
        m_MeshBody->SetSkeletalMesh(BodyMesh->m_SkEquipment);
        m_MeshBody->SetAnimation(m_AnimSeq);
        m_MeshBody->Play(true);//body change = need animation update
    }
    //
    if(charData.m_CurrentGlove)
        m_MeshGlove->SetSkeletalMesh(charData.m_CurrentGlove->m_SkEquipment);
    if(charData.m_CurrentShoe)
        m_MeshShoe->SetSkeletalMesh(charData.m_CurrentShoe->m_SkEquipment);
    m_MeshShoulderPad->SetSkeletalMesh(charData.m_CurrentShoulder ? charData.m_CurrentShoulder->m_SkEquipment : nullptr);
    m_MeshBelt->SetSkeletalMesh(charData.m_CurrentBelt ? charData.m_CurrentBelt->m_SkEquipment : nullptr);
    //
    //m_MeshBackpack->SetStaticMesh(charData.m_CurrentBackpack);
    m_MeshRightHand->SetStaticMesh(charData.m_CurrentRightWeapon?charData.m_CurrentRightWeapon->m_StEquipment:nullptr);
    m_MeshLeftHand->SetStaticMesh(charData.m_CurrentLeftWeapon?charData.m_CurrentLeftWeapon->m_StEquipment :nullptr);
    //
    
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
    m_PlCreateManager->OnDataChanged();
}

void APlayerVisual::HideMesh()
{
    m_MeshBody->SetVisibility(false);
    m_MeshFace->SetVisibility(false);
    m_MeshBelt->SetVisibility(false);
    m_MeshGlove->SetVisibility(false);
    m_MeshShoe->SetVisibility(false);
    m_MeshHeadGear->SetVisibility(false);
    m_MeshShoulderPad->SetVisibility(false);
    m_MeshHair->SetVisibility(false);
    m_MeshBackpack->SetVisibility(false);
    m_MeshRightHand->SetVisibility(false);
    m_MeshLeftHand->SetVisibility(false);
}

