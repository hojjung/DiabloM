#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/WorldMap/DefaultMenu/DiaInvenGridSlot.h"
#include "DiaInvenGridPanel.generated.h"


struct FItemInstance;
class UItemPopupInfo;
class UDiaInvenGridSlot;
class UInventory;
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
	UPROPERTY()
	UInventory* m_Inven;

	int m_nPopupSelectedIndex;
	
public:
	void Init(UInventory* itemContainer);

public:
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetGrid(int x,int y);
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void UpdateSlot(int index,  FItemInstance& itemInst);
	UFUNCTION(BlueprintCallable, Category = "Widget")
	bool AddItem(int index, FItemInstance& itemWantAdd);
	UFUNCTION(BlueprintCallable, Category = "Widget")
    bool AddItemAuto(FItemInstance& itemWantAdd);
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void AddItemStack(int index);
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void RemoveItem(int index);
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void RemoveItemStack(int index);


	FORCEINLINE UInventory* GetInven() const
	{
		return m_Inven;
	}

	FORCEINLINE const TArray<UDiaInvenGridSlot*>& GetArySlots() const
	{
		return m_ArySlot;
	}

public:
	static UDiaInvenGridPanel* GetInvenWidgetInst;
	
};
