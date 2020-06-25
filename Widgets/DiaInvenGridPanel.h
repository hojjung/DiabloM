// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/DiaInvenGridSlot.h"
#include "Components/GridPanel.h"

#include "DiaInvenGridPanel.generated.h"

/**
 * 
 */

class Inventory;
class UDefaultMenu;
UCLASS()
class DIABLOM_API UDiaInvenGridPanel : public UUserWidget
{
	GENERATED_BODY()
public:
	UDiaInvenGridPanel(const FObjectInitializer& objInit);
	
protected:
	UPROPERTY(EditDefaultsOnly,Category="Widget")
	TSubclassOf<UDiaInvenGridSlot> m_ClassGridSlot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UGridPanel* m_SlotGridPanel;
	UPROPERTY()
	TArray<UDiaInvenGridSlot*> m_ArySlot;
protected:
	Inventory* m_Inven;

public:
	void Init(Inventory* itemContainer);

public:
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetGrid(int x,int y);
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void UpdateSlot(int index, const FItemInstance& itemInst);
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void AddItem(int index, FItemInstance& itemWantAdd);
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void AddItemStack(int index);
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void RemoveItem(int index);
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void RemoveItemStack(int index);

	bool OnDropHeapedIndex(int dropIndex, int dragIndex);
};
