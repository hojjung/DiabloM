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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_MainSizeBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_IconImage;
	UPROPERTY()
	UDiaInvenGridPanel* m_PreGrid;
	UPROPERTY()
	UDiaInvenGridSlot* m_PreSlot;

public:
	int m_nPreIndex;

	FItemInstance m_DraggedItem;

public:
	void SetDDO(const FItemInstance& itemInst);

	void InitDrag(UDiaInvenGridSlot* preSlot);//slot turn blur

public:
	UFUNCTION()
	void OnDragCancel(UDragDropOperation* meSelf);
	UFUNCTION()
	void OnDrop(UDragDropOperation* meSelf);
};
