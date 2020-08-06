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
	m_Player = Cast<APlayerDiabloCharacter>(m_PlayerController->GetPawn());
}

void UDiabloCheatManager::PrintPlayerStats()
{
	m_PlayerController->PrintStat();
}

void UDiabloCheatManager::PrintInventory()
{
	m_PlayerController->PrintInven();
}

void UDiabloCheatManager::PrintEquipment()
{
	m_PlayerController->PrintEquipment();
}

void UDiabloCheatManager::SaveInven()
{
	SaveLoadManager::Get->SaveInventory();
}

void UDiabloCheatManager::LoadInven()
{
	SaveLoadManager::Get->LoadInventory();
}
