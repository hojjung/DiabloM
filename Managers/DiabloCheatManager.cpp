#include "DiabloCheatManager.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "SaveLoad/SaveLoadManagerOld.h"
#include "Managers/DiabloGameInstance.h"

void UDiabloCheatManager::InitCheatManager()
{
	Super::InitCheatManager();
	PRINTF("Cheat Manager Init !");
	m_GameManager = GetWorld()->GetGameInstance<UDiabloGameInstance>();
	m_PlayerController = Cast<ADiabloPlayerController>(GetWorld()->GetFirstPlayerController());
	if(m_PlayerController)
		m_Player = Cast<APlayerDiabloCharacter>(m_PlayerController->GetPawn());
}

void UDiabloCheatManager::PrintPlayerStats()
{
	m_PlayerController->PrintStat();
}

void UDiabloCheatManager::PrintInventoryOld()
{
	m_PlayerController->PrintInven();
}

void UDiabloCheatManager::PrintEquipment()
{
	m_PlayerController->PrintEquipment();
}

void UDiabloCheatManager::SaveInven()
{
	SaveLoadManagerOld::Get->SaveInventory();
}

void UDiabloCheatManager::LoadInven()
{
	SaveLoadManagerOld::Get->LoadInventoryOld();
}

void UDiabloCheatManager::SaveEquip()
{
	SaveLoadManagerOld::Get->SaveEquipment();
}

void UDiabloCheatManager::LoadEquip()
{
	SaveLoadManagerOld::Get->LoadEquipment();
}

void UDiabloCheatManager::DeleteAllSlot()
{
	SaveLoadManagerOld::Get->DeleteAllSlot();
}

