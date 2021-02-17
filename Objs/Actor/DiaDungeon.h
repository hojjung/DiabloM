// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"

#include "GridFlowAsset.h"
#include "Core/Dungeon.h"
#include "DiaDungeon.generated.h"


USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FDgDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UDungeonThemeAsset* m_DgTheme;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftObjectPtr<UGridFlowAsset> m_DgGridFlow;
};

UCLASS()
class DIABLOM_API ADiaDungeon : public ADungeon
{
	GENERATED_BODY()

public:
	static FString m_EmitNameEnemy;
	
	static FString m_EmitNameStart;
	
public:
	ADiaDungeon(const FObjectInitializer& ObjectInitializer);
	
public:
	bool IsMyActor(AActor* actor);

	void ShuffleSpawnPoints(TArray<FTransform>& aryEmit,int iter=1) const;
	
	TArray<FTransform>& GetArySpawnPoints();

	FVector GetStartPoint();

};
