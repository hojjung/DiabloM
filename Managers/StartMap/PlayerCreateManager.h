// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Datas/PlayerInitDataTable.h"
#include "SaveLoad/SaveCharacterStatus.h"
#include "SaveLoad/SaveEquipment.h"

#include "PlayerCreateManager.generated.h"

DECLARE_DELEGATE(FOnStartCreation);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnVisualChange, const FCurrentCharData&);

class UDiabloGameInstance;

UCLASS()
class DIABLOM_API UPlayerCreateManager : public UObject
{
	GENERATED_BODY()
	
public:
    ~UPlayerCreateManager();
    void InitArraysFromTable();
    void SetItemFromSetting();

    static UPlayerCreateManager* Get;
	
protected:
    FCurrentCharData m_CurrentCharData;

public:
    FText m_CurrentTextName;
    //
    int m_IndexItem;
    
    int m_CurrentSelectSlot;

    TArray<FPlayerInitItemTableRow*> m_AryInitItem;

    //
public:
    FOnVisualChange m_OnVisualChange;
    
    FOnStartCreation m_OnStartCreation;
    
public:
    void Init(UDiabloGameInstance* gameInst);

    void ClearIndex();

    void OnDataChanged(); //바뀔때마다 비쥬얼을 업데이트함

    FORCEINLINE FCurrentCharData& GetCurrentCharData()
    {
        return m_CurrentCharData;
    }

    void SetCurrentDataFromSaveFile();
    
public:
    void DecreaseItem();
    
    void IncreaseItem();
    
    int DoneCreateCharcter();
    
    const FCurrentCharData& GetCurrentCharData() const;

    void SetCurrentDataWithPlayer();
};
