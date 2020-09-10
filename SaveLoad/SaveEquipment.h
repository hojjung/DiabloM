// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SaveLoadManagerOld.h"
#include "Datas/PlayerInitDataTable.h"
#include "GameFramework/SaveGame.h"
#include "Objs/EquipmentSystemOld.h"

#include "SaveEquipment.generated.h"



UCLASS()
class DIABLOM_API USaveEquipment : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
    ESaveVersion m_SaveVersion;
	
    UPROPERTY(EditAnywhere)
	TArray<FItemInstance> m_EquipAry;
	
    void SetEquipSaveData(const TArray<FEquipSlot*>& equip);//
	
    void SetEquipLoadData(EquipmentSystemOld** outEquip);

	void SetEquipSaveDataFromCreation(const FCurrentCharData& charData);//
};
