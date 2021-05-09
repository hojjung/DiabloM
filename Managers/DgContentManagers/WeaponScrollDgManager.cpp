#include "WeaponScrollDgManager.h"

#include "Managers/DiabloGameInstance.h"

#define WEAPONDGLIMITTIME 70.f

UWeaponScrollDgManager::UWeaponScrollDgManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundDgTable(
		TEXT("DataTable'/Game/DataTables/Dungeon/WeaponDgTable.WeaponDgTable'"));

	m_WeaponTable = FoundDgTable.Object;
}//죽일때마다 강화석 얻음

void UWeaponScrollDgManager::Init()
{
	m_WeaponTable->GetAllRows(TEXT("MagicDgManager-No Table Data"), m_DgDataRow);
}

void UWeaponScrollDgManager::RequestMoveWeaponDg(int dgLevel)
{
	m_CurrentDgData = m_DgDataRow[dgLevel];
}

void UWeaponScrollDgManager::OnLevelLoadComplete(UWorld* world)
{
	StartDungeon();
}

void UWeaponScrollDgManager::StartDungeon()
{
	Super::StartDungeon();
	
	m_bIsMatchStarted = true;

	m_fTimer=0.f;
}

void UWeaponScrollDgManager::EndDungeon(bool b)
{
	Super::EndDungeon(b);
	
	m_bIsMatchStarted = false;

	FTimerHandle m_TimerHandle_OnTimer;
	
	UDiabloGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(m_TimerHandle_OnTimer, this,&UWeaponScrollDgManager::MoveToNormalDungeon, 2.2f,false);
}

void UWeaponScrollDgManager::Tick(float deltaTime)
{
	if(!m_bIsMatchStarted)
	{
		return;
	}
	
	Super::Tick(deltaTime);

	m_fTimer+=deltaTime;

	if(m_fTimer>WEAPONDGLIMITTIME)
	{
		EndDungeon(true);	
	}
}

FString UWeaponScrollDgManager::GetOpenLevelAssetName()
{
	return TEXT("WeaponDg");	
}

void UWeaponScrollDgManager::OnMonsterDead(AMonsterPawn* self)
{
	UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::MonsterKill);

	int Prize = m_CurrentDgData->GetRandomPrize();

	m_OnObtainBounty.ExecuteIfBound(Prize);

	m_WeaponStones+=Prize;
}
