// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "CheckBox.h"
#include "GridPanel.h"
#include "Blueprint/UserWidget.h"
#include "DiaStorageGridPanel.generated.h"

class UHoverToggle;
struct FItemInstance;
class UItemPopupInfo;
class UDiaInvenGridSlot;
class UInventory;
class UDefaultMenu;
UCLASS()
class DIABLOM_API UDiaStorageGridPanel : public UUserWidget
{
	GENERATED_BODY()
public:
	static UDiaStorageGridPanel* GetStorageWidgetInst;
	
	UDiaStorageGridPanel(const FObjectInitializer& objInit);
	
protected:
	UPROPERTY(EditDefaultsOnly,Category="Widget")
	TSubclassOf<UDiaInvenGridSlot> m_ClassGridSlot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UGridPanel* m_SlotGridPanel1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UGridPanel* m_SlotGridPanel2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UGridPanel* m_SlotGridPanel3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UGridPanel* m_SlotGridPanel4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UGridPanel* m_SlotGridPanel5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UHoverToggle* m_BtnPanel1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UHoverToggle* m_BtnPanel2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UHoverToggle* m_BtnPanel3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UHoverToggle* m_BtnPanel4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UHoverToggle* m_BtnPanel5;
	
protected:
	UPROPERTY()
	TArray<UDiaInvenGridSlot*> m_ArySlot1;
	UPROPERTY()
	TArray<UDiaInvenGridSlot*> m_ArySlot2;
	UPROPERTY()
	TArray<UDiaInvenGridSlot*> m_ArySlot3;
	UPROPERTY()
	TArray<UDiaInvenGridSlot*> m_ArySlot4;
	UPROPERTY()
	TArray<UDiaInvenGridSlot*> m_ArySlot5;
	UPROPERTY()
	TArray<UCheckBox*> m_AryBtns;
	//
	UPROPERTY()
	TArray<UGridPanel*> m_AryGridPanels;
	
	TArray<TArray<UDiaInvenGridSlot*>*> m_AryArySlots;
	
	TArray<UInventory*>* m_AryStorages;

	int m_nPreSelectedPanelIndex;
	
	int m_nCurrentSelectedPanelIndex;

	//
public:
	void Init(TArray<UInventory*>* itemContainer);

public:
    void SetGrid(int indexPanel,int x,int y);
    void UpdateSlot1(int index,  FItemInstance& itemInst);
	void UpdateSlot2(int index,  FItemInstance& itemInst);
	void UpdateSlot3(int index,  FItemInstance& itemInst);
	void UpdateSlot4(int index,  FItemInstance& itemInst);
	void UpdateSlot5(int index,  FItemInstance& itemInst);
    bool AddItem(int index, FItemInstance& itemWantAdd);
    bool AddItemAuto(FItemInstance& itemWantAdd);
    void AddItemStack(int index);

	FORCEINLINE const TArray<UInventory*>& GetInven() const
	{
		return *m_AryStorages;
	}

	FORCEINLINE TArray<TArray<UDiaInvenGridSlot*>*>& GetArySlots2()
	{
		return m_AryArySlots;
	}

public:
	UFUNCTION()
	void Open1(bool bOpen);
	UFUNCTION()
    void Open2(bool bOpen);
	UFUNCTION()
    void Open3(bool bOpen);
	UFUNCTION()
    void Open4(bool bOpen);
	UFUNCTION()
    void Open5(bool bOpen);
};



