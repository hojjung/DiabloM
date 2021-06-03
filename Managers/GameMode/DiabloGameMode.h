// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Characters/Pawns/OtherPlayerPawn.h"
#include "Components/PostProcessComponent.h"
#include "GameFramework/GameMode.h"
#include "Managers/GoldManager.h"
#include "Managers/DgContentManagers/DungeonManager.h"
#include "Managers/SystemManagers/ChatManager.h"

#include "DiabloGameMode.generated.h"

class UPlayerUpgradeManager;
class UPlayfabManager;
class APlayerVisual;


UCLASS()
class DIABLOM_API ADiabloGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ADiabloGameMode();
	
protected:
	UPROPERTY()
	UDungeonManager* m_DungeonManager;

public:
	virtual void StartPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;
};


