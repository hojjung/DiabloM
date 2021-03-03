// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "NavigationPath.h"
#include "NavigationData.h"
#include "NavigationSystem.h"
#include "Characters/MonsterPawn.h"
#include "Datas/DungeonDataTable.h"


#include "MonsterSpawnManager.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBossBattleEnd,bool);
UCLASS()
class DIABLOM_API UMonsterSpawnManager : public UObject
{
	GENERATED_BODY()


public:
	UMonsterSpawnManager();

	FOnBossBattleEnd m_OnBossBattleEnd;
	
protected:
	static const int m_nMonsterPoolCount = 11;
	UPROPERTY()
	UNavigationSystemV1* m_NavSys;
	UPROPERTY()
	UWorld* m_CurrentWorld;
	UPROPERTY()
	UParticleSystem* m_ParticleCoin;
	UPROPERTY()
	TArray<AMonsterPawn*> m_AryMonsterSpawnedCurrently;
	UPROPERTY()
	int m_nKillCount;
	UPROPERTY()
	AMonsterPawn* m_SpawnedBoss;
	UPROPERTY()
	TSubclassOf<UCameraShake> m_ClassShake;

	FName m_IdEnemy;

	FName m_IdBossEnemy;

	FName m_IdSpecialEnemy;

	bool m_bBossSpawned;
	
	const FDungeonDataTableRow* m_DgDataTable;

	float m_fSpawnRadius;

	float m_SensingInterval;
	
	FTimerHandle m_TimerHandle_OnTimer;

	
	
protected:
	FVector GetRandomPointFromNav(const FVector& loc,const float& radius);
	
	AMonsterPawn* CreateMob(FVector loc);

	void MakeNamedMonster(AMonsterPawn* mob);
	//void MakeBossMonster(AMonsterPawn* mob);
	AMonsterPawn* GetReadyMonster();

	void OnTimer();

	void SetTimer(const float TimeDelay);
	
	void SetSensingInterval(const float newSensingInterval);

	void SetSensingUpdatesEnabled(const bool bEnabled);

	
	
public:
	AMonsterPawn* SpawnMob(FVector loc);
	
	void Reset();

	FORCEINLINE TArray<AMonsterPawn*>& GetCurrentMonsters()
	{
		return m_AryMonsterSpawnedCurrently;
	}

	AMonsterPawn* GetNearestMonster(const FVector& wantPos);
	
	void StartSpawn(UWorld* world,const FDungeonDataTableRow* dgData);

	void AddKillCount();

	void SpawnBossMob();

	void FailBossKill();

	AMonsterPawn* GetBossMob() const
	{
		return m_SpawnedBoss;
	}

	bool IsBossBattleIn()
	{
		return m_bBossSpawned && GetBossMob() && GetBossMob()->IsAlive();
	}
};


