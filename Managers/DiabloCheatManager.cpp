#include "DiabloCheatManager.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Item/Inventory.h"
#include "SaveLoad/SaveLoadManager.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/StartMap/PlayerCreateManager.h"


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
	TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
	
	USaveLoadManager::Get->SaveInventory(UPlayerCreateManager::Get->m_CurrentSelectSlot,DiaPC->GetInven()->GetItemAry());
}

void UDiabloCheatManager::LoadInven()
{
	USaveLoadManager::Get->LoadInventory(UPlayerCreateManager::Get->m_CurrentSelectSlot);
	USaveLoadManager::Get->SetLoadedInvenDataToPlayer(UPlayerCreateManager::Get->m_CurrentSelectSlot);
}

void UDiabloCheatManager::SaveEquip()
{
	TArray<FItemInstance> AryEquip;

	for(FEquipSlot* Slot : ADiabloPlayerController::Get->GetEquipment()->GetArySlotPtr())
	{
		AryEquip.Emplace(Slot->m_Item);
	}
	
	USaveLoadManager::Get->SaveEquipment(UPlayerCreateManager::Get->m_CurrentSelectSlot,AryEquip);
	
}

void UDiabloCheatManager::LoadEquip()
{
	USaveLoadManager::Get->LoadEquipment(UPlayerCreateManager::Get->m_CurrentSelectSlot);
	USaveLoadManager::Get->SetLoadedEquipDataToPlayer(UPlayerCreateManager::Get->m_CurrentSelectSlot);
}

void UDiabloCheatManager::SaveCharStat()
{
	TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
	TWeakObjectPtr<APlayerDiabloCharacter> DiaPl = DiaPC->GetPlayerPawn();
	FText Name = DiaPl->m_TextUnitName;
	int Lev= DiaPl->GetLevel();
	int Hair= DiaPl->m_HairIndex;
	int Face= DiaPl->m_FaceIndex;
	
	USaveLoadManager::Get->SaveCharacterStat(UPlayerCreateManager::Get->m_CurrentSelectSlot,Lev,Name,Face,Hair,USaveLoadManager::Get->GetCurrentPlayerClassName(),DiaPl->m_fCurrentExp);
}

void UDiabloCheatManager::LoadCharStat()
{
	USaveLoadManager::Get->LoadCharStat(UPlayerCreateManager::Get->m_CurrentSelectSlot);
	USaveLoadManager::Get->SetLoadedCharDataToPlayer(UPlayerCreateManager::Get->m_CurrentSelectSlot);
}

void UDiabloCheatManager::DeleteAllSlot()
{
	USaveLoadManager::Get->DeleteAllSlot();
}

void UDiabloCheatManager::RemoveAllEffect()
{
	TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
	TWeakObjectPtr<APlayerDiabloCharacter> DiaPl = DiaPC->GetPlayerPawn();
	DiaPl->RemoveAllEffect();
}

void UDiabloCheatManager::SetPlayerLevel(int levelWant)
{TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
	TWeakObjectPtr<APlayerDiabloCharacter> DiaPl = DiaPC->GetPlayerPawn();
	DiaPl->SetCharacterLevel(levelWant);
}

void UDiabloCheatManager::AddPlayerExp(float wantV)
{TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
	TWeakObjectPtr<APlayerDiabloCharacter> DiaPl = DiaPC->GetPlayerPawn();
	DiaPl->EarnExp(wantV);
}

