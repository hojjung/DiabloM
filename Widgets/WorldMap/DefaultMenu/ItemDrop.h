// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Datas/ItemDataTable.h"

#include "ItemDrop.generated.h"

UCLASS()
class DIABLOM_API UItemDrop : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;
	void ThrowItem(const FItemInstance& itemThrow);
};
