#include "PetDgManager.h"

#include "Managers/DiabloGameInstance.h"

#define PETDGTIME 25.f

UPetDgManager::UPetDgManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundDgTable(
		TEXT("DataTable'/Game/DataTables/Dungeon/PetDgTable.PetDgTable'"));

	m_PetTable = FoundDgTable.Object;
}

void UPetDgManager::Init()
{
	m_PetTable->GetAllRows("",m_AryTables);
}

void UPetDgManager::RequestMovePetDg(int dgLevel)
{
	m_CurrentTable = m_AryTables[dgLevel];
}

void UPetDgManager::OnLevelLoadComplete(UWorld* world)
{
	StartDungeon();
}

void UPetDgManager::StartDungeon()
{
	Super::StartDungeon();

	m_bIsMatchStarted = true;

	m_fTimer=0.f;
}

void UPetDgManager::EndDungeon(bool b)
{
	Super::EndDungeon(b);

	m_bIsMatchStarted = false;

	FTimerHandle m_TimerHandle_OnTimer;
	
	UDiabloGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(m_TimerHandle_OnTimer, this,&UPetDgManager::MoveToNormalDungeon, 2.2f,false);
}

void UPetDgManager::Tick(float deltaTime)
{
	if(!m_bIsMatchStarted)
	{
		return;
	}
	
	Super::Tick(deltaTime);

	m_fTimer+=deltaTime;

	if(m_fTimer>PETDGTIME)
	{
		EndDungeon(true);	
	}
}
FString UPetDgManager::GetOpenLevelAssetName()
{
	return TEXT("PetDg");	
}