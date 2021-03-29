#include "MonsterSpawnManager.h"
#include "DiabloGameInstance.h"
#include "DungeonManager.h"
#include "EngineUtils.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"

UMonsterSpawnManager::UMonsterSpawnManager()
{
	//Blueprint'/Game/Blueprints/BP_StompShake.BP_StompShake'
	static ConstructorHelpers::FClassFinder<UCameraShake> FoundCamshake(TEXT("Blueprint'/Game/Blueprints/BP_StompShake.BP_StompShake_C'"));
	m_ClassShake = FoundCamshake.Class;
	m_bBossSpawned = false;
	m_SensingInterval = 5.f;
	m_CurrentWorld = nullptr;
	m_NavSys = nullptr;
	m_fSpawnRadius = 6200.f;
	m_IdEnemy = "enemy";
	m_IdBossEnemy = "boss";
	m_IdSpecialEnemy = "special";

	static ConstructorHelpers::FObjectFinder<UDataTable> FoundData(TEXT("DataTable'/Game/DataTables/Entities/MonsterTable.MonsterTable'"));
	m_GoldGoblinEntity = FoundData.Object->FindRow<FMonsterEntity>("GoldGoblin","");
	//(DataTable=DataTable'"/Game/DataTables/Entities/MonsterTable.MonsterTable"',RowName="GoldGoblin")

	m_nGoldGoblinSpawnCount=0;
}

void UMonsterSpawnManager::StartSpawn(UWorld* world, const FDungeonDataTableRow* dgData)
{
	m_nKillCount=0;
	
	m_nGoldGoblinSpawnCount = FMath::RandRange(5,10);

	m_nGoldGoblinSpawnCount+=m_nKillCount;
	
	m_CurrentWorld = world;

	m_NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(m_CurrentWorld);

	m_DgDataTable = dgData;

	if (!m_DgDataTable)
	{
		return;
	}

	Reset();

	int i = 0;

	while (i++ < m_nMonsterPoolCount)
	{
		AMonsterPawn* SpawnedMob = CreateMob(FVector::ZeroVector);

		m_AryMonsterSpawnedCurrently.Add(SpawnedMob);

		SpawnMobToLoc(FVector::ZeroVector);
	}

	SetSensingUpdatesEnabled(true);
}

void UMonsterSpawnManager::SetSensingUpdatesEnabled(const bool bEnabled)
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

void UMonsterSpawnManager::SetTimer(const float TimeInterval)
{
	if (m_CurrentWorld && GEngine->GetNetMode(GetWorld()) < NM_Client)
	{
		m_CurrentWorld->GetTimerManager().SetTimer(m_TimerHandle_OnTimer, this, &UMonsterSpawnManager::OnTimer,
		                                           TimeInterval,
		                                           false);
	}
}

void UMonsterSpawnManager::SetSensingInterval(const float newSensingInterval)
{
	if (m_SensingInterval != newSensingInterval)
	{
		m_SensingInterval = newSensingInterval;

		if (m_CurrentWorld)
		{
			if (m_SensingInterval <= 0.f)
			{
				SetTimer(0.f);
			}
			else
			{
				float CurrentElapsed = m_CurrentWorld->GetTimerManager().GetTimerElapsed(m_TimerHandle_OnTimer);

				CurrentElapsed = FMath::Max(0.f, CurrentElapsed);

				if (CurrentElapsed < m_SensingInterval)
				{
					SetTimer(m_SensingInterval - CurrentElapsed);
				}
				else if (CurrentElapsed > m_SensingInterval)
				{
					SetTimer(KINDA_SMALL_NUMBER);
				}
			}
		}
	}
}

void UMonsterSpawnManager::OnTimer()
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
};


void UMonsterSpawnManager::Reset()
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

AMonsterPawn* UMonsterSpawnManager::GetReadyMonster()
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
AMonsterPawn* UMonsterSpawnManager::SpawnMobToLoc(FVector loc)
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

	const FMonsterEntityHandle& MobHandle = m_DgDataTable->m_Monster;

	const FMonsterEntity* MonData = MobHandle.GetRow<FMonsterEntity>("");

	NewLoc.Z += Mob->GetCapsule()->GetScaledCapsuleHalfHeight();

	Mob->SetActorLocation(NewLoc);

	float GoldScale = 1.f;

	float StatScale = 1.f;
	
	if(m_nKillCount >= m_nGoldGoblinSpawnCount)
	{
		m_nGoldGoblinSpawnCount = FMath::RandRange(75,150);

		m_nGoldGoblinSpawnCount+=m_nKillCount;

		MonData = m_GoldGoblinEntity;

		GoldScale = 15.f;

		StatScale = 5.f;
	}
	
	Mob->DataInject(MonData, m_DgDataTable->GetMobHp(), m_DgDataTable->GetMobGold(), EMonsterType::Normal,
           m_DgDataTable->m_NormalDropTableHandle.GetRow<FItemDropTableRow>(""),StatScale,MonData->m_fScale,GoldScale);	
	
	return Mob;
}

