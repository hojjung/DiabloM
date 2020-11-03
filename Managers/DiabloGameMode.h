// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionManagerComponent.h"
#include "DiabloM.h"
#include "DungeonMiniMap.h"
#include "GameFramework/GameMode.h"
#include "DiabloGameMode.generated.h"

class ADiaDungeon;
class APortal;

DECLARE_MULTICAST_DELEGATE_OneParam(FDelta,float);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDungeonCreateBP,ADiaDungeon*,dg);
UCLASS()
class DIABLOM_API ADiabloGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ADiabloGameMode();
	void InitRewardManager();
	void FindSpawnPoint();
	void InitMinimap();

	static ADiabloGameMode* Get;

	
	UPROPERTY(BlueprintAssignable)
	FDungeonCreateBP m_OnDgCreated;
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UActionManagerComponent* m_ActionManager;
	UPROPERTY(VisibleAnywhere)
	APortal* m_PlayerVillageSpawn;
	//UPROPERTY()
	//UGridFlowMiniMap* m_MiniMap;
	UPROPERTY(EditAnywhere)
	TArray<FDungeonMiniMapOverlayIcon> m_AryOverlayMinimap;

	UPROPERTY()
	TMap<FName,ADiaDungeon*> m_MapDungeonActors;
	
	FDelta m_OnTick;
public:
	virtual void StartPlay() override;

	ADiaDungeon* GetDungeon(FName id);

	void SetDungeonInstanceToMap();
	
	void InitDungeonInstances();
	
	void InitSpawnManager();

	APortal* GetSpawnPoint();

	FORCEINLINE UActionManagerComponent* GetActionManager()
	{
		return m_ActionManager;
	}

	virtual void Tick(float DeltaSeconds) override;

	FORCEINLINE FDelta GetOnDeltaTick()
	{
		return m_OnTick;
	}

	//UFUNCTION(BlueprintCallable)
	//UGridFlowMiniMap* GetMinimapManager()
	//{
	//	return m_MiniMap;
	//}
};
