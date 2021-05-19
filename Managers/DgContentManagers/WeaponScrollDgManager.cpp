#include "WeaponScrollDgManager.h"

#include "Engine/AssetManager.h"
#include "Managers/DiabloGameInstance.h"


UWeaponScrollDgManager::UWeaponScrollDgManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundDgTable(
		TEXT("DataTable'/Game/DataTables/Dungeon/WeaponDgTable.WeaponDgTable'"));

	m_WeaponTable = FoundDgTable.Object;

	m_SensingInterval = 3.f;
} //죽일때마다 강화석 얻음

void UWeaponScrollDgManager::Init()
{
	m_WeaponTable->GetAllRows(TEXT("MagicDgManager-No Table Data"), m_DgDataRow);

	m_LoadedMonsters.Init(TSharedPtr<FStreamableHandle>(), 12);
}

void UWeaponScrollDgManager::RequestMoveWeaponDg(int dgLevel)
{
	m_CurrentDgData = m_DgDataRow[dgLevel];
}

void UWeaponScrollDgManager::OnLevelLoadComplete(UWorld* world)
{
	Super::OnLevelLoadComplete(world);

	m_CurrentWorld = world;

	m_NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(m_CurrentWorld);

	StartDungeon();

	StartSpawn(world);
	

	//SetSpawnMonsterOnTick(true);
}

void UWeaponScrollDgManager::StartDungeon()
{
	Super::StartDungeon();

	m_bIsMatchStarted = true;

	m_fTimer = 0.f;

	m_ObtainStoneFromHere = 0;

	m_nMobCount = 0;
}

void UWeaponScrollDgManager::EndDungeon(bool b)
{	
	if (m_bIsMatchStarted)
	{
		m_bIsMatchStarted = false;
		
		UDiabloGameInstance::Get->m_GoldManager->AddWeaponStones(m_ObtainStoneFromHere.GetValue());

		m_ObtainStoneFromHere = 0;

		m_nMobCount = 0;			

		UDiabloGameInstance::Get->GetWorld()->GetTimerManager().ClearTimer(m_TimerHandle_OnTimer);

		UDiabloGameInstance::Get->GetPlChar()->SetFSM_Enable(false);

		FTimerHandle TimerHandle_OnTimer;

		UDiabloGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_OnTimer, this, &UWeaponScrollDgManager::MoveToNormalDungeon, 2.2f, false);
	}

	Super::EndDungeon(b);
}

void UWeaponScrollDgManager::Tick(float deltaTime)
{
	if (!m_bIsMatchStarted)
	{
		return;
	}

	Super::Tick(deltaTime);

	m_fTimer += deltaTime;

	float RemainTime = WEAPONDGLIMITTIME - m_fTimer;

	m_OnTimerTick.Broadcast(RemainTime);

	if (m_fTimer > WEAPONDGLIMITTIME)
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
	if(!m_bIsMatchStarted)
	{
		return;
	}
	
	UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::MonsterKill);

	int Prize = m_CurrentDgData->GetRandomPrize();

	m_ObtainStoneFromHere += Prize;

	m_nMobCount++;

	m_OnMobDead.ExecuteIfBound(m_nMobCount, m_ObtainStoneFromHere.GetValue());

	UDiabloGameInstance::Get->GetPlCon()->ShowDamageText(
		FString::Printf(TEXT("%d개 획득"), Prize), self, EDamagePopup::ObtainWeaponStone);
}

