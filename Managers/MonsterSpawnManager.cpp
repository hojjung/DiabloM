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
	m_fSpawnRadius = 1200.f;
	m_IdEnemy = "enemy";
	m_IdBossEnemy = "boss";
	m_IdSpecialEnemy = "special";
}

void UMonsterSpawnManager::StartSpawn(UWorld* world, const FDungeonDataTableRow* dgData)
{
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

		SpawnMob(FVector::ZeroVector);
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
		if (!SpawnMob(FVector::ZeroVector))
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

AMonsterPawn* UMonsterSpawnManager::SpawnMob(FVector loc)
{
	float MinX = loc.X - 500.f;
	float MaxX = loc.X + 500.f;

	float MinY = loc.Y - 500.f;
	float MaxY = loc.Y + 500.f;

	loc.X = FMath::RandRange(MinX, MaxX);
	loc.Y = FMath::RandRange(MinY, MaxY);


	FVector NewLoc = GetRandomPointFromNav(loc, 2000.f);

	const FMonsterEntityHandle& MobHandle = m_DgDataTable->m_Monster;

	const FMonsterEntity* MonData = MobHandle.GetRow<FMonsterEntity>("");

	AMonsterPawn* Mob = GetReadyMonster();

	if (!Mob)
	{
		return nullptr;
	}

	NewLoc.Z += Mob->GetCapsule()->GetScaledCapsuleHalfHeight();

	Mob->SetActorLocation(NewLoc);


	Mob->DataInject(MonData, m_DgDataTable->GetMobHp(), m_DgDataTable->GetMobGold(), EMonsterType::Normal,
	                m_DgDataTable->m_nAvoidLevel, m_DgDataTable->m_NormalDropTableHandle.GetRow<FItemDropTableRow>(""));


	return Mob;
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

	if (m_nKillCount > 10) //never spawned
	{
		PRINTF("SpawnBOss");
		//SpawnBossMob();
	}

	
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

	Mob->DataInject(MonData, m_DgDataTable->GetMobHp(), m_DgDataTable->GetMobGold(), EMonsterType::Boss,
                    m_DgDataTable->m_nAvoidLevel,
                    m_DgDataTable->m_NormalDropTableHandle.GetRow<FItemDropTableRow>(""),
                    m_DgDataTable->m_fBossMonsterStatFactor,
                    m_DgDataTable->m_fBossMonsterRenderScale);

	m_SpawnedBoss =  Mob;

	m_SpawnedBoss->m_OnDead.AddLambda(
	[&](AMonsterPawn* pawn)
	{
		m_bBossSpawned = false;;
		m_SpawnedBoss=nullptr;
		pawn->Destroy();
		PRINTF("TestLambda BossDead");
		m_OnBossBattleEnd.Broadcast(true);
	}

	);
	
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
