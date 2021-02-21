// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "NavigationPath.h"
#include "NavigationData.h"
#include "NavigationSystem.h"
#include "Characters/MonsterPawn.h"
#include "Datas/DungeonDataTable.h"


#include "MonsterSpawnManager.generated.h"

UCLASS()
class DIABLOM_API UMonsterSpawnManager : public UObject
{
	GENERATED_BODY()


public:
	UMonsterSpawnManager();
	
protected:
	static const int m_nMonsterPoolCount = 33;
	UPROPERTY()
	UNavigationSystemV1* m_NavSys;
	UPROPERTY()
	UWorld* m_CurrentWorld;
	UPROPERTY()
	TArray<AMonsterPawn*> m_AryMonsterSpawnedCurrently;

	FName m_IdEnemy;

	FName m_IdBossEnemy;

	FName m_IdSpecialEnemy;
	
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

	AMonsterPawn* GetNearestMonster(const FVector& wantPos,bool bSeeHideObj,AMonsterPawn* ignoreActor =nullptr);
	
	void StartSpawn(UWorld* world,const FDungeonDataTableRow* dgData);
};


