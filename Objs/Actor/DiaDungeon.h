// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Dungeon.h"
#include "DiaDungeon.generated.h"

UCLASS()
class DIABLOM_API ADiaDungeon : public ADungeon
{
	GENERATED_BODY()

public:
	static FString m_EmitNameEnemy;
	static FString m_EmitNameStart;
	
	ADiaDungeon(const FObjectInitializer& ObjectInitializer);
	
public:
	bool IsMyActor(AActor* actor);

	void ShuffleSpawnPoints(TArray<FTransform>& aryEmit,int iter=1) const;
	
	TArray<FTransform>& GetArySpawnPoints();

	FVector GetStartPoint();	
};
