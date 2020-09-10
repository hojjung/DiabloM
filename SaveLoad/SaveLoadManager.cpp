#include "SaveLoadManager.h"
#include "Characters/DiabloPlayerController.h"
#include "DiabloM.h"
#include "SaveEquipment.h"
#include "Kismet/GameplayStatics.h"
#include "SaveLoad/SaveInventory.h"
#include "SaveLoad/SaveEquipment.h"
#include "SaveLoad/SaveCharacterStatus.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/StartMap/PlayerCreateManagerOld.h"


USaveLoadManager* USaveLoadManager::Get=nullptr;


USaveLoadManager::~USaveLoadManager()
{
   USaveLoadManager::Get=nullptr;
   m_AryLoadedCharacters.Empty();
   m_AryLoadedEquipments.Empty();
   m_AryLoadedInventoryOlds.Empty();
   m_OnDataCreated.Unbind();
}

void USaveLoadManager::InitSaveLoadManager()
{   USaveLoadManager::Get=this;

   m_nMaxSlotCount = 7;

   m_nCurrentSlotCount=5;

   m_AryLoadedCharacters.Init(nullptr,m_nCurrentSlotCount);
   m_AryLoadedEquipments.Init(nullptr,m_nCurrentSlotCount);
   m_AryLoadedInventoryOlds.Init(nullptr,m_nCurrentSlotCount);
   
   TryLoadAllCharacter();
}

void USaveLoadManager::TryLoadAllCharacter()
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

void USaveLoadManager::DeleteSlot(int i)
{
   if(!UGameplayStatics::DeleteGameInSlot("Equipment",i))
   {
      //PRINTF("Fail Delete Equipment");
   }
   if(!UGameplayStatics::DeleteGameInSlot("Character",i))
   {
      //PRINTF("Fail Delete Character");
   }
   if(!UGameplayStatics::DeleteGameInSlot("InventoryOld",i))
   {
      //PRINTF("Fail Delete InventoryOld");
   }

   m_AryLoadedCharacters[i]=nullptr;
   m_AryLoadedEquipments[i]=nullptr;
   m_AryLoadedInventoryOlds[i]=nullptr;
}

void USaveLoadManager::DeleteAllSlot()
{
   PRINTF("DeleteSlot");
   for(int i=0; i<m_nCurrentSlotCount;i++)
   {
      DeleteSlot(i);
   }
}

void USaveLoadManager::SaveInventory()const
{
   USaveInventory* SaveInven = Cast<USaveInventory>(
      UGameplayStatics::CreateSaveGameObject(USaveInventory::StaticClass()));

   SaveInven->m_SaveVersion = m_SaveVersion;

   ADiabloPlayerController* Controller = ADiabloPlayerController::Get;
   
   SaveInven->SetSaveData(Controller->GetInven());

   UGameplayStatics::SaveGameToSlot(SaveInven,"InventoryOld",m_nCurrentPlayerIndex);
   
   PRINTF("SaveInventory");
}

void USaveLoadManager::LoadInventoryOld()const
{
   USaveInventory* LoadInven = Cast<USaveInventory>(
      UGameplayStatics::LoadGameFromSlot("InventoryOld", m_nCurrentPlayerIndex));

   LoadInven->m_SaveVersion = m_SaveVersion;

   ADiabloPlayerController* Controller = ADiabloPlayerController::Get;
   
   LoadInven->SetInvenLoadData(Controller->GetInven());

   PRINTF("LoadInventoryOld");
}

void USaveLoadManager::SaveEquipment() const
{
   USaveEquipment* SaveEquip = Cast<USaveEquipment>(
      UGameplayStatics::CreateSaveGameObject(USaveEquipment::StaticClass()));

   SaveEquip->m_SaveVersion = m_SaveVersion;

   ADiabloPlayerController* Controller = ADiabloPlayerController::Get;
   
   SaveEquip->SetEquipSaveData(Controller->GetEquipment()->GetArySlotPtr());

   UGameplayStatics::SaveGameToSlot(SaveEquip,"Equipment",m_nCurrentPlayerIndex);

   PRINTF("SaveEquipment");
}

void USaveLoadManager::LoadEquipment() const
{
   USaveEquipment* LoadEquip = Cast<USaveEquipment>(
      UGameplayStatics::LoadGameFromSlot("Equipment", m_nCurrentPlayerIndex));

   ADiabloPlayerController* Controller = ADiabloPlayerController::Get;

   EquipmentSystemOld* EquipPtr = Controller->GetEquipment();
      
   LoadEquip->SetEquipLoadData(&EquipPtr);

   PRINTF("LoadEquipment");
}

