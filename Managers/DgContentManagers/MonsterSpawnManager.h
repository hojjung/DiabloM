// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "NavigationPath.h"
#include "NavigationData.h"
#include "NavigationSystem.h"
#include "Characters/Pawns/UnitPawn.h"
#include "Datas/DungeonDataTable.h"


#include "MonsterSpawnManager.generated.h"

UCLASS(Abstract)
class DIABLOM_API UMonsterSpawnManager : public UObject
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE(FOnBattleStart);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnBattleEnd,bool);
	

	FOnBattleStart m_OnBattleStart;

	FOnBattleEnd m_OnBattleEnd;
	
	virtual AUnitPawn* GetNearestEnemy(const FVector& wantPos)
	{
		return nullptr;
	};

	virtual void StartDungeon()
	{
		m_OnBattleStart.Broadcast();
	};

	virtual void EndDungeon(bool b)
	{
		m_OnBattleEnd.Broadcast(b);
	};

	virtual void Tick(float delta)
	{
		
	}

	virtual void OnLevelLoadComplete(UWorld* world)
	{
		PRINTF("This IsParent MonsterSpawn LevelLoadComp");
	};

	virtual bool IsBattleStarted()
	{
		return false;
	};

	virtual FString GetOpenLevelAssetName()
	{
		return FString();
	}
};


