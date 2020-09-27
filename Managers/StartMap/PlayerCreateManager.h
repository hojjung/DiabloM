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
    void SetFaceFromSetting();
    void InitArraysFromTable();
    void SetPerkFromSetting();
    void SetItemFromSetting();
    void SetHairFromSetting();
    void SetArmorFromSetting();

    static UPlayerCreateManager* Get;
	
protected:
    FCurrentCharData m_CurrentCharData;

public:
    FText m_CurrentTextName;
    TArray<FPlayerHairRow*> m_AryHair;
    TArray<FPlayerFaceRow*> m_AryFace;
    TArray<FPlayerArmorRow*> m_AryArmor;
    TArray<FPlayerItemRow*> m_AryItem;
    TArray<FPlayerPerkRow*> m_AryPerk;
    //
    int m_IndexHair;
    int m_IndexFace;
    int m_IndexArmor;
    int m_IndexWeapon;
    int m_IndexItem;
    int m_IndexPerk;

    int m_CurrentSelectSlot;

    //
public:
    FOnVisualChange m_OnVisualChange;
    FOnStartCreation m_OnStartCreation;
    
public:
    void Init(UDiabloGameInstance* gameInst);

    void ClearIndex();

    void OnDataChanged(); //바뀔때마다 비쥬얼을 업데이트함

    FORCEINLINE const FCurrentCharData& GetCurrentCharData()
    {
        return m_CurrentCharData;
    }

    void SetCurrentDataFromSaveFile(const USaveCharacterStatus* char_stat,const USaveEquipment* save_equipment);
public:
    void DecreaseHair();
    void IncreaseHair();
    void DecreaseFace();
    void IncreaseFace();
    void DecreaseArmor();//이게 직업선택이 되야함
    void IncreaseArmor();
    void DecreaseItem();
    void IncreaseItem();
    void DecreasePerk();
    void IncreasePerk();
    
    int DoneCreateCharcter();
    
    USkeletalMesh* GetFace(int index);
    USkeletalMesh* GetHair(int index,bool hasHelMet);

    const FCurrentCharData& GetCurrentCharData() const;
};
