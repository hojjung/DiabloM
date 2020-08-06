#include "SaveLoadManager.h"
#include "Characters/DiabloPlayerController.h"
#include "DiabloM.h"
#include "Kismet/GameplayStatics.h"
#include "SaveLoad/SaveInventory.h"
#include "SaveLoad/SaveCharacterStatus.h"


SaveLoadManager* SaveLoadManager::Get=nullptr;


void SaveLoadManager::CreateSaveLoadInstance()
{
   SaveLoadManager::Get=this;
}

SaveLoadManager::~SaveLoadManager()
{
   SaveLoadManager::Get=nullptr;
}

void SaveLoadManager::SaveInventory()const
{
   auto* SaveInven =Cast<USaveInventory>( UGameplayStatics::CreateSaveGameObject(USaveInventory::StaticClass()));

   auto* Controller =ADiabloPlayerController::Get;
   
   SaveInven->SetSaveData(Controller->GetInven());

   UGameplayStatics::SaveGameToSlot(SaveInven,"Inventory",0);

   PRINTF("SaveInventory");
}

void SaveLoadManager::LoadInventory()const
{
   auto* LoadInven =Cast<USaveInventory>(UGameplayStatics::LoadGameFromSlot("Inventory",0));

   auto* Controller =ADiabloPlayerController::Get;
   
   LoadInven->SetInvenLoadData(Controller->GetInven());

   PRINTF("LoadInventory");
}
