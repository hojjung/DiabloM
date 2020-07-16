// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/SizeBox.h"
#include "Datas/ItemDataTable.h"
#include "DiaInvenGridSlot.generated.h"

/**
 * 
 */
class UDiaInvenGridPanel;
class UDiaDragDrop;


//const UUserWidget * theInstigator, const FItemInstance & itemInst
UCLASS()
class DIABLOM_API UDiaInvenGridSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	DECLARE_DELEGATE_RetVal_TwoParams(bool, FDropIndex, int, FItemInstance&);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClicked, const FGeometry &,instigator, const FItemInstance &,itemClicked);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UOverlay* m_Overlay1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgItemVisual;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgItemEffectBG;//like for legendary orange light
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgSlotHighlight;//like for legendary orange light
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgSlotBG;//normal BG
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextItemStackCount;

protected:
	int m_nIndex;

	FItemInstance m_CopiedItemData;

public:
	static UDiaDragDrop* GetDDOInst;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_SizeItemVisual;//for DDO

	FDropIndex m_OnDropIndex;

	FDropIndex m_OnDragIndex;

	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FOnClicked m_OnClicked;

public:

	UFUNCTION(BlueprintCallable, Category = "Item")
	void InitSlot(int indexFromGrid);
	UFUNCTION(BlueprintCallable,Category="Item")
	void SetSlot(const FItemInstance& itemInstance);
	UFUNCTION(BlueprintCallable, Category = "Item")
	void ClearSlot();

	void SetIconOpacity(float opacityMaxOne);

	void SetVisualColorTint(FLinearColor colorW);

	void SetHighlightColorTint(FLinearColor colorW);

	bool IsSlotEmpty();

	void UpdateItemVisual(const FItemInstance& itemInstance);
protected:
	void UpdateText(const FItemInstance& itemInstance);

	void UpdateEffectBG(const FItemInstance& itemInstance);


	void SetSlotFocus(UDiaDragDrop* ddo);

	void ClearSlotFocus();

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	//
	UDiaDragDrop* CreateDDO(const FItemInstance& itemInst);


public:
	FORCEINLINE int  GetIndex()
	{
		return m_nIndex;
	}
};
