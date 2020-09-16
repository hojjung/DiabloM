// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "GameFramework/CheatManager.h"
#include "DiabloCheatManager.generated.h"

/**
 * 
 */
class APlayerDiabloCharacter;
class ADiabloPlayerController;
class UDiabloGameInstance;

UCLASS()
class DIABLOM_API UDiabloCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	ADiabloPlayerController* m_PlayerController;
	UPROPERTY()
	APlayerDiabloCharacter* m_Player;
	UPROPERTY()
	UDiabloGameInstance* m_GameManager;
protected:
	virtual void InitCheatManager() override;
public:
	UFUNCTION(exec)
	void PrintPlayerStats();
	UFUNCTION(exec)
	void PrintInventoryOld();
	UFUNCTION(exec)
	void PrintEquipment();

	UFUNCTION(exec)
	void SaveInven();
	UFUNCTION(exec)
    void LoadInven();
	UFUNCTION(exec)
    void SaveEquip();
	UFUNCTION(exec)
    void LoadEquip();
	UFUNCTION(exec)
    void SaveCharStat();
	UFUNCTION(exec)
    void LoadCharStat();
	UFUNCTION(exec)
	void DeleteAllSlot();
	UFUNCTION(exec)
	void RemoveAllEffect();
};
