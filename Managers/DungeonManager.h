// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Objs/Actor/DiaDungeon.h"
#include "Objs/Containers/Quadtree.h"
#include "UObject/NoExportTypes.h"
#include "Village/DgToVillagePortal.h"

#include "DungeonManager.generated.h"


class AMonsterPawn;
struct FDungeonDataRow;



DECLARE_MULTICAST_DELEGATE_OneParam(FDungeonCreate,bool);

UCLASS()
class DIABLOM_API UDungeonManager : public UObject
{
	GENERATED_BODY()

public:
	UDungeonManager(const FObjectInitializer& objInit);
	
public:
	int m_nCurrentMonsterCount;

	int m_nClearableCount;

protected:
	UPROPERTY()
	TSubclassOf<ADgToVillagePortal> m_ClassDgVillagePortal;
	UPROPERTY()
	ADgToVillagePortal* m_CurrentDgVillagePortal;
	UPROPERTY()
	TArray<AMonsterPawn*> m_AryMonsterSpawnedCurrently;
	UPROPERTY(Transient)
	UMaterialInterface* m_MatMinimap;
	
	TArray<FDungeonDataRow*> m_AryDungeonData;//던전 데이터는 돌려쓰면 됨
	
	TWeakObjectPtr<ADiaDungeon> m_CurrentDungeon;

	FDungeonDataRow* m_CurrentDungeonData;

	FName m_NamePortalID;

	int m_nMonsterLevel;

	int m_nDungeonType;
	
	int m_nPointIndex;

	bool m_bIsPlayerInDungeon;

	FVector m_RecentDungeonFeetLoc;

	FDungeonCreate m_OnPortalCreate;

	
protected:
	int StageLevelToDungeonType(int stageLevel);
	
	void LoadDungeonLevel(FDungeonDataRow* SelectedDungeonData);
	
	void SpawnMonstersToDungeon(int MonsterLevel, FDungeonDataRow* SelectedDungeonData);

	void DungeonComplete();
	
public:
	void Init();
	
	int StageLevelToDungeonLevel(int stageLevel);
	
	void CreateQuadTreeBound();

	UFUNCTION(BlueprintCallable)
	void CreateDefaultInfinityDungeon(int level=1);
	UFUNCTION(BlueprintCallable)
	void PortalToVillage(bool isDgCleared);
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

	void MonsterDead();

	FVector GetCurrentPlayerFeetLoc();
};