AUnitPawn* UWeaponScrollDgManager::GetNearestEnemy(const FVector& wantPos)
{
	float Dist = FLT_MAX;

	AMonsterPawn* ResultMob = nullptr;

	for (AMonsterPawn* Mob : m_AryMonsterSpawnedCurrently)
	{
		if (!Mob || Mob->IsReadyToPool() || !Mob->IsAlive())
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

void UWeaponScrollDgManager::BeginDestroy()
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

float UWeaponScrollDgManager::GetTimePercent()
{
	return m_fTimer / WEAPONDGLIMITTIME;
}

FVector UWeaponScrollDgManager::GetRandomPointFromNav(const FVector& loc, const float& radius)
{
	FNavLocation ResultLoc;

	if (!m_NavSys->GetRandomReachablePointInRadius(loc, radius, ResultLoc))
	{
		//FAIL
		return loc;
	}

	return ResultLoc;
}

AMonsterPawn* UWeaponScrollDgManager::CreateMob(FVector loc)
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

AMonsterPawn* UWeaponScrollDgManager::GetReadyMonster()
{
	AMonsterPawn* SelectedPawn = nullptr;

	for (AMonsterPawn* MPawn : m_AryMonsterSpawnedCurrently)
	{
		if (MPawn->IsReadyToPool()) //죽은애만 데려옴
		{
			SelectedPawn = MPawn;
			break;
		}
	}

	return SelectedPawn;
}

void UWeaponScrollDgManager::OnTimer()
{
	if (!m_CurrentWorld)
	{
		return;
	}
	//Spawn
	for (int i = 0; i < 5; i++)
	{
		if (!SpawnMobToLoc(FVector::ZeroVector))
		{
			break;
		}
	}

	SetTimer(m_SensingInterval);
}

void UWeaponScrollDgManager::SetTimer(const float TimeDelay)
{
	if (m_CurrentWorld && GEngine->GetNetMode(GetWorld()) < NM_Client)
	{
		m_CurrentWorld->GetTimerManager().SetTimer(m_TimerHandle_OnTimer, this, &UWeaponScrollDgManager::OnTimer,
		                                           TimeDelay,
		                                           false);
	}
}

void UWeaponScrollDgManager::SetSpawnMonsterOnTick(const bool bEnabled)
{
	if (bEnabled && m_SensingInterval > 0.f)
	{
		const float InitialDelay = (m_SensingInterval * FMath::SRand()) + KINDA_SMALL_NUMBER;

		SetTimer(InitialDelay);
	}
	else
	{
		SetTimer(0.f);
	}
}

void UWeaponScrollDgManager::Reset()
{
	for (AMonsterPawn* Mob : m_AryMonsterSpawnedCurrently)
	{
		if (!Mob)
		{
			continue;
		}
		Mob->Destroy();
	}

	m_AryMonsterSpawnedCurrently.Reset();
}

AMonsterPawn* UWeaponScrollDgManager::SpawnMobToLoc(FVector loc)
{
	AMonsterPawn* Mob = GetReadyMonster();

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

	const FMonsterEntityHandle& MobHandle = m_CurrentDgData->m_Monsters.GetRandom();

	const FMonsterEntity* MonData = MobHandle.GetRow<FMonsterEntity>("");

	NewLoc.Z += Mob->GetCapsule()->GetScaledCapsuleHalfHeight();

	Mob->SetActorLocation(NewLoc);

	float GoldScale = 1.f;

	float StatScale = 1.f;

	Mob->DataInject(MonData, m_CurrentDgData->GetMobHp(), m_CurrentDgData->GetMobGold(), EMonsterType::Normal,
	                StatScale,
	                MonData->m_fScale, GoldScale);

	return Mob;
}

void UWeaponScrollDgManager::StartSpawn(UWorld* world)
{
	for (auto& Handle : m_LoadedMonsters)
	{
		if (Handle.Get())
		{
			Handle.Get()->ReleaseHandle();
		}
	}

	FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();

	for (int i = 0; i < m_CurrentDgData->m_Monsters.Num(); i++)
	{
		StreamableManager.LoadSynchronous(m_CurrentDgData->m_Monsters[i].GetRow<FMonsterEntity>("")->m_MonsterMeshSoft,
		                                  true,
		                                  &m_LoadedMonsters[i]);
	}

	m_CurrentWorld = world;

	m_NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(m_CurrentWorld);

	Reset();

	int i = 0;

	while (i++ < MonsterPoolCount)
	{
		AMonsterPawn* SpawnedMob = CreateMob(FVector::ZeroVector);

		m_AryMonsterSpawnedCurrently.Add(SpawnedMob);

		SpawnedMob->m_OnDeathAnimBefore.AddUObject(this, &UWeaponScrollDgManager::OnMonsterDead);

		SpawnMobToLoc(FVector::ZeroVector);
	}

	SetSpawnMonsterOnTick(true);
}

int UWeaponScrollDgManager::GetObtainedStone()
{
	return m_ObtainStoneFromHere.GetValue();
}
