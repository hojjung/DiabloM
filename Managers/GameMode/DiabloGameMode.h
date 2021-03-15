// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionManagerComponent.h"
#include "DiabloM.h"
#include "Components/PostProcessComponent.h"
#include "GameFramework/GameMode.h"
#include "Managers/ChatManager.h"
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
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UActionManagerComponent* m_PlayerActionManager;
	UPROPERTY()
	UPlayfabManager* m_PlManager;
	UPROPERTY()
	UPlayerUpgradeManager* m_PlUpgrade;
	UPROPERTY()
	UChatManager* m_ChatManager;
	
public:
	virtual void StartPlay() override;

	FORCEINLINE UActionManagerComponent* GetPlayerActionManager()
	{
		return m_PlayerActionManager;
	}
};


