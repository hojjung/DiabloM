// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Objs/Actor/DiaDungeon.h"
#include "Objs/Containers/Quadtree.h"
#include "UObject/NoExportTypes.h"
#include "DungeonManager.generated.h"


class AMonsterPawn;
struct FDungeonDataRow;



DECLARE_MULTICAST_DELEGATE_OneParam(FDungeonCreate,bool);

UCLASS()
class DIABLOM_API UDungeonManager : public UObject
{
	GENERATED_BODY()

protected:
	TArray<FDungeonDataRow*> m_AryDungeonData;//던전 데이터는 돌려쓰면 됨
	UPROPERTY()
	TArray<AMonsterPawn*> m_AryMonsterSpawnedCurrently;
	
	TWeakObjectPtr<ADiaDungeon> m_CurrentDungeon;

	FDungeonDataRow* m_CurrentDungeonData;

	int m_nMonsterLevel;

	int m_nDungeonType;
	
	int m_nPointIndex;

	bool m_bIsPlayerInDungeon;

	FVector m_RecentDungeonFeetLoc;

	FDungeonCreate m_OnPortalCreate;
	UPROPERTY(Transient)
	UMaterialInterface* m_MatMinimap;
	
	
protected:
	int StageLevelToDungeonLevel(int stageLevel);
	
	int StageLevelToDungeonType(int stageLevel);
	
	void LoadDungeonLevel(FDungeonDataRow* SelectedDungeonData);
	
	void SpawnMonstersToDungeon(int MonsterLevel, FDungeonDataRow* SelectedDungeonData);
	
	
	
public:
	void Init();
	void CreateQuadTreeBound();

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
	UFUNCTION(BlueprintCallable)
    void RestartDungeon();
	UFUNCTION(BlueprintCallable)
	void ClearDungeon();
	
	bool IsDungeonOpened();

	FORCEINLINE FDungeonCreate& GetOnDungeonCreate()
	{
		return m_OnPortalCreate;
	}

	bool IsPlayerInDg();
};
