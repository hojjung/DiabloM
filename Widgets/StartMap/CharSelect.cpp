#include "CharSelect.h"



#include "Characters/StartMap/PlayerCreateController.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/StartMap/PlayerCreateManager.h"
#include "SaveLoad/SaveEquipment.h"
#include "SaveLoad/SaveLoadManager.h"
#include "SaveLoad/SaveCharacterStatus.h"

void UCharSelect::Init(USaveLoadManager* SaveLoadManagerOld)
{
    for(const USaveCharacterStatus* Char : SaveLoadManagerOld->GetLoadedChars())
    {
        if(!Char)
        {
            continue;
        }
        
        CreateCharInfo(Char);
    }
    SaveLoadManagerOld->m_OnDataCreated.BindUObject(this,&UCharSelect::CreateCharInfo);
    //create all charInfo From Save Datas
}

void UCharSelect::CreateCharInfo(const USaveCharacterStatus* charStats)
{
    UCharInfo* CharInfoCreated = CreateWidget<UCharInfo>(this, m_ClassCharInfo);

    CharInfoCreated->Init(charStats);

    CharInfoCreated->m_OnSelect.BindUObject(this,&UCharSelect::FocusCharacter);

    m_SlotParent->AddChildToVerticalBox(CharInfoCreated);
}

void UCharSelect::FocusCharacter(int slotIndex,UCharInfo* focusedInfo)
{
    if(m_FocusedInfo && focusedInfo != m_FocusedInfo)
    {
        m_FocusedInfo->DeselectSlot();
        m_FocusedInfo=nullptr;
    }
    PRINTF("Index:%d",slotIndex);
    
    Cast<APlayerCreateController> (GetOwningPlayer())->GetPlayerVisual()->ShowMesh();
    
    UPlayerCreateManager* PlMa=UPlayerCreateManager::Get;
    USaveCharacterStatus* CharStat = USaveLoadManager::Get->GetLoadedChars()[slotIndex];
    USaveEquipment* CharEquip = USaveLoadManager::Get->GetLoadedEquip()[slotIndex];

    PlMa->SetCurrentDataFromSaveFile(CharStat,CharEquip);

    m_FocusedInfo=focusedInfo;
}
