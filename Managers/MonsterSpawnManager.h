// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "NavigationPath.h"
#include "NavigationData.h"
#include "NavigationSystem.h"
#include "Characters/MonsterPawn.h"
#include "Datas/SpawnDataTable.h"


#include "MonsterSpawnManager.generated.h"

UCLASS()
class DIABLOM_API UMonsterSpawnManager : public UObject
{
	GENERATED_BODY()


public:
	UMonsterSpawnManager();
	
protected:
	float m_fSpawnRadius;
	UPROPERTY()
	UNavigationSystemV1* m_NavSys;
	UPROPERTY()
	UWorld* m_CurrentWorld;

	FName m_IdEnemy;

	FName m_IdBossEnemy;

	FName m_IdSpecialEnemy;

	UPROPERTY()
	TArray<AMonsterPawn*> m_AryMonsterSpawnedCurrently;
	
protected:
	FVector GetRandomPoint(const FVector& loc,const float& radius);
	
	AMonsterPawn* SpawnMob(FVector loc);
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateWorld(UWorld* world);
	UFUNCTION(BlueprintCallable)
	bool SpawnIter(const FVector& centerSpawnLoc,const FMonsterHordeRow& selectedHorde,int level=1,UDungeonManager* dgSpawnedManager=nullptr);
	//

	void Reset();

	FORCEINLINE TArray<AMonsterPawn*>& GetCurrentMonsters()
	{
		return m_AryMonsterSpawnedCurrently;
	}

	AMonsterPawn* GetNearestMonster(const FVector& wantPos,bool bSeeHideObj,AMonsterPawn* ignoreActor =nullptr);
};

