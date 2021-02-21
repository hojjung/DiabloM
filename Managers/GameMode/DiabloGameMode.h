// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionManagerComponent.h"
#include "DiabloM.h"
#include "Components/PostProcessComponent.h"
#include "GameFramework/GameMode.h"
#include "Managers/MonsterSpawnManager.h"

#include "DiabloGameMode.generated.h"

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
	
protected:
	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void StartPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FORCEINLINE UActionManagerComponent* GetPlayerActionManager()
	{
		return m_PlayerActionManager;
	}
};


