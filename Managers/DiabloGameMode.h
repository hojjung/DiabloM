// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "GameFramework/GameMode.h"
#include "DiabloGameMode.generated.h"

class ADiaDungeon;


UCLASS()
class DIABLOM_API ADiabloGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ADiabloGameMode();
	void SetDungeonInstanceToMap();
	void InitDungeonInstances();
	void InitSpawnManager();

	static ADiabloGameMode* Get;

protected:
	TMap<FName,ADiaDungeon*> m_MapDungeonActors;	
public:

	virtual void StartPlay() override;

	ADiaDungeon* GetDungeon(FName id);
};
