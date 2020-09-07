// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "SaveCharacterStatus.h"



class PlayerCreateManager;

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
	TArray<TSharedPtr<USaveCharacterStatus>> m_AryLoadedCharacters;

	TQueue<int> m_ArySlotIndexNotUsed;
	
public:
	void InitSaveLoadManager();
protected:
	void TryLoadAllCharacter();

public:	
	void SaveInventory() const;

	void LoadInventory() const;
	
	void SaveEquipment() const;

	void LoadEquipment() const;

	void SaveCharacterStat() const;
	
	bool LoadCharacterStat(int index);

	FORCEINLINE TArray<TSharedPtr<USaveCharacterStatus>>& GetCharStatAry()
	{
		return  m_AryLoadedCharacters;
	}

	int GetEmptyIndex();

	void CreateNewCharacter(PlayerCreateManager* plManager);

	//Focus Character need
};
