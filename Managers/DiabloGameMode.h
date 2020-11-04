// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionManagerComponent.h"
#include "DiabloM.h"
#include "DungeonMiniMap.h"
#include "GameFramework/GameMode.h"
#include "DiabloGameMode.generated.h"

class UGridFlowMiniMap;
class ADiaDungeon;
class APortal;

DECLARE_MULTICAST_DELEGATE_OneParam(FDelta,float);
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

	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UActionManagerComponent* m_ActionManager;
	UPROPERTY(VisibleAnywhere)
	APortal* m_PlayerVillageSpawn;
	UPROPERTY()
	UGridFlowMiniMap* m_MiniMap;
	UPROPERTY(EditAnywhere, Category = "MiniMap")
	TArray<FDungeonMiniMapOverlayIcon> m_AryOverlayMinimap;
	UPROPERTY(EditAnywhere, Category = "MiniMap")
	int32 m_MinimapTextureSize;

	UPROPERTY(EditAnywhere, Category = "MiniMap")
	float m_MinimapOutlineThickness;

	UPROPERTY(EditAnywhere, Category = "MiniMap")
	float m_MinimapDoorThickness;

	UPROPERTY(EditAnywhere, Category = "MiniMap")
	UMaterialInterface* m_MinimapMaterialTemplate;

	UPROPERTY(EditAnywhere, Category = "MiniMap")
	float m_MinimapBlurRadius;

	UPROPERTY(EditAnywhere, Category = "MiniMap")
	int32 m_MinimapBlurIterations;

	UPROPERTY(EditAnywhere, Category = "MiniMap-Fog Of War")
	bool m_MinimapbEnableFogOfWar;

	UPROPERTY(EditAnywhere, Category = "MiniMap-Fog Of War", meta = (EditCondition = bEnableFogOfWar))
	float m_MinimapFogOfWarTextureScale;

	UPROPERTY(EditAnywhere, Category = "MiniMap-Fog Of War", meta = (EditCondition = bEnableFogOfWar))
	FName m_MinimapFogOfWarTrackingItem;

	UPROPERTY(EditAnywhere, Category = "MiniMap-Fog Of War", meta = (EditCondition = bEnableFogOfWar))
	UTexture2D* m_MinimapFogOfWarExploreTexture;

	UPROPERTY(EditAnywhere, Category = "MiniMap-Fog Of War", meta = (EditCondition = bEnableFogOfWar))
	float m_MinimapFogOfWarVisiblityDistance;
	
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
