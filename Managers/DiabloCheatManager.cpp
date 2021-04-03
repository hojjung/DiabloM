#include "DiabloCheatManager.h"
#include "DungeonManager.h"
#include "OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"


UDiabloCheatManager::UDiabloCheatManager()
{
}

void UDiabloCheatManager::InitCheatManager()
{
	Super::InitCheatManager();
	
	PRINTF("Cheat Manager Init !");
	
	m_GameManager = GetWorld()->GetGameInstance<UDiabloGameInstance>();
	
	m_PlayerController = Cast<ADiabloPlayerController>(GetWorld()->GetFirstPlayerController());
	
	if(m_PlayerController)
		m_Player = Cast<APlayerDiabloCharacter>(m_PlayerController->GetPawn());
}

void UDiabloCheatManager::BackToSelectMenu()
{
}

void UDiabloCheatManager::PrintPlayerStats()
{
}

void UDiabloCheatManager::PrintInventoryOld()
{
}


void UDiabloCheatManager::SaveTalent()
{

}

void UDiabloCheatManager::LoadTalent()
{
	
}

void UDiabloCheatManager::SaveSkill()
{
}

void UDiabloCheatManager::LoadSkill()
{
}

void UDiabloCheatManager::SaveInven()
{
}

void UDiabloCheatManager::LoadInven()
{
}

void UDiabloCheatManager::SaveEquip()
{
}

void UDiabloCheatManager::LoadEquip()
{
}

void UDiabloCheatManager::SaveCharStat()
{
}

void UDiabloCheatManager::LoadCharStat()
{
}

void UDiabloCheatManager::DeleteAllSlot()
{
}

void UDiabloCheatManager::RemoveAllEffect()
{
}

void UDiabloCheatManager::SetPlayerLevel(int levelWant)
{
}

void UDiabloCheatManager::AddPlayerExp(float wantV)
{
}

void UDiabloCheatManager::CreateDungeon(int stageLevel)
{
}

void UDiabloCheatManager::PortalToDungeon()
{
}

void UDiabloCheatManager::PortalToVillage()
{
}

void UDiabloCheatManager::DamageToPlayer(float wantV)
{
	
}

void UDiabloCheatManager::KillPlayer()
{
}

void UDiabloCheatManager::StunPlayer(float duration)
{

}

void UDiabloCheatManager::MoveStage(int index)
{
	m_GameManager->m_DungeonManager->SelectDungeon(index);
	m_GameManager->m_DungeonManager->OpenLevel();
}

void UDiabloCheatManager::Logout()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineIdentityPtr OnlineIdentity = Subsystem->GetIdentityInterface();
	OnlineIdentity->Logout(0);
	UGameplayStatics::OpenLevel(UDiabloGameInstance::Get->GetWorld(), "NewGameInitLevel", true);
}

void UDiabloCheatManager::Purchase01()
{
	m_GameManager->m_ShopManager->PurchasePacakge01();
}

void UDiabloCheatManager::Purchase02()
{
	m_GameManager->m_ShopManager->PurchasePacakge02();
}

