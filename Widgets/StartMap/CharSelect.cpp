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

void UCharSelect::FocusCharacter(int slotIndex)
{
    PRINTF("Index:%d",slotIndex);
    Cast<APlayerCreateController> (GetOwningPlayer())->GetPlayerVisual()->ShowMesh();
    PlayerCreateManager* PlMa=PlayerCreateManager::Get;
    auto* CharStat= SaveLoadManager::Get->GetLoadedChars()[slotIndex];
    auto* CharEquip= SaveLoadManager::Get->GetLoadedEquip()[slotIndex];

    PlMa->SetCurrentDataFromSaveFile(CharStat,CharEquip);
}
