// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionManagerComponent.h"
#include "DiabloM.h"
#include "DungeonMiniMap.h"
#include "Components/PostProcessComponent.h"
#include "GameFramework/GameMode.h"
#include "Objs/Containers/Quadtree.h"

#include "DiabloGameMode.generated.h"

class UGridFlowMiniMap;
class ADiaDungeon;
class APortal;
class UGridFlowTilemap;


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

	TUniquePtr<Quadtree> m_QuadTree;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector2D m_Min;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector2D m_Max;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int m_nDepth;
	
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

	UPROPERTY(EditAnywhere, Category = "MiniMap-Fog Of War", meta = (EditCondition = m_bMinimapbEnableFogOfWar))
	float m_fMinimapFogOfWarTextureScale;

	UPROPERTY(EditAnywhere, Category = "MiniMap-Fog Of War", meta = (EditCondition = m_bMinimapbEnableFogOfWar))
	FName m_NameMinimapFogOfWarTrackingItem;

	UPROPERTY(EditAnywhere, Category = "MiniMap-Fog Of War", meta = (EditCondition = m_bMinimapbEnableFogOfWar))
	UTexture2D* m_MinimapFogOfWarExploreTexture;

	UPROPERTY(EditAnywhere, Category = "MiniMap-Fog Of War", meta = (EditCondition = m_bMinimapbEnableFogOfWar))
	float m_fMinimapFogOfWarVisiblityDistance;
	
	UPROPERTY()
	TMap<FName,ADiaDungeon*> m_MapDungeonActors;
	
	//FDelta m_OnTick;
	
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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	ADiaDungeon* GetDungeon(FName id);
	
	APortal* GetSpawnPoint();

	FORCEINLINE UActionManagerComponent* GetActionManager()
	{
		return m_ActionManager;
	}


	UFUNCTION(BlueprintCallable)
	UGridFlowMiniMap* GetMinimapManager()
	{
		return m_MiniMap;
	}

	void RegisterQuadElement(ITickHideable* actor);

	//UGridFlowTilemap*

	void SetQuadTreeCoord(ADiaDungeon* dgActor,UGridFlowTilemap* dgTilemap);
};
