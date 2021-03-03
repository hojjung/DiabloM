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
	
protected:
	TArray<const FDungeonDataTableRow*> m_AryDgDataTable;
	
	UPROPERTY()
	UMonsterSpawnManager* m_MonsterManager;
	
	UPROPERTY()
	TArray<int> m_AryDgUnlocked;

	const FDungeonDataTableRow* m_CurrentDg;

	//보스는 10킬이후부터 생성?
	//보스는 1회만 죽여야한다
	//보스는 1회만?
	//그럼 결국 던전이 킬카운트 가지고 있어야한다
public:
	void Init(UMonsterSpawnManager*  mMang);

	void OpenLevel();
	
	void LoadLevelComplete(UWorld* world);
	
	void SetDungeonLevel(const FString& dgUnlockAry);

	FORCEINLINE const TArray<const FDungeonDataTableRow*>& GetAryDgData() const
	{
		return m_AryDgDataTable;
	}

	FORCEINLINE const TArray<int>& GetAryDgUnlocked() const
	{
		return m_AryDgUnlocked;
	}

	void SelectDungeon(int index);
};
