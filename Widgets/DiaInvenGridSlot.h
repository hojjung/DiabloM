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
UCLASS()
class DIABLOM_API UDiaInvenGridSlot : public UUserWidget
{
	GENERATED_BODY()
	
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDiaDragDrop> m_ClassDDO;
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

	UPROPERTY()
	UDiaInvenGridPanel* m_ParentGrid;
protected:
	int m_nIndex;

	FItemInstance m_CopiedItemData;

public:
	static UDiaDragDrop* GetDDOInst;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_SizeItemVisual;
public:

	UFUNCTION(BlueprintCallable, Category = "Item")
	void InitSlot(int indexFromGrid,UDiaInvenGridPanel* parentGrid);
	UFUNCTION(BlueprintCallable,Category="Item")
	void SetSlot(const FItemInstance& itemInstance);
	UFUNCTION(BlueprintCallable, Category = "Item")
	void ClearSlot();

	void SetIconOpacity(float opacityMaxOne);

	bool IsSlotEmpty();

protected:
	void UpdateText(const FItemInstance& itemInstance);

	void UpdateEffectBG(const FItemInstance& itemInstance);

	void UpdateItemVisual(const FItemInstance& itemInstance);

	void SetSlotFocus();

	void ClearSlotFocus();

	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	//
	UDiaDragDrop* CreateDDO(const FItemInstance& itemInst);

	virtual bool CanDrop(const FItemInstance& itemInst);//for slot limit

public:
	FORCEINLINE int  GetIndex()
	{
		return m_nIndex;
	}
};
