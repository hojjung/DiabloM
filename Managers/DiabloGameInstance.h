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
	UDiabloGameInstance(const FObjectInitializer& objInit);

	static UDiabloGameInstance* Get;
protected:
	ItemManager* m_ItemManager;
	SaveLoadManager* m_SaveLoadManager;
protected:
	UPROPERTY(EditAnywhere)
	UDataTable* m_BaseUnitTable;
	UPROPERTY(EditAnywhere)
	UDataTable* m_PlayerUnitTable;
	UPROPERTY(EditAnywhere)
	UDataTable* m_ItemTable;

protected:
	virtual void Init() override;

	virtual void Shutdown()override ;

public:
	const FBaseStatTable* GetBaseUnit(FName id) const;
	const FItemData* GetItemData(FName id)const;
	FItemInstance CreateItem(FName id);
};