void UMonsterSpawnManager::OnBossDead(AMonsterPawn* pawn)
{
	m_SpawnedBoss->m_OnDead.Remove(m_BossDeleHandle);
	m_bBossSpawned = false;;
	m_SpawnedBoss=nullptr;
	pawn->Destroy();
	m_OnBossBattleEnd.Broadcast(true);
	UDiabloGameInstance::Get->m_PlayerUpgradeManager->ClearCooldownAllSkill();
	UDiabloGameInstance::Get->m_DungeonManager->LevelUpDungeon();
}

AMonsterPawn* UMonsterSpawnManager::GetNearestMonster(const FVector& wantPos)
{
	if(m_bBossSpawned && m_SpawnedBoss)
	{
		return m_SpawnedBoss;
	}
	
	float Dist = FLT_MAX;

	AMonsterPawn* ResultMob = nullptr;

	for (AMonsterPawn* Mob : m_AryMonsterSpawnedCurrently)
	{
		bool b2 = !Mob;
		bool b3 = Mob->IsReadyToPool(); //공중에있다는뜻
		bool b4 = !Mob->IsAlive();

		if (b2 || b3 || b4)
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

FVector UMonsterSpawnManager::GetRandomPointFromNav(const FVector& loc, const float& radius)
{
	FNavLocation ResultLoc;

	if (!m_NavSys->GetRandomReachablePointInRadius(loc, radius, ResultLoc))
	{
		//FAIL
		return loc;
	}

	return ResultLoc;
}

AMonsterPawn* UMonsterSpawnManager::CreateMob(FVector loc)
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

void UMonsterSpawnManager::MakeNamedMonster(AMonsterPawn* mob)
{
	//material setting need
}

void UMonsterSpawnManager::AddKillCount()
{
	m_nKillCount++;

		
}

void UMonsterSpawnManager::SpawnBossMob()
{
	//
	UGameplayStatics::GetPlayerController(UDiabloGameInstance::Get->GetWorld(),0)->ClientPlayCameraShake(m_ClassShake);
	//
	FVector PlayerLoc = UGameplayStatics::GetPlayerPawn(UDiabloGameInstance::Get->GetWorld(),0)->GetActorLocation();

	FVector NewLoc = GetRandomPointFromNav(PlayerLoc, 1000.f);
	
	auto* Mob = CreateMob(PlayerLoc);

	const FMonsterEntityHandle& MobHandle = m_DgDataTable->m_Monster;

	const FMonsterEntity* MonData = MobHandle.GetRow<FMonsterEntity>("");

	//m_fBossMonsterStatFactor

	NewLoc.Z += Mob->GetCapsule()->GetScaledCapsuleHalfHeight();

	Mob->SetActorLocation(NewLoc);

	Mob->DataInject(MonData, m_DgDataTable->GetMobHp(), m_DgDataTable->GetMobGold(), EMonsterType::Boss,m_DgDataTable->m_NormalDropTableHandle.GetRow<FItemDropTableRow>(""),10,MonData->m_fBossMonsterRenderScale,10.f);

	m_SpawnedBoss =  Mob;

	m_BossDeleHandle =m_SpawnedBoss->m_OnDead.AddUObject(this,&UMonsterSpawnManager::OnBossDead);
	
	APlayerDiabloCharacter* Pl = Cast<APlayerDiabloCharacter> (UGameplayStatics::GetPlayerPawn(UDiabloGameInstance::Get->GetWorld(),0));

	Pl->FocusTarget(m_SpawnedBoss);

	m_bBossSpawned = true;
}

void UMonsterSpawnManager::FailBossKill()
{
	//쿨타임 생기고
	//보스 없애기
	m_bBossSpawned = false;;
	m_SpawnedBoss->Destroy();
	m_SpawnedBoss=nullptr;
	m_OnBossBattleEnd.Broadcast(false);
}
