// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Objs/Actor/DiaDungeon.h"
#include "UObject/NoExportTypes.h"
#include "DungeonManager.generated.h"


class AMonsterPawn;
struct FDungeonDataRow;

UCLASS()
class DIABLOM_API UDungeonManager : public UObject
{
	GENERATED_BODY()

public:
	UDungeonManager();
	
protected:
	TArray<FDungeonDataRow*> m_AryDungeonData;//던전 데이터는 돌려쓰면 됨
	UPROPERTY()
	TArray<AMonsterPawn*> m_AryMonsterSpawnedCurrently;
	UPROPERTY()
	ADiaDungeon* m_CurrentDungeon;

	int m_nPointIndex;

	FVector m_RecentDungeonFeetLoc;

protected:
	int StageLevelToDungeonLevel(int stageLevel);
	
	int StageLevelToDungeonType(int stageLevel);
	
	void LoadDungeonLevel(FDungeonDataRow* SelectedDungeonData);
	
	void SpawnMonstersToDungeon(int MonsterLevel, FDungeonDataRow* SelectedDungeonData);
	
public:
	void Init();
	
	UFUNCTION(BlueprintCallable)
	void CreateDefaultInfinityDungeon(int level=1);
	UFUNCTION(BlueprintCallable)
	void ShowSpawnedMonster();
	UFUNCTION(BlueprintCallable)
	void HideSpawnedMonster();
	UFUNCTION(BlueprintCallable)
	void PortalToVillage();
	UFUNCTION(BlueprintCallable)
	void PortalToRecentDungeon();
};
