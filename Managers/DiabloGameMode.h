// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionManagerComponent.h"
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
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UActionManagerComponent* m_ActionManager;
	UPROPERTY(VisibleAnywhere)
	APlayerSpawnPoint* m_PlayerVillageSpawn;
	
	TMap<FName,ADiaDungeon*> m_MapDungeonActors;
	
public:
	virtual void StartPlay() override;

	ADiaDungeon* GetDungeon(FName id);

	void SetDungeonInstanceToMap();
	
	void InitDungeonInstances();
	
	void InitSpawnManager();

	APlayerSpawnPoint* GetSpawnPoint();

	FORCEINLINE UActionManagerComponent* GetActionManager()
	{
		return m_ActionManager;
	}
};
