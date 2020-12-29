// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GridFlowAsset.h"
#include "Core/Dungeon.h"
#include "DiaDungeon.generated.h"


USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FDgDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName m_Id;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UDungeonThemeAsset* m_DgTheme;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UGridFlowAsset* m_DgGridFlow;
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
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FDgDataAsset> m_AryDgDatas;
	
	TMap<FName,FDgDataAsset*> m_MapDgDatas; 
	
public:
	void Init();
	
	bool IsMyActor(AActor* actor);

	void ShuffleSpawnPoints(TArray<FTransform>& aryEmit,int iter=1) const;
	
	TArray<FTransform>& GetArySpawnPoints();

	FVector GetStartPoint();

	const FDgDataAsset& GetDgData(FName id);
};
