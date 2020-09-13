#include "CharSelect.h"



#include "Characters/StartMap/PlayerCreateController.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/StartMap/PlayerCreateManager.h"
#include "SaveLoad/SaveEquipment.h"
#include "SaveLoad/SaveLoadManager.h"
#include "SaveLoad/SaveCharacterStatus.h"

void UCharSelect::Init(USaveLoadManager* SaveLoadManagerOld)
{
    m_AryCharInfoSlot.Init(nullptr,MAXSLOT);
    m_FocusedIndex=-1;
    for(const USaveCharacterStatus* Char : SaveLoadManagerOld->GetLoadedChars())
    {
        if(!Char)
        {
            continue;
        }
        
        CreateCharInfo(Char);
    }
    SaveLoadManagerOld->m_OnDataCreated.BindUObject(this,&UCharSelect::CreateCharInfo);

    m_BtnContinue->OnClicked.AddDynamic(this,&UCharSelect::ContinueToGameWorld);
    //create all charInfo From Save Datas
}

void UCharSelect::CreateCharInfo(const USaveCharacterStatus* charStats)
{
    UCharInfo* CharInfoCreated = CreateWidget<UCharInfo>(this, m_ClassCharInfo);

    CharInfoCreated->Init(charStats);

    CharInfoCreated->m_OnSelect.BindUObject(this,&UCharSelect::FocusCharacter);

    m_SlotParent->AddChildToVerticalBox(CharInfoCreated)->SetPadding(FMargin(20.f));

    m_AryCharInfoSlot[charStats->m_nSlotIndex]=TWeakObjectPtr<UCharInfo>(CharInfoCreated);

}

void UCharSelect::FocusCharacter(int slotIndex)
{
    if(m_FocusedIndex == slotIndex)
    {
        return;
    }

    if(slotIndex == -1)
    {
        return;
    }

    if(m_FocusedIndex!=-1)
        m_AryCharInfoSlot[m_FocusedIndex]->DeselectSlot();

    m_FocusedIndex=slotIndex;
    
    Cast<APlayerCreateController> (GetOwningPlayer())->GetPlayerVisual()->ShowMesh();
    
    UPlayerCreateManager* PlMa=UPlayerCreateManager::Get;
    USaveCharacterStatus* CharStat = USaveLoadManager::Get->GetLoadedChars()[m_FocusedIndex];
    USaveEquipment* CharEquip = USaveLoadManager::Get->GetLoadedEquip()[m_FocusedIndex];

    PlMa->SetCurrentDataFromSaveFile(CharStat,CharEquip);

    m_FocusedIndex=slotIndex;
    
    PlMa->m_CurrentSelectSlot = m_FocusedIndex;

}

void UCharSelect::ContinueToGameWorld()
{
    PRINTF("Continue World");

    UGameplayStatics::OpenLevel(GetWorld(),"StartHub");
    
}
