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
	
protected:
	UPROPERTY()
	UMonsterSpawnManager* m_MonsterManager;
	
	const FDungeonDataTableRow* m_CurrentDg;
	UPROPERTY()
	TArray<int> m_AryDgUnlocked;

public:
	void Init(UMonsterSpawnManager*  mMang);

	void OpenLevel();
	
	void LoadLevelComplete(UWorld* world);
	
	void SetDungeonLevel(const FString& dgUnlockAry);
};
