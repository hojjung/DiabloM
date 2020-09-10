// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Item/EquipmentSystem.h"
#include "SaveLoadManager.generated.h"


class UPlayerCreateManager;
class USaveEquipment;
class USaveInventory;
class USaveCharacterStatus;


DECLARE_DELEGATE_OneParam(FOnSaveDataCreated, const USaveCharacterStatus*);

UENUM(BlueprintType)
enum class ESaveVersion :uint8 //������ �ν��Ͻ��� ��ġ
{
    Init
};

UCLASS()
class DIABLOM_API USaveLoadManager : public UObject
{
    GENERATED_BODY()


public:
    static USaveLoadManager* Get;

    ~USaveLoadManager();

    int m_nCurrentPlayerIndex = 0;

    ESaveVersion m_SaveVersion = ESaveVersion::Init;


    //슬롯이존재해야함
    int m_nMaxSlotCount;

    int m_nCurrentSlotCount;

    FOnSaveDataCreated m_OnDataCreated;

protected:
    UPROPERTY()
    TArray<USaveCharacterStatus*> m_AryLoadedCharacters;
    UPROPERTY()
    TArray<USaveEquipment*> m_AryLoadedEquipments;
    UPROPERTY()
    TArray<USaveInventory*> m_AryLoadedInventoryOlds;
    //They DonNeedInst
public:
    void InitSaveLoadManager();

protected:
    void TryLoadAllCharacter();


public:
    void DeleteSlot(int i);

    void DeleteAllSlot();

    void SaveInventory() const;

    void LoadInventoryOld() const;

    void SaveEquipment() const;

    void LoadEquipment() const;

    void SaveCharacterStat() const;

    bool LoadCharacterStat(int index);


    void CreateNewCharacter(UPlayerCreateManager* plManager);

    const TArray<USaveCharacterStatus*>& GetLoadedChars() const;
    const TArray<USaveEquipment*>& GetLoadedEquip() const;
    const TArray<USaveInventory*>& GetLoadedInven() const;
    int GetEmptyIndex();
    //Focus Character need
    void LoadItemDataForInstance(TArray<FItemInstance>& itemAry);
};
