#include "SaveLoadManager.h"
#include "Characters/DiabloPlayerController.h"
#include "DiabloM.h"
#include "SaveEquipment.h"
#include "Kismet/GameplayStatics.h"
#include "SaveLoad/SaveInventory.h"
#include "SaveLoad/SaveEquipment.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/StartMap/PlayerCreateManager.h"


SaveLoadManager* SaveLoadManager::Get=nullptr;


SaveLoadManager::~SaveLoadManager()
{
   SaveLoadManager::Get=nullptr;
   m_AryLoadedCharacters.Empty();
   m_AryLoadedEquipments.Empty();
   m_AryLoadedInventorys.Empty();
   m_OnDataCreated.Unbind();
}

void SaveLoadManager::InitSaveLoadManager()
{
   SaveLoadManager::Get=this;

   m_nMaxSlotCount = 7;

   m_nCurrentSlotCount=5;

   m_AryLoadedCharacters.Init(nullptr,m_nCurrentSlotCount);
   m_AryLoadedEquipments.Init(nullptr,m_nCurrentSlotCount);
   m_AryLoadedInventorys.Init(nullptr,m_nCurrentSlotCount);
   
   TryLoadAllCharacter();
}

void SaveLoadManager::TryLoadAllCharacter()
{
   for(int i=0; i< m_nCurrentSlotCount;i++)
   {
      if(!LoadCharacterStat(i))
      {
         PRINTF("Fail: %d",i);
      }
      else
      {
         PRINTF("Success: %d",i);
      }
   }
}

void SaveLoadManager::DeleteSlot(int i)
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

void SaveLoadManager::DeleteAllSlot()
{
   PRINTF("DeleteSlot");
   for(int i=0; i<m_nCurrentSlotCount;i++)
   {
      DeleteSlot(i);
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
   m_AryLoadedCharacters[index]=LoadCharStat;
   
   auto* LoadCharEquip =Cast<USaveEquipment>(UGameplayStatics::LoadGameFromSlot("Equipment",index));
   m_AryLoadedEquipments[index]=LoadCharEquip;
   auto* LoadCharInven =Cast<USaveInventory>(UGameplayStatics::LoadGameFromSlot("Inventory",index));
   m_AryLoadedInventorys[index]=LoadCharInven;
   return true;
}


void SaveLoadManager::CreateNewCharacter(PlayerCreateManager* plManager)
{
   //
   int PlayerIndex =GetEmptyIndex();
   
   if(PlayerIndex==-1)
   {
      PRINTF("Empty Index: %d",PlayerIndex);
      return;
   }
   //   
   auto* SaveCharStat =Cast<USaveCharacterStatus>(UGameplayStatics::CreateSaveGameObject(USaveCharacterStatus::StaticClass()));
   auto* SaveEquip    =Cast<USaveEquipment>(UGameplayStatics::CreateSaveGameObject(USaveEquipment::StaticClass()));
   auto* SaveInven    =Cast<USaveInventory>(UGameplayStatics::CreateSaveGameObject(USaveInventory::StaticClass()));
   //SaveCharStat
   SaveCharStat->m_SaveVersion=m_SaveVersion;
   SaveCharStat->m_nSlotIndex= PlayerIndex;
   SaveCharStat->m_nLevel = 1;
   SaveCharStat->m_TextName  = plManager->m_CurrentTextName;
   SaveCharStat->m_IndexFace = plManager->m_IndexFace;
   SaveCharStat->m_IndexHair = plManager->m_IndexHair;
   UGameplayStatics::SaveGameToSlot(SaveCharStat,"Character",PlayerIndex);
   //SaveEquip
   SaveEquip->m_SaveVersion=m_SaveVersion;
   SaveEquip->SetEquipSaveDataFromCreation(plManager->GetCurrentCharData());
   UGameplayStatics::SaveGameToSlot(SaveEquip,"Equipment",PlayerIndex);
   //SaveInven
   SaveInven->m_SaveVersion=m_SaveVersion;
   
   UGameplayStatics::SaveGameToSlot(SaveInven,"Inventory",PlayerIndex);
   //
   m_AryLoadedCharacters[PlayerIndex]=SaveCharStat;
   m_AryLoadedEquipments[PlayerIndex]=SaveEquip;
   m_AryLoadedInventorys[PlayerIndex]=SaveInven;
   //
   m_OnDataCreated.ExecuteIfBound(SaveCharStat);

   //create Ned Widget
   //Set the WIdget
}

int SaveLoadManager::GetEmptyIndex()
{
   for (int i = 0; i < m_AryLoadedCharacters.Num(); i++)
   {
      if (m_AryLoadedCharacters[i] == nullptr)
      {
         return i;
      }
   }

   return -1;
}
