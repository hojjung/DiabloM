#include "PetDgManager.h"
#include "Engine/AssetManager.h"
#include "Managers/DiabloGameInstance.h"



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

void UPetDgManager::BeginDestroy()
{
	Super::BeginDestroy();
	
	for (auto& Handle : m_LoadedMonsters)
	{
		if (Handle.Get())
		{
			Handle.Get()->ReleaseHandle();
		}
	}
}

void UPetDgManager::RequestMovePetDg(int dgLevel)
{
	m_CurrentTable = m_AryTables[dgLevel];
}

void UPetDgManager::OnLevelLoadComplete(UWorld* world)
{
	m_CurrentWorld = world;
	
	m_NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(m_CurrentWorld);
	
	StartDungeon();
}

void UPetDgManager::StartDungeon()
{
	Super::StartDungeon();

	m_bIsMatchStarted = true;

	m_fTimer=0.f;

	SpawnPetDgMonsters(20);
}

void UPetDgManager::SpawnPetDgMonsters(int count)
{
	m_nMaxKillCount = count;
	
	m_LoadedMonsters.Init(TSharedPtr<FStreamableHandle>(), m_nMaxKillCount);
	
	FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();

	for (int i = 0; i < m_CurrentTable->m_Monsters.Num(); i++)
	{
		StreamableManager.LoadSynchronous(m_CurrentTable->m_Monsters[i].GetRow<FMonsterEntity>("")->m_MonsterMeshSoft, true,&m_LoadedMonsters[i]);
	}

	int i = 0;

	while (i++ < m_nMaxKillCount)
	{
		AMonsterPawn* SpawnedMob = SpawnMobToLoc(FVector::ZeroVector);

		m_AryMonsterSpawnedCurrently.Add(SpawnedMob);

		SpawnedMob->m_OnDeathAnimBefore.AddUObject(this, &UPetDgManager::AddKillCount);
	}
}

void UPetDgManager::EndDungeon(bool b)
{
	m_fTimer=0;

	if(m_bIsMatchStarted)
	{
		Super::EndDungeon(b);

		m_PetTicket+=GetCurrentReward();
		
		FTimerHandle m_TimerHandle_OnTimer;
		
		UDiabloGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(m_TimerHandle_OnTimer, this,&UPetDgManager::MoveToNormalDungeon, 2.2f,false);
	}

	m_nCurrentKillCount=0;

	m_nMaxKillCount=0;

	m_bIsMatchStarted = false;
}

void UPetDgManager::Tick(float deltaTime)
{
	if(!m_bIsMatchStarted)
	{
		return;
	}
	
	Super::Tick(deltaTime);

	m_fTimer+=deltaTime;

	m_OnTickTimer.Broadcast(m_fTimer);

	if(m_fTimer>PETDGTIME)
	{
		EndDungeon(false);	
	}
}
FString UPetDgManager::GetOpenLevelAssetName()
{
	return TEXT("PetDg");	
}

FVector UPetDgManager::GetRandomPointFromNav(const FVector& loc, const float& radius)
{
	FNavLocation ResultLoc;

	if (!m_NavSys->GetRandomReachablePointInRadius(loc, radius, ResultLoc))
	{
		//FAIL
		return loc;
	}

	return ResultLoc;
}

void UPetDgManager::AddKillCount(AMonsterPawn*)
{
	if(!m_bIsMatchStarted)
	{
		return;
	}
	
	m_nCurrentKillCount++;

	m_OnMonsterKilled.ExecuteIfBound();

	if(m_nCurrentKillCount>=m_nMaxKillCount)
	{
		EndDungeon(true);
	}
}

AMonsterPawn* UPetDgManager::CreateMob(FVector loc)
{
	FActorSpawnParameters Param;

	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	Param.bNoFail = true;

	//88
	FRotator Rot;
	Rot.Pitch = 0.f;
	Rot.Roll = 0.f;
	Rot.Yaw = FMath::RandRange(-360.f, 360.f);

	AMonsterPawn* Mob = m_CurrentWorld->SpawnActor<AMonsterPawn>(AMonsterPawn::StaticClass(), loc, Rot, Param);
	Mob->SetAcive(false);

	check(Mob);

	return Mob;
}

AUnitPawn* UPetDgManager::GetNearestEnemy(const FVector& wantPos)
{
	float Dist = FLT_MAX;

	AMonsterPawn* ResultMob = nullptr;

	for (AMonsterPawn* Mob : m_AryMonsterSpawnedCurrently)
	{
		if (!Mob  || !Mob->IsAlive())
		{
			continue;
		}

		float DistNew = FVector::DistSquared2D(Mob->GetActorLocation(), wantPos);

		if (Dist >= DistNew)
		{
			Dist = DistNew;

			ResultMob = Mob;
		}
	}

	return ResultMob;
}

int UPetDgManager::GetCurrentReward()
{
	return m_CurrentTable->m_nTicket;
}


AMonsterPawn* UPetDgManager::SpawnMobToLoc(FVector loc)
{
	AMonsterPawn* Mob = CreateMob(FVector::ZeroVector);

	if (!Mob)
	{
		return nullptr;
	}

	float MinX = loc.X - 500.f;
	float MaxX = loc.X + 500.f;

	float MinY = loc.Y - 500.f;
	float MaxY = loc.Y + 500.f;

	loc.X = FMath::RandRange(MinX, MaxX);
	loc.Y = FMath::RandRange(MinY, MaxY);


	FVector NewLoc = GetRandomPointFromNav(loc, 4000.f);

	const FMonsterEntityHandle& MobHandle = m_CurrentTable->m_Monsters.GetRandom();

	const FMonsterEntity* MonData = MobHandle.GetRow<FMonsterEntity>("");

	NewLoc.Z += Mob->GetCapsule()->GetScaledCapsuleHalfHeight();

	Mob->SetActorLocation(NewLoc);

	float GoldScale = 1.f;

	float StatScale = FMath::RandRange(0.8f,1.2f);

	Mob->DataInject(MonData, m_CurrentTable->GetMobHp(), 0, EMonsterType::Normal, StatScale,
					MonData->m_fScale, GoldScale);

	return Mob;
}

float UPetDgManager::GetMobCountPercent()
{
	return static_cast<float>(m_nCurrentKillCount) / static_cast<float>(m_nMaxKillCount);	
}

FString UPetDgManager::GetMobRemainCountStr()
{
	return FString::Printf(TEXT("처치 몬스터 %d/%d"),m_nCurrentKillCount,m_nMaxKillCount);
}
