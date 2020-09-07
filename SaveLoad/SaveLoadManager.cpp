#include "SaveLoadManager.h"
#include "Characters/DiabloPlayerController.h"
#include "DiabloM.h"
#include "SaveEquipment.h"
#include "Kismet/GameplayStatics.h"
#include "SaveLoad/SaveInventory.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/StartMap/PlayerCreateManager.h"


SaveLoadManager* SaveLoadManager::Get=nullptr;


SaveLoadManager::~SaveLoadManager()
{
   SaveLoadManager::Get=nullptr;
   m_AryLoadedCharacters.Empty();
   m_ArySlotIndexNotUsed.Empty();
   m_OnDataCreated.Clear();
}

void SaveLoadManager::InitSaveLoadManager()
{
   SaveLoadManager::Get=this;

   m_nMaxSlotCount = 5;

   m_nCurrentSlotCount=2;

   TryLoadAllCharacter();
}

void SaveLoadManager::TryLoadAllCharacter()
{
   for(int i=0; i< m_nCurrentSlotCount;i++)
   {
      if(!LoadCharacterStat(i))
      {
         //해당인덱스 슬롯에 데이터 없음
         m_ArySlotIndexNotUsed.Enqueue(i);
         PRINTF("Fail: %d",i);
      }
      else
      {
         PRINTF("Success: %d",i);
      }
   }
}

void SaveLoadManager::DeleteAllSlot()
{
   PRINTF("DeleteSlot");
   for(int i=0; i<m_nCurrentSlotCount;i++)
   {
      if(!UGameplayStatics::DeleteGameInSlot("Equipment",i))
      {
         //PRINTF("Fail Delete Equipment");
      }
      if(!UGameplayStatics::DeleteGameInSlot("Character",i))
      {
         //PRINTF("Fail Delete Character");
      }
      if(!UGameplayStatics::DeleteGameInSlot("Inventory",i))
      {
         //PRINTF("Fail Delete Inventory");
      }
   }
}

void SaveLoadManager::SaveInventory()const
{
   auto* SaveInven =Cast<USaveInventory>( UGameplayStatics::CreateSaveGameObject(USaveInventory::StaticClass()));

   SaveInven->m_SaveVersion = m_SaveVersion;
      
   auto* Controller =ADiabloPlayerController::Get;
   
   SaveInven->SetSaveData(Controller->GetInven());

   UGameplayStatics::SaveGameToSlot(SaveInven,"Inventory",m_nCurrentPlayerIndex);
   
   PRINTF("SaveInventory");
}

void SaveLoadManager::LoadInventory()const
{
   auto* LoadInven =Cast<USaveInventory>(UGameplayStatics::LoadGameFromSlot("Inventory",m_nCurrentPlayerIndex));

   LoadInven->m_SaveVersion = m_SaveVersion;
   
   auto* Controller =ADiabloPlayerController::Get;
   
   LoadInven->SetInvenLoadData(Controller->GetInven());

   PRINTF("LoadInventory");
}

void SaveLoadManager::SaveEquipment() const
{
   auto* SaveEquip =Cast<USaveEquipment>( UGameplayStatics::CreateSaveGameObject(USaveEquipment::StaticClass()));

   SaveEquip->m_SaveVersion = m_SaveVersion;
   
   auto* Controller =ADiabloPlayerController::Get;
   
   SaveEquip->SetEquipSaveData(Controller->GetEquipment()->GetArySlotPtr());

   UGameplayStatics::SaveGameToSlot(SaveEquip,"Equipment",m_nCurrentPlayerIndex);

   PRINTF("SaveEquipment");
}

void SaveLoadManager::LoadEquipment() const
{
   auto* LoadEquip =Cast<USaveEquipment>(UGameplayStatics::LoadGameFromSlot("Equipment",m_nCurrentPlayerIndex));

   auto* Controller =ADiabloPlayerController::Get;

   EquipmentSystem* EquipPtr = Controller->GetEquipment();
      
   LoadEquip->SetEquipLoadData(&EquipPtr);

   PRINTF("LoadEquipment");
}

void SaveLoadManager::SaveCharacterStat() const
{
   auto* SaveCharStat =Cast<USaveCharacterStatus>( UGameplayStatics::CreateSaveGameObject(USaveCharacterStatus::StaticClass()));
   //SaveCharStat->m_nLevel
   auto* Controller =ADiabloPlayerController::Get;

   SaveCharStat->m_nSlotIndex= m_nCurrentPlayerIndex;

   SaveCharStat->m_nLevel = Controller->GetUnitPawn()->GetLevel();
//m_TextUnitName
   SaveCharStat->m_TextName = Controller->GetUnitPawn()->m_TextUnitName;

   UGameplayStatics::SaveGameToSlot(SaveCharStat,"Character",m_nCurrentPlayerIndex);

}

bool SaveLoadManager::LoadCharacterStat(int index)
{
   if(!UGameplayStatics::DoesSaveGameExist("Character",index))
   {
      return false;
   }
   auto* LoadCharStat =Cast<USaveCharacterStatus>(UGameplayStatics::LoadGameFromSlot("Character",index));

   PRINTF("Loaded - %d - %s",index,*LoadCharStat->m_TextName.ToString());  
   m_AryLoadedCharacters.Emplace(LoadCharStat);   
   //여기에도 인스턴스를 매달아야함
   
   return true;
}


void SaveLoadManager::CreateNewCharacter(PlayerCreateManager* plManager)
{
   int PlayerIndex=-1;
   
   if(!m_ArySlotIndexNotUsed.Dequeue(PlayerIndex))
   {
      PRINTF("No Slot - Fail Creation");
      return;
   }
   PRINTF("Empty Index: %d",PlayerIndex);
   //   
   auto* SaveCharStat =Cast<USaveCharacterStatus>( UGameplayStatics::CreateSaveGameObject(USaveCharacterStatus::StaticClass()));
   auto* SaveEquip =Cast<USaveEquipment>( UGameplayStatics::CreateSaveGameObject(USaveEquipment::StaticClass()));
   auto* SaveInven =Cast<USaveInventory>( UGameplayStatics::CreateSaveGameObject(USaveInventory::StaticClass()));
   //SaveCharStat
   SaveCharStat->m_SaveVersion=m_SaveVersion;
   SaveCharStat->m_nSlotIndex= PlayerIndex;
   SaveCharStat->m_nLevel = 1;
   SaveCharStat->m_TextName  = plManager->m_TextName;
   SaveCharStat->m_IndexFace = plManager->m_IndexFace;
   SaveCharStat->m_IndexHair = plManager->m_IndexHair;
   UGameplayStatics::SaveGameToSlot(SaveCharStat,"Character",PlayerIndex);
   //SaveEquip
   SaveEquip->m_SaveVersion=m_SaveVersion;
   SaveEquip->SetEquipSaveDataFromCreation(plManager->m_CurrentCharData);
   UGameplayStatics::SaveGameToSlot(SaveEquip,"Equipment",PlayerIndex);
   //SaveInven
   SaveInven->m_SaveVersion=m_SaveVersion;
   
   UGameplayStatics::SaveGameToSlot(SaveInven,"Inventory",PlayerIndex);
   //
   m_AryLoadedCharacters.Emplace(SaveCharStat);
   //
   m_OnDataCreated.Broadcast(SaveCharStat);

   //create Ned Widget
   //Set the WIdget
}
