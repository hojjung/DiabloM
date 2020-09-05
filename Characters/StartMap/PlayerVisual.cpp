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
    m_CachedHairFullMesh = nullptr;
    m_CachedHairHalfMesh = nullptr;
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
    m_PlCreateManager->OnDataChanged();
}

void APlayerVisual::OnMeshVisualChanged(const FCurrentCharData& charData)
{
    m_CachedHairHalfMesh = charData.m_CurrentHair->m_MeshHalfHair;
    m_CachedHairFullMesh = charData.m_CurrentHair->m_MeshFullHair;

    m_MeshFace->SetSkeletalMesh(charData.m_CurrentFace->m_MeshFace);

    bool HasHelmet = !charData.m_CurrentArmor->m_HelmetHandle.IsNull();

    if (HasHelmet)
    {
        m_MeshHair->SetSkeletalMesh(charData.m_CurrentHair->m_MeshHalfHair);
        m_MeshHeadGear->SetSkeletalMesh(UItemDataTable::GetItemData(charData.m_CurrentArmor->m_HelmetHandle.RowName).m_SkEquipment);
    }
    else
    {
        m_MeshHair->SetSkeletalMesh(charData.m_CurrentHair->m_MeshFullHair);
    }

    m_MeshBody->SetSkeletalMesh(UItemDataTable::GetItemData(charData.m_CurrentArmor->m_BodyArmorHandle.RowName).m_SkEquipment);
    //
    m_MeshGlove->SetSkeletalMesh(charData.m_CurrentArmor->m_GloveHandle.IsNull()? nullptr:UItemDataTable::GetItemData(charData.m_CurrentArmor->m_GloveHandle.RowName).m_SkEquipment);
    m_MeshShoe->SetSkeletalMesh(charData.m_CurrentArmor->m_ShoeHandle.IsNull()? nullptr:UItemDataTable::GetItemData(charData.m_CurrentArmor->m_ShoeHandle.RowName).m_SkEquipment);
    m_MeshShoulderPad->SetSkeletalMesh(charData.m_CurrentArmor->m_Shoulderandle.IsNull()? nullptr: UItemDataTable::GetItemData(charData.m_CurrentArmor->m_Shoulderandle.RowName).m_SkEquipment);
    m_MeshBelt->SetSkeletalMesh(charData.m_CurrentArmor->m_BeltHandle.IsNull()? nullptr: UItemDataTable::GetItemData(charData.m_CurrentArmor->m_BeltHandle.RowName).m_SkEquipment);
    //
    m_MeshBackpack->SetStaticMesh(charData.m_CurrentArmor->m_BackpackHandle.IsNull()? nullptr: UItemDataTable::GetItemData(charData.m_CurrentArmor->m_BackpackHandle.RowName).m_StEquipment);
    m_MeshRightHand->SetStaticMesh(charData.m_CurrentWeapon->m_RightWeaponHandle.IsNull()? nullptr: UItemDataTable::GetItemData(charData.m_CurrentWeapon->m_RightWeaponHandle.RowName).m_StEquipment);
    m_MeshLeftHand->SetStaticMesh(charData.m_CurrentWeapon->m_LeftWeaponHandle.IsNull()? nullptr: UItemDataTable::GetItemData(charData.m_CurrentWeapon->m_LeftWeaponHandle.RowName).m_StEquipment);

    m_MeshBody->SetAnimation(m_AnimSeq);
    m_MeshBody->Play(true);
}

void APlayerVisual::SetHairMeshFull()
{
    m_MeshHair->SetSkeletalMesh(m_CachedHairFullMesh);
}

void APlayerVisual::SetHairMeshHalf()
{
    m_MeshHair->SetSkeletalMesh(m_CachedHairHalfMesh);
}
