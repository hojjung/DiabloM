// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterSpawnManager.h"

#include "DungeonManager.generated.h"


class ANavigationData;
class AMonsterPawn;


USTRUCT()
struct FDungeonDataSpec
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int m_nCurrentStage;
	UPROPERTY(EditAnywhere)
	int m_nMaxStage;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FDungeonCreate,bool);

UCLASS()
class DIABLOM_API UDungeonManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnDungeonMaxUpdate);
	
	FOnDungeonMaxUpdate m_OnDungeonMaxUpdate;
	
	UDungeonManager(const FObjectInitializer& objInit);

	static UDataTable* DungeonDataTable;

	static UDataTable* DropDataTable;

	static UDataTable* MonsterEntityTable;

	static UDataTable* GoldDungeonDataTable;

	//FItemDropTableRow
	//FMonsterEntity
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDgOpen,int);

	FOnDgOpen m_OnDgOpen;
protected:
	TArray<const FDungeonDataTableRow*> m_AryDgDataTable;
	
	UPROPERTY()
	UMonsterSpawnManager* m_MonsterManager;
	
	const FDungeonDataTableRow* m_CurrentDg;

	

	UPROPERTY()
	int m_nMyMaxStageLevel;
	UPROPERTY()
	int m_nCurrentStageLevel;
	UPROPERTY()
	int m_nSafeMaxStageLevel;
	UPROPERTY()
	int m_nSafeCurrentStageLevel;
	//보스는 10킬이후부터 생성?
	//보스는 1회만 죽여야한다
	//보스는 1회만?
	//그럼 결국 던전이 킬카운트 가지고 있어야한다

protected://GoldDg
	TArray<const FDungeonDataTableRow*> m_AryGoldDgDataTable;

	const FDungeonDataTableRow* m_CurrentGoldDg;
public:
	void Init(UMonsterSpawnManager*  mMang);

	void SetDungeonData(const FString& dgJsonStr);

	void OpenLevel();
	
	void LoadLevelComplete(UWorld* world);
	
	FORCEINLINE const TArray<const FDungeonDataTableRow*>& GetAryDgData() const
	{
		return m_AryDgDataTable;
	}

	void SelectNormalDungeon(int index);

	void SelectGoldDungeon(int index);

	void LevelUpDungeon();//Call By Boss

	BigInt GetCurrentDungeonBounty();

	BigInt GetMaxDungeonBounty();

	int GetMaxStage() const;

	int GetCurrentStage()const;

	void SetMaxStageLevel(int stageLv);

	void SetCurrentStageLevel(int stageLv);

	int GetLevelBonus();

	FString GetDgDataStr();
};
