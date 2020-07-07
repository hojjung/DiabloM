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

struct FItemInstance;
class UItemPopupInfo;
class UDiaInvenGridSlot;
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
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UItemPopupInfo* m_ItemPopup;

protected:
	Inventory* m_Inven;

public:
	void Init(Inventory* itemContainer);

protected:
	void ShowItemInfo(const FGeometry & theInstigator, const FItemInstance& itemInst);

	void HideItemInfo();

public:
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetGrid(int x,int y);
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void UpdateSlot(int index, const FItemInstance& itemInst);
	UFUNCTION(BlueprintCallable, Category = "Widget")
	bool AddItem(int index, FItemInstance& itemWantAdd);
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void AddItemStack(int index);
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void RemoveItem(int index);
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void RemoveItemStack(int index);


	FORCEINLINE Inventory* GetInven() const
	{
		return m_Inven;
	}

	void BoxDraw2(FPaintContext& cont, FVector2D point, float widthheight, FLinearColor color) const;

	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	//

private:
	FVector2D ClickedItemSlot;
	FVector2D B;
	FVector2D C;
};
