// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Dungeon.h"
#include "DiaDungeon.generated.h"

class ADgMobSpawnPoint;

UCLASS()
class DIABLOM_API ADiaDungeon : public ADungeon
{
	GENERATED_BODY()

public:
	ADiaDungeon(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Dungeon)
	FName m_LevelName;
protected:
	UPROPERTY()
	TArray<ADgMobSpawnPoint*> m_ArySpawnPoints;
	UPROPERTY()
	TArray<AActor*> m_AryMyActors;
public:
	bool IsMyActor(AActor* actor);
	
	void AddSpawnPoints(ADgMobSpawnPoint* spawnPoint);

	void AddMyActors(AActor* actor);


	void ShuffleSpawnPoints(int iter=1);
	

	const TArray<ADgMobSpawnPoint*>& GetArySpawnPoints() const
	{
		return m_ArySpawnPoints;
	}
};
