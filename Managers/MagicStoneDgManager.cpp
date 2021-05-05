#include "MagicStoneDgManager.h"
#include "DiabloGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Datas/DungeonDataTable.h"

#define MAGICDGTIME 50

UMagicStoneDgManager::UMagicStoneDgManager()
{
	m_nDgLevel = 0;
	static ConstructorHelpers::FClassFinder<UCameraShake> FoundCamshake(
		TEXT("Blueprint'/Game/Blueprints/BP_StompShake.BP_StompShake_C'"));
	m_ClassShake = FoundCamshake.Class;
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundDgTable(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaAccessory01.GachaAccessory01'"));

	m_MagicDgTable = FoundDgTable.Object;
}

void UMagicStoneDgManager::Init()
{
	m_MagicDgTable->GetAllRows(TEXT("MagicDgManager-No Table Data"), m_DgDataRow);
}

void UMagicStoneDgManager::RequestMoveMagicStoneDg(int dgLevel)
{
	m_nDgLevel = dgLevel;

	m_CurrentDgData = m_DgDataRow[m_nDgLevel];

	UGameplayStatics::OpenLevel(UDiabloGameInstance::Get->GetWorld(),TEXT("MagicDragonNest"), true);

	m_LevelLoadHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(
		this, &UMagicStoneDgManager::OnLevelLoaded);
}

void UMagicStoneDgManager::OnLevelLoaded(UWorld* world)
{
	PRINTF("MagicStoneManager! World:%s", *world->GetMapName());

	FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(m_LevelLoadHandle);

	UGameplayStatics::GetPlayerController(UDiabloGameInstance::Get->GetWorld(), 0)->ClientPlayCameraShake(m_ClassShake);

	FMonsterEntity* DragonEntity = m_CurrentDgData->m_Monsters[0].GetRow<FMonsterEntity>("");
	//
	FVector Loc = FVector(290, 290, 102);

	FRotator Rot = FRotator(0, 130, 0);
	//
	FActorSpawnParameters Param;

	Param.bNoFail = true;

	m_SpawnedMagicDragon = world->SpawnActor<AMonsterPawn>(AMonsterPawn::StaticClass(), Loc, Rot, Param);

	m_SpawnedMagicDragon->DataInject(DragonEntity, m_CurrentDgData->GetMobHp(), m_CurrentDgData->GetMobGold(),
	                                 EMonsterType::MagicDragon, 1, 1, 1);

	m_SpawnedMagicDragon->SetMagicStones(FMath::RandRange(m_CurrentDgData->m_nPrizeMagicStoneMin,
	                                                      m_CurrentDgData->m_nPrizeMagicStoneMax));

	m_SpawnedMagicDragon->m_OnDead.AddUObject(this,&UMagicStoneDgManager::EndMagicDgSuccess);

	StartMagicDg();
}


void UMagicStoneDgManager::Tick(float delta_seconds)
{
	//서버에서 받아온거
	if (!m_bIsMatchStarted)
	{
		return;
	}

	m_fTimer += delta_seconds;

	float TimeRemain = MAGICDGTIME - m_fTimer;

	m_OnTick.ExecuteIfBound(TimeRemain);

	if (m_fTimer > MAGICDGTIME)
	{
		EndMagicDgFail();
	}
}


void UMagicStoneDgManager::StartMagicDg()
{
	APlayerDiabloCharacter* PlChar = UDiabloGameInstance::Get->GetPlChar();

	PlChar->FocusTarget(m_SpawnedMagicDragon);

	m_fTimer = 0.f;
	
	m_bIsMatchStarted = true;
}

void UMagicStoneDgManager::EndMagicDgSuccess(AMonsterPawn* dragonDead)
{
	PRINTF("사냥성공");
	m_bIsMatchStarted = false;
}

void UMagicStoneDgManager::EndMagicDgFail()
{
	PRINTF("사냥실패");

	m_bIsMatchStarted = false;
}
void UMagicStoneDgManager::AddMagicStones(int magicStones)
{
	
}
