// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Blueprint/DragDropOperation.h"
#include "Datas/ItemDataTable.h"
#include "Widgets/WorldMap/DefaultMenu/DiaInvenGridSlot.h"
#include "DiaDragDrop.generated.h"


UCLASS()
class DIABLOM_API UDiaDragDrop : public UDragDropOperation
{
	GENERATED_BODY()

public:
	static UDiaDragDrop* GetDDOInst;
	
	UPROPERTY()
	UImage* m_PreSlot;

public:
	int m_nPreIndex;

	FItemInstance m_DraggedItem;

public:
	void SetDDO(const FItemInstance & itemInst,UImage * preSlot);

public:
	UFUNCTION()
	void OnDragCancel(UDragDropOperation* meSelf);
	UFUNCTION()
	void OnDrop(UDragDropOperation* meSelf);
};
