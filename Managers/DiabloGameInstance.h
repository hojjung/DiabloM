// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Datas/CharacterDataTable.h"
#include "Datas/ItemDataTable.h"
#include "Managers/ItemManager.h"
#include  "SaveLoad/SaveLoadManager.h"
#include "DiabloGameInstance.generated.h"

struct FItemInstance;
struct FItemTier;
struct FItemTier;

UCLASS()
class DIABLOM_API UDiabloGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UDiabloGameInstance();//

	static UDiabloGameInstance* Get;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ADroppedItem> m_DropItemClass; 
public:
	ItemManager* m_ItemManager;
	SaveLoadManager* m_SaveLoadManager;
protected:
	UPROPERTY(EditAnywhere)
	UDataTable* m_MonsterUnitTable;
	UPROPERTY(EditAnywhere)
	UDataTable* m_NPCUnitTable;
	UPROPERTY(EditAnywhere)
	UDataTable* m_PlayerUnitTable;
	UPROPERTY(EditAnywhere)
	UDataTable* m_ItemTable;
	
protected:
	virtual void Init() override;
	virtual void OnStart() override;

	virtual void Shutdown()override ;

public:
	const FEntityTable* GetMonsterUnit(FName id) const;
	const FEntityTable* GetNPCUnit(FName id) const;
	const FPlayerEntityTable* GetPlayerUnit(FName id) const;
	const FItemData* GetItemData(FName id)const;
	FItemInstance CreateItem(FName id);
};
