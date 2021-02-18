#include "MonsterSpawnManager.h"
#include "DiabloGameInstance.h"
#include "DungeonManager.h"
#include "EngineUtils.h"
#include "GridFlowMiniMap.h"
#include "Characters/DiabloPlayerController.h"

UMonsterSpawnManager::UMonsterSpawnManager()
{
	m_CurrentWorld = nullptr;
	m_NavSys = nullptr;
	m_fSpawnRadius = 1200.f;
	m_IdEnemy = "enemy";
	m_IdBossEnemy = "boss";
	m_IdSpecialEnemy = "special";
}

void UMonsterSpawnManager::UpdateWorld(UWorld* world)
{
	m_CurrentWorld = world;
	m_NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(m_CurrentWorld);
}

bool UMonsterSpawnManager::SpawnIter(TArray<FTransform>& locAry, const FDungeonStageData* selectedHorde, int level,
	UDungeonManager* dgSpawnedManager)
{
	int IterMax = FMath::Min(selectedHorde->m_AryHorde.Num(),locAry.Num());

	for (int i = 0; i < IterMax; i++)
	{
		for(int j =0; j<selectedHorde->m_AryHorde.Num();j++)
		{
			FMonsterHordeRow* MobHorde = selectedHorde->m_AryHorde[i].GetRow<FMonsterHordeRow>("");
			
			for(int k=0;k<MobHorde->m_AryMonsterEntity.Num();k++)
			{
				for(int l=0;MobHorde->m_AryMonsterEntity[k].m_nCount;l++)
				{
					FVector PointSpawn = GetRandomPoint(locAry[i].GetLocation(), m_fSpawnRadius);

					AMonsterPawn* SpawnedMob = SpawnMob(PointSpawn);

					if (!SpawnedMob)
					{
						continue;
					}

					m_AryMonsterSpawnedCurrently.Add(SpawnedMob);
					
					SpawnedMob->InitMonster(MobHorde->m_AryMonsterEntity[k].m_MonsterEntity, level, dgSpawnedManager);
				}
			}
		}
	}

	return true;
}

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

FVector UMonsterSpawnManager::GetRandomPoint(const FVector& loc, const float& radius)
{
	FNavLocation ResultLoc;

	if (!m_NavSys->GetRandomReachablePointInRadius(loc, radius, ResultLoc))
	{
		//FAIL
		return loc;
	}

	return ResultLoc;
}

AMonsterPawn* UMonsterSpawnManager::SpawnMob(FVector loc)
{
	check(UCharacterDataTable::ClassMonsterPawn);

	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	Param.bNoFail = true;
	loc.Z += 88.f;
	//88
	FRotator Rot;
	Rot.Yaw = FMath::RandRange(0.f, 360.f);

	AMonsterPawn* Mob = m_CurrentWorld->SpawnActor<AMonsterPawn
	>(UCharacterDataTable::ClassMonsterPawn, loc, Rot, Param);

	UGridFlowMiniMap::Get->AddTrackActor(m_IdEnemy,Mob);

	check(Mob);

	return Mob;
}

void UMonsterSpawnManager::MakeNamedMonster(AMonsterPawn* mob)
{
	auto AryBuff =  UDiabloGameInstance::Get->GetDungeonManager()->GetCurrentDgStageData()->m_AryClassNamedMonsterBuff;

	for(auto BB : AryBuff)
	{
		if (BB!=nullptr)
		{
			FGameplayAbilitySpec Spec = FGameplayAbilitySpec(BB, mob->GetCharacterLevel(), -1, this);
			
			mob->GetDiaAbilitySystem()->GiveAbility(Spec);
		}
	}

	mob->SetActorScale3D(FVector(1.5f,1.5f,1.5f));
	//material setting need
}
