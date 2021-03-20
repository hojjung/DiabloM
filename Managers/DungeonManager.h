// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



#include "GameplayTagContainer.h"
#include "MonsterSpawnManager.h"
#include "NavigationSystem.h"
#include "PlayfabManager.h"
#include "Datas/DungeonDataTable.h"
#include "UObject/NoExportTypes.h"

#include "DungeonManager.generated.h"


class ANavigationData;
class AMonsterPawn;
struct FDungeonDataRow;



DECLARE_MULTICAST_DELEGATE_OneParam(FDungeonCreate,bool);

UCLASS()
class DIABLOM_API UDungeonManager : public UObject
{
	GENERATED_BODY()

public:
	UDungeonManager(const FObjectInitializer& objInit);

	static UDataTable* DungeonDataTable;

	static UDataTable* DropDataTable;

	static UDataTable* MonsterEntityTable;

	//FItemDropTableRow
	//FMonsterEntity
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDgOpen,int);

	FOnDgOpen m_OnDgOpen;
protected:
	TArray<const FDungeonDataTableRow*> m_AryDgDataTable;
	
	UPROPERTY()
	UMonsterSpawnManager* m_MonsterManager;
	
	const FDungeonDataTableRow* m_CurrentDg;

	int m_nMyMaxStageLevel;

	int m_nCurrentStageLevel;
	//보스는 10킬이후부터 생성?
	//보스는 1회만 죽여야한다
	//보스는 1회만?
	//그럼 결국 던전이 킬카운트 가지고 있어야한다
public:
	void Init(UMonsterSpawnManager*  mMang);

	void OpenLevel();
	
	void LoadLevelComplete(UWorld* world);
	
	void SetDungeonLevel(const FString& currentDG);

	FORCEINLINE const TArray<const FDungeonDataTableRow*>& GetAryDgData() const
	{
		return m_AryDgDataTable;
	}

	FORCEINLINE int GetMyMaxStageLevel() const
	{
		return m_nMyMaxStageLevel;
	}

	FORCEINLINE int GetCurrentStageLevel() const
	{
		return m_nCurrentStageLevel;
	}

	void SelectDungeon(int index);

	void LevelUpDungeon();//Call By Boss
};
