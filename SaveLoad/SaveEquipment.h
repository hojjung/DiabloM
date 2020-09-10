#pragma once

#include "DiabloM.h"
#include "SaveLoadManager.h"
#include "Datas/PlayerInitDataTable.h"
#include "GameFramework/SaveGame.h"
#include "Item/EquipmentSystem.h"
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
	
    void SetEquipLoadData(UEquipmentSystem** outEquip);

	void SetEquipSaveDataFromCreation(const FCurrentCharData& charData);//
};
