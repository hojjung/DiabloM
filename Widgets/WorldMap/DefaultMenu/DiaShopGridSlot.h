// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Datas/ItemDataTable.h"
#include "DiaShopGridSlot.generated.h"

class UDiaInvenGridPanel;
class UDiaDragDrop;


UCLASS()
class DIABLOM_API UDiaShopGridSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	DECLARE_DELEGATE_RetVal_TwoParams(bool, FDropIndex, int, FItemInstance&);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnClicked, const FGeometry &,  FItemInstance &);
	DECLARE_DELEGATE(FOnDrag);
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextItemLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgTypeIcon;
protected:
	int m_nIndex;

	FItemInstance m_CopiedItemData;

	FTextFormat m_LevelFormat;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_SizeItemVisual;//for DDO

	FDropIndex m_OnDropIndex;

	FDropIndex m_OnDragIndex;

	FOnClicked m_OnClicked;
	
	FOnDrag m_OnDragDetect;

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
	
	void SetLevelText(int itemLevel);

	void SetSlotFocus(UDiaDragDrop* ddo);

	void ClearSlotFocus();

	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent) override;
	//
	UDiaDragDrop* CreateDDO(const FItemInstance& itemInst);

public:
	FORCEINLINE int  GetIndex()
	{
		return m_nIndex;
	}
};
