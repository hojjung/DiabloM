#include "MonsterSpawnManager.h"
#include "DiabloGameInstance.h"
#include "DungeonManager.h"
#include "EngineUtils.h"
#include "Characters/DiabloPlayerController.h"

UMonsterSpawnManager::UMonsterSpawnManager()
{
	m_SensingInterval = 5.f;
	m_CurrentWorld = nullptr;
	m_NavSys = nullptr;
	m_fSpawnRadius = 1200.f;
	m_IdEnemy = "enemy";
	m_IdBossEnemy = "boss";
	m_IdSpecialEnemy = "special";
}

void UMonsterSpawnManager::StartSpawn(UWorld* world,const FDungeonDataTableRow* dgData)
{
	m_CurrentWorld = world;
	
	m_NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(m_CurrentWorld);

	m_DgDataTable = dgData;

	if(!m_DgDataTable)
	{
		return;
	}
	
	Reset();

	int i=0;

	while (i++<m_nMonsterPoolCount)
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
	for(int i=0; i< 5;i++)
	{
	   SpawnMob(FVector::ZeroVector);
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

	for(AMonsterPawn* MPawn : m_AryMonsterSpawnedCurrently)
	{
		if(MPawn->IsReadyToPool() && !MPawn->IsAlive())//죽은애만 데려옴
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

	loc.X = FMath::RandRange(MinX,MaxX);
	loc.Y = FMath::RandRange(MinY,MaxY);
	
	
	FVector NewLoc = GetRandomPointFromNav(loc, 2000.f);
	
	const FMonsterEntityHandle& RandomMob = m_DgDataTable->m_AryMonster.GetRandom();

	const FMonsterEntity* MonData = RandomMob.GetRow<FMonsterEntity>("");

	AMonsterPawn* Mob = GetReadyMonster();

	if(!Mob)
	{
		PRINTF("Skip Theres no available Monster");
		return nullptr;
	}

	NewLoc.Z+= Mob->GetCapsule()->GetScaledCapsuleHalfHeight();

	Mob->SetActorLocation(NewLoc);

	Mob->DataInject(MonData,100.f);
	
	Mob->SetAcive(true);

	PRINTF("SpawnedMob!");
	//Calculate Health
	
	return Mob;
}

AMonsterPawn* UMonsterSpawnManager::GetNearestMonster(const FVector& wantPos, bool bSeeHideObj,
                                                      AMonsterPawn* ignoreActor)
{
	float Dist = FLT_MAX;

	AMonsterPawn* ResultMob = nullptr;

	for (AMonsterPawn* Mob : m_AryMonsterSpawnedCurrently)
	{
		if (Mob == ignoreActor || !Mob || (Mob->IsHidden() && !bSeeHideObj) || !Mob->IsAlive())
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
	Rot.Pitch=0.f;
	Rot.Roll=0.f;
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
