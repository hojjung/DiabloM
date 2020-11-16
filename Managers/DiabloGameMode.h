// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionManagerComponent.h"
#include "DiabloM.h"
#include "DungeonMiniMap.h"
#include "Components/PostProcessComponent.h"
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
	static ADiabloGameMode* Get;
	
	ADiabloGameMode();
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UPostProcessComponent* m_PostProcess;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UActionManagerComponent* m_ActionManager;
	
	UPROPERTY(VisibleAnywhere)
	APortal* m_PlayerVillageSpawn;
	
	UPROPERTY()
	UGridFlowMiniMap* m_MiniMap;
	
protected:
	UPROPERTY(EditAnywhere, Category = "MiniMap")
	TArray<FDungeonMiniMapOverlayIcon> m_AryOverlayMinimap;
	
	UPROPERTY(EditAnywhere, Category = "MiniMap")
	int32 m_fMinimapTextureSize;

	UPROPERTY(EditAnywhere, Category = "MiniMap")
	float m_fMinimapOutlineThickness;

	UPROPERTY(EditAnywhere, Category = "MiniMap")
	float m_fMinimapDoorThickness;

	UPROPERTY(EditAnywhere, Category = "MiniMap")
	UMaterialInterface* m_MinimapMaterialTemplate;

	UPROPERTY(EditAnywhere, Category = "MiniMap")
	float m_fMinimapBlurRadius;

	UPROPERTY(EditAnywhere, Category = "MiniMap")
	int32 m_nMinimapBlurIterations;

	UPROPERTY(EditAnywhere, Category = "MiniMap-Fog Of War")
	bool m_bMinimapbEnableFogOfWar;

	UPROPERTY(EditAnywhere, Category = "MiniMap-Fog Of War", meta = (EditCondition = bEnableFogOfWar))
	float m_fMinimapFogOfWarTextureScale;

	UPROPERTY(EditAnywhere, Category = "MiniMap-Fog Of War", meta = (EditCondition = bEnableFogOfWar))
	FName m_NameMinimapFogOfWarTrackingItem;

	UPROPERTY(EditAnywhere, Category = "MiniMap-Fog Of War", meta = (EditCondition = bEnableFogOfWar))
	UTexture2D* m_MinimapFogOfWarExploreTexture;

	UPROPERTY(EditAnywhere, Category = "MiniMap-Fog Of War", meta = (EditCondition = bEnableFogOfWar))
	float m_fMinimapFogOfWarVisiblityDistance;
	
	UPROPERTY()
	TMap<FName,ADiaDungeon*> m_MapDungeonActors;
	
	FDelta m_OnTick;
	
protected:
	void InitRewardManager();
	
	void InitMinimap();
	
	void InitDungeonInstances();
	
	void InitSpawnManager();
	
	void FindSpawnPoint();
	
	void SetDungeonInstanceToMap();
	
	virtual void Tick(float DeltaSeconds) override;
	
public:
	virtual void StartPlay() override;

	ADiaDungeon* GetDungeon(FName id);
	
	APortal* GetSpawnPoint();

	FORCEINLINE UActionManagerComponent* GetActionManager()
	{
		return m_ActionManager;
	}

	FORCEINLINE FDelta GetOnDeltaTick()
	{
		return m_OnTick;
	}

	UFUNCTION(BlueprintCallable)
	UGridFlowMiniMap* GetMinimapManager()
	{
		return m_MiniMap;
	}
};
