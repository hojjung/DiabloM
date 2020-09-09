// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "SaveCharacterStatus.h"


class PlayerCreateManager;
class USaveEquipment;
class USaveInventory;

class DIABLOM_API SaveLoadManager
{
public:
	static SaveLoadManager* Get;
	
	~SaveLoadManager();

	int m_nCurrentPlayerIndex=0;

	ESaveVersion m_SaveVersion = ESaveVersion::Init;


	//슬롯이존재해야함
	int m_nMaxSlotCount;

	int m_nCurrentSlotCount;

	FOnSaveDataCreated m_OnDataCreated; 
protected:
	//index Need Insul
	TArray<USaveCharacterStatus*> m_AryLoadedCharacters;
	TArray<USaveEquipment*> m_AryLoadedEquipments;
	TArray<USaveInventory*> m_AryLoadedInventorys;
	//They DonNeedInst
public:
	void InitSaveLoadManager();
	
protected:
	void TryLoadAllCharacter();
	

public:	
	void DeleteSlot(int i);
	
	void DeleteAllSlot();
	
	void SaveInventory() const;

	void LoadInventory() const;
	
	void SaveEquipment() const;

	void LoadEquipment() const;

	void SaveCharacterStat() const;
	
	bool LoadCharacterStat(int index);

	FORCEINLINE const TArray<USaveCharacterStatus*>& GetCharStatAry() const
	{
		return  m_AryLoadedCharacters;
	}

	void CreateNewCharacter(PlayerCreateManager* plManager);
	
	FORCEINLINE const TArray<USaveCharacterStatus*>& GetLoadedChars() const
	{
		return m_AryLoadedCharacters;
	}
	FORCEINLINE const TArray<USaveEquipment*>& GetLoadedEquip() const
	{
		return m_AryLoadedEquipments;
	}
	FORCEINLINE const TArray<USaveInventory*>& GetLoadedInven() const
	{
		return m_AryLoadedInventorys;
	}
	int GetEmptyIndex();
	//Focus Character need
	void LoadItemDataForInstance(TArray<FItemInstance>& itemAry);
};
