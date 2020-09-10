#include "DiabloCheatManager.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "SaveLoad/SaveLoadManager.h"
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
	USaveLoadManager::Get->SaveInventory();
}

void UDiabloCheatManager::LoadInven()
{
	USaveLoadManager::Get->LoadInventoryOld();
}

void UDiabloCheatManager::SaveEquip()
{
	USaveLoadManager::Get->SaveEquipment();
}

void UDiabloCheatManager::LoadEquip()
{
	USaveLoadManager::Get->LoadEquipment();
}

void UDiabloCheatManager::DeleteAllSlot()
{
	USaveLoadManager::Get->DeleteAllSlot();
}

