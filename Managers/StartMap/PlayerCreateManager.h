// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Datas/PlayerInitDataTable.h"
#include "SaveLoad/SaveCharacterStatus.h"
#include "SaveLoad/SaveEquipment.h"

/**
 * 월드맵에서 쓰일 캐릭터를 미리
 * 렌더링 하고 데이터를 취합해줌
 *
 * 위젯 세팅이 중요한데 결국 플레이어 컨트롤러가 시작하는것임
 * 이 클래스를 플레이어 컨트롤러가 사용하게 끔 학것
 *
 * 위젯은 뷰, 얘가 바로 모델이다.
 *
 * 로드하면? 위젯생각말자
 *
 * 데이터 구조체 전부를 컨테이너로 담아서 다가저야한다.
 */
DECLARE_DELEGATE(FOnStartCreation);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnVisualChange, const FCurrentCharData&);

class UDiabloGameInstance;

class DIABLOM_API PlayerCreateManager //바뀐다는건 얘밖에 모름
{
public:
    PlayerCreateManager();
    ~PlayerCreateManager();
    void SetFaceFromSetting();
    void InitArraysFromTable();
    void SetPerkFromSetting();
    void SetItemFromSetting();
    void SetHairFromSetting();
    void SetArmorFromSetting();

    static PlayerCreateManager* Get;
	
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
    void DecreaseArmor();
    void IncreaseArmor();
    void DecreaseItem();
    void IncreaseItem();
    void DecreasePerk();
    void IncreasePerk();
    
    void DoneCreateCharcter();
    
    USkeletalMesh* GetFace(int index);
    USkeletalMesh* GetHair(int index,bool hasHelMet);

    const FCurrentCharData& GetCurrentCharData() const;
};
