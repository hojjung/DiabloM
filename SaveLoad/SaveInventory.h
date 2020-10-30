// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "SaveLoadManager.h"
#include "GameFramework/SaveGame.h"
#include "Item/Inventory.h"
#include "SaveInventory.generated.h"

UCLASS()
class DIABLOM_API USaveInventory : public USaveGame
{
	GENERATED_BODY()

public:
	USaveInventory();

public:
	UPROPERTY(EditAnywhere)
	ESaveVersion m_SaveVersion;
	//어떤 아이템이, 어떤 인벤칸에 있는지, 그리고 옵션의 종류와 수치,등급
	UPROPERTY(EditAnywhere)
	int m_nCount;
	UPROPERTY(EditAnywhere)
	TArray<FItemInstance> m_InvenAry;
public:
	void SetSaveData(const TArray<FItemInstance>& inven);//
	
};
