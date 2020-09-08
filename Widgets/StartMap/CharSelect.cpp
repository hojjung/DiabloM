// Fill out your copyright notice in the Description page of Project Settings.


// ReSharper disable All
#include "CharSelect.h"



#include "Characters/StartMap/PlayerCreateController.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/StartMap/PlayerCreateManager.h"
#include "SaveLoad/SaveEquipment.h"
#include "SaveLoad/SaveLoadManager.h"
#include "SaveLoad/SaveCharacterStatus.h"

void UCharSelect::Init(SaveLoadManager* saveLoadManager)
{
    for(auto* Char : saveLoadManager->GetLoadedChars())
    {
        if(!Char)
        {
            continue;
        }
        CreateCharInfo(Char);
    }
    saveLoadManager->m_OnDataCreated.BindUObject(this,&UCharSelect::CreateCharInfo);
    //create all charInfo From Save Datas
}

void UCharSelect::CreateCharInfo(const USaveCharacterStatus* charStats)
{
    auto* CharInfoCreated= CreateWidget<UCharInfo>(this,m_ClassCharInfo);

    CharInfoCreated->Init(charStats);

    CharInfoCreated->m_OnSelect.BindUObject(this,&UCharSelect::FocusCharacter);

    m_SlotParent->AddChildToVerticalBox(CharInfoCreated);
}

void UCharSelect::StartCreation()
{
    UDiabloGameInstance::Get->m_PlCreateManager->StartCreateCharcter();
}

void UCharSelect::FocusCharacter(int slotIndex)
{
    PRINTF("Index:%d",slotIndex);
    Cast<APlayerCreateController> (GetOwningPlayer())->GetPlayerVisual()->ShowMesh();
    PlayerCreateManager* PlMa=PlayerCreateManager::Get;
    auto* CharStat= SaveLoadManager::Get->GetLoadedChars()[slotIndex];
    auto* CharEquip= SaveLoadManager::Get->GetLoadedEquip()[slotIndex];
    
    PlMa->m_CurrentCharData. m_CurrentBody=CharEquip->m_EquipAry[(int)ESlots::Torso].m_ItemData ?CharEquip->m_EquipAry[(int)ESlots::Torso].m_ItemData :nullptr;
    PlMa->m_CurrentCharData. m_CurrentHelmet = CharEquip->m_EquipAry[(int)ESlots::Head].m_ItemData ? CharEquip->m_EquipAry[(int)ESlots::Head].m_ItemData :nullptr;
    PlMa->m_CurrentCharData. m_CurrentHair=PlMa->GetHair(CharStat->m_IndexHair,PlMa->m_CurrentCharData.m_CurrentHelmet);
    PlMa->m_CurrentCharData. m_CurrentFace=PlMa->GetFace(CharStat->m_IndexFace);
    PlMa->m_CurrentCharData. m_CurrentShoe= CharEquip->m_EquipAry[(int)ESlots::Leg].m_ItemData ? CharEquip->m_EquipAry[(int)ESlots::Leg].m_ItemData :nullptr;
    PlMa->m_CurrentCharData. m_CurrentGlove= CharEquip->m_EquipAry[(int)ESlots::Hand].m_ItemData ? CharEquip->m_EquipAry[(int)ESlots::Hand].m_ItemData :nullptr;
    PlMa->m_CurrentCharData. m_CurrentShoulder= CharEquip->m_EquipAry[(int)ESlots::Shoulder].m_ItemData ? CharEquip->m_EquipAry[(int)ESlots::Shoulder].m_ItemData :nullptr;
    PlMa->m_CurrentCharData. m_CurrentBelt= CharEquip->m_EquipAry[(int)ESlots::Waist].m_ItemData ? CharEquip->m_EquipAry[(int)ESlots::Waist].m_ItemData :nullptr;
    //PlMa->m_CurrentCharData. m_CurrentBackpack= CharEquip->m_EquipAry[(int)ESlots::Head].m_ItemData ? CharEquip->m_EquipAry[(int)ESlots::Head].m_ItemData->m_SkEquipment :nullptr;
    PlMa->m_CurrentCharData. m_CurrentRightWeapon= CharEquip->m_EquipAry[(int)ESlots::WeaponRight].m_ItemData ? CharEquip->m_EquipAry[(int)ESlots::WeaponRight].m_ItemData :nullptr;
    PlMa->m_CurrentCharData. m_CurrentLeftWeapon= CharEquip->m_EquipAry[(int)ESlots::WeaponLeft].m_ItemData ? CharEquip->m_EquipAry[(int)ESlots::WeaponLeft].m_ItemData :nullptr;

    
    PlayerCreateManager::Get->OnDataChanged();
    //use index to get char info,status and equip
}
