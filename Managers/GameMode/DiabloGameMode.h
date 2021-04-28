// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionManagerComponent.h"
#include "DiabloM.h"
#include "Characters/OtherPlayerPawn.h"
#include "Components/PostProcessComponent.h"
#include "GameFramework/GameMode.h"
#include "Managers/ChatManager.h"
#include "Managers/GoldManager.h"
#include "Managers/MonsterSpawnManager.h"


#include "DiabloGameMode.generated.h"

class UPlayerUpgradeManager;
class UPlayfabManager;
class APlayerVisual;


DECLARE_MULTICAST_DELEGATE_OneParam(FDelta,float);
UCLASS()
class DIABLOM_API ADiabloGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ADiabloGameMode();
	
	void SpawnVisualActor();

	void SpawnOtherPVPActor();

	FOnTick m_OnTick;
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UActionManagerComponent* m_PlayerActionManager;
	UPROPERTY()
	UPlayfabManager* m_PlManager;
	UPROPERTY()
	UPlayerUpgradeManager* m_PlUpgrade;
	UPROPERTY()
	UChatManager* m_ChatManager;
	UPROPERTY()
	APlayerVisual* m_VisualActor;
	UPROPERTY()//for pvp visual
	AOtherPlayerPawn* m_OtherPlayer;
	UPROPERTY()
	UGoldManager* m_GoldManager;

	bool m_bGoldOfflineLock;

public:
	virtual void StartPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FORCEINLINE UActionManagerComponent* GetPlayerActionManager()
	{
		return m_PlayerActionManager;
	}

	void OnMenuOpen(bool b);

	FORCEINLINE APlayerVisual* GetVisualActor()
	{
		return m_VisualActor;
	}

	virtual void Tick(float DeltaSeconds) override;

};


