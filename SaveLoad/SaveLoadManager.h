// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"

#include "Characters/DiabloPlayerController.h"
#include "Datas/PlayerInitDataTable.h"
#include "Item/EquipmentSystem.h"
#include "SaveLoadManager.generated.h"

class USaveStorage;
class UPlayerCreateManager;
class USaveEquipment;
class USaveInventory;
class USaveCharacterStatus;
class USaveShareStorage;

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
    const FString m_InvenSlotName;

    const FString m_EquipSlotName;

    const FString m_CharSlotName;

    const FString m_StorageSlotName;

    const FString m_StorageShareSlotName;
    
    static USaveLoadManager* Get;

    USaveLoadManager();
    
    ~USaveLoadManager();

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
    TArray<USaveInventory*> m_AryLoadedInventory;
    UPROPERTY()
    TArray<USaveStorage*> m_AryLoadedStorage;
    UPROPERTY()
    USaveShareStorage* m_LoadShareStorage;
    //They DonNeedInst
protected:
    void TryLoadAllCharacter();
    
public:
    void DeleteSlot(int i);

    void DeleteAllSlot();

    void DeleteShareStorage();
    //
    void SaveInventory(int slotIndex, const TArray<FItemInstance>& aryItem);
    void SaveEquipment(int slotIndex,const TArray<FItemInstance>& aryItem);
    void SetEquipSaveDataFromCreation(const FCurrentCharData& charData,TArray<FItemInstance>& newEquipAry);
    void SaveCharacterStat(int slotIndex, int level, FText nameText, int faceIndex, int hairIndex,FName classID,float exp=0.f,float gold=0.f);
    void SaveStorage(int slotIndex,const TArray<bool>& aryOpen,const TArray<TArray<FItemInstance>>& aryItems);
    void SaveShareStorage(const TArray<bool>& aryOpen,const TArray<TArray<FItemInstance>>& aryItems);
    //
    void LoadInventory(int slotIndex);
    void LoadEquipment(int slotIndex);
    void LoadCharStat(int index);
    void LoadStorage(int slotIndex);
    void LoadShareStorage();

    bool DoesSaveDataExist(int slotIndex);
    //
private:
    void CreateSetEquipSlotItem(TArray<FItemInstance>& arrayUsing,FName itemId, ESlotsEquipAry slot);
    
public:
    int CreateNewCharacter(UPlayerCreateManager* plManager);
    
    void SetLoadedEquipDataToPlayer(int slotIndex);
    
    void SetLoadedCharDataToPlayer(int slotIndex);
    
    void SetLoadedInvenDataToPlayer(int slotIndex);

    void SetLoadedStorageDataToPlayer(int slot_index);
    
    void CreateSetPlayerCharacter();
    
    const TArray<USaveCharacterStatus*>& GetLoadedChars() const;
    
    const TArray<USaveEquipment*>& GetLoadedEquip() const;
    
    const TArray<USaveInventory*>& GetLoadedInven() const;

    const TArray<USaveStorage*>& GetLoadedStorage() const;
    
    int GetEmptyIndex();
    //Focus Character need
    void LoadItemDataForInstance(TArray<FItemInstance>& itemAry,ESaveVersion version);

    FName GetCurrentPlayerClassName();

};