void USaveLoadManager::SaveCharacterStat() const
{
   USaveCharacterStatus* SaveCharStat = Cast<USaveCharacterStatus>(
      UGameplayStatics::CreateSaveGameObject(USaveCharacterStatus::StaticClass()));
   //SaveCharStat->m_nLevel
   ADiabloPlayerController* Controller = ADiabloPlayerController::Get;

   SaveCharStat->m_nSlotIndex= m_nCurrentPlayerIndex;

   SaveCharStat->m_nLevel = Controller->GetUnitPawn()->GetLevel();
//m_TextUnitName
   SaveCharStat->m_TextName = Controller->GetUnitPawn()->m_TextUnitName.ToString();

   UGameplayStatics::SaveGameToSlot(SaveCharStat,"Character",m_nCurrentPlayerIndex);

}

bool USaveLoadManager::LoadCharacterStat(int index)
{
   if(!UGameplayStatics::DoesSaveGameExist("Character",index))
   {
      return false;
   }

   USaveCharacterStatus* LoadCharStat = Cast<USaveCharacterStatus>(
      UGameplayStatics::LoadGameFromSlot("Character", index));

   PRINTF("Loaded - %d - %s",index,*LoadCharStat->m_TextName);  
   m_AryLoadedCharacters[index]=LoadCharStat;

   USaveEquipment* LoadCharEquip = Cast<USaveEquipment>(UGameplayStatics::LoadGameFromSlot("Equipment", index));
   
   LoadItemDataForInstance(LoadCharEquip->m_EquipAry);
   
   m_AryLoadedEquipments[index]=LoadCharEquip;
   USaveInventory* LoadCharInven = Cast<USaveInventory>(UGameplayStatics::LoadGameFromSlot("InventoryOld", index));
   m_AryLoadedInventoryOlds[index]=LoadCharInven;
   return true;
}


void USaveLoadManager::CreateNewCharacter(PlayerCreateManagerOld* plManager)
{
   //
   int PlayerIndex =GetEmptyIndex();
   
   if(PlayerIndex==-1)
   {
      PRINTF("Empty Index: %d",PlayerIndex);
      return;
   }
   //   
   USaveCharacterStatus* SaveCharStat = Cast<USaveCharacterStatus>(
      UGameplayStatics::CreateSaveGameObject(USaveCharacterStatus::StaticClass()));
   USaveEquipment* SaveEquip = Cast<USaveEquipment>(
      UGameplayStatics::CreateSaveGameObject(USaveEquipment::StaticClass()));
   USaveInventory* SaveInven = Cast<USaveInventory>(
      UGameplayStatics::CreateSaveGameObject(USaveInventory::StaticClass()));
   //SaveCharStat
   SaveCharStat->m_SaveVersion=m_SaveVersion;
   SaveCharStat->m_nSlotIndex= PlayerIndex;
   SaveCharStat->m_nLevel = 1;
   SaveCharStat->m_TextName  = plManager->m_CurrentTextName.ToString();
   SaveCharStat->m_IndexFace = plManager->m_IndexFace;
   SaveCharStat->m_IndexHair = plManager->m_IndexHair;
   UGameplayStatics::SaveGameToSlot(SaveCharStat,"Character",PlayerIndex);
   //SaveEquip
   SaveEquip->m_SaveVersion=m_SaveVersion;
   SaveEquip->SetEquipSaveDataFromCreation(plManager->GetCurrentCharData());
   LoadItemDataForInstance(SaveEquip->m_EquipAry);
   UGameplayStatics::SaveGameToSlot(SaveEquip,"Equipment",PlayerIndex);
   //SaveInven
   SaveInven->m_SaveVersion=m_SaveVersion;
   
   UGameplayStatics::SaveGameToSlot(SaveInven,"InventoryOld",PlayerIndex);
   //
   m_AryLoadedCharacters[PlayerIndex]=SaveCharStat;
   m_AryLoadedEquipments[PlayerIndex]=SaveEquip;
   m_AryLoadedInventoryOlds[PlayerIndex]=SaveInven;
   PRINTF("Created in Index:%d",PlayerIndex);
   //
   m_OnDataCreated.ExecuteIfBound(SaveCharStat);

   //create Ned Widget
   //Set the WIdget
}

const TArray<USaveCharacterStatus*>& USaveLoadManager::GetLoadedChars() const
{
   return m_AryLoadedCharacters;
}

const TArray<USaveEquipment*>& USaveLoadManager::GetLoadedEquip() const
{
   return m_AryLoadedEquipments;
}

const TArray<USaveInventory*>& USaveLoadManager::GetLoadedInven() const
{
   return m_AryLoadedInventoryOlds;
}

int USaveLoadManager::GetEmptyIndex()
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

void USaveLoadManager::LoadItemDataForInstance(TArray<FItemInstance>& itemAry)
{
   for(auto& ItemInst : itemAry)
   {
      ItemInst.m_ItemData = UItemDataTable::GetItemDataPtr(ItemInst.m_ItemID);
   }
}
