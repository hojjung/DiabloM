// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Datas/CharacterDataTable.h"
#include "Datas/ItemDataTable.h"
#include "DiabloGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UDiabloGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UDiabloGameInstance(const FObjectInitializer& objInit);

protected:
	UPROPERTY(EditAnywhere)
	UDataTable* m_BaseUnitTable;
	UPROPERTY(EditAnywhere)
	UDataTable* m_PlayerUnitTable;
	UPROPERTY(EditAnywhere)
	UDataTable* m_ItemTable;

public:
	const FBaseStatTable* GetBaseUnit(FName id) const;
	const FItemData* GetItemData(FName id)const;
};
