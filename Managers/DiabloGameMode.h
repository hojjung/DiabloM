// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "GameFramework/GameMode.h"
#include "Objs/Actor/PlayerSpawnPoint.h"

#include "DiabloGameMode.generated.h"

class ADiaDungeon;


UCLASS()
class DIABLOM_API ADiabloGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ADiabloGameMode();
	
	static ADiabloGameMode* Get;

protected:
	TMap<FName,ADiaDungeon*> m_MapDungeonActors;

	UPROPERTY(VisibleAnywhere)
	APlayerSpawnPoint* m_PlayerVillageSpawn;
public:

	virtual void StartPlay() override;

	ADiaDungeon* GetDungeon(FName id);

	void SetDungeonInstanceToMap();
	void InitDungeonInstances();
	void InitSpawnManager();

	APlayerSpawnPoint* GetSpawnPoint();
};
