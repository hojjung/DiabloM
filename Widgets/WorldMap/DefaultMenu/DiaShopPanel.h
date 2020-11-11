#pragma once

#include "DiabloM.h"
#include "CheckBox.h"
#include "GridPanel.h"
#include "Blueprint/UserWidget.h"
#include "Village/ShopKeeper.h"

#include "DiaShopPanel.generated.h"

struct FItemInstance;
class UItemPopupInfo;
class UDiaInvenGridSlot;
class UInventory;
class UDefaultMenu;

UCLASS()
class DIABLOM_API UDiaShopPanel : public UUserWidget
{
    GENERATED_BODY()

public:
    UDiaShopPanel(const FObjectInitializer& objInit);

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
    UCheckBox* m_BtnPanel1;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    UCheckBox* m_BtnPanel2;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    UCheckBox* m_BtnPanel3;

protected:
    UPROPERTY()
    TArray<UDiaInvenGridSlot*> m_ArySlot1;
    UPROPERTY()
    TArray<UDiaInvenGridSlot*> m_ArySlot2;
    UPROPERTY()
    TArray<UDiaInvenGridSlot*> m_ArySlot3;
    UPROPERTY()
    TArray<UCheckBox*> m_AryBtns;
    UPROPERTY()
    TArray<UGridPanel*> m_AryGridPanels;

    TArray<TArray<UDiaInvenGridSlot*>*> m_AryArySlots;

    TArray<UInventory*>* m_PtrAryStorages;

    int m_nCurrentSelectedPanelIndex;


public:
    void Init();

public:
    void SetGrid(int indexPanel, int x, int y);

    void UpdateSlot(int index, FItemInstance& itemInst);

    bool AddItem(int index, FItemInstance& itemWantAdd);

    bool AddItemAuto(FItemInstance& itemWantAdd);

    void AddItemStack(int index);

    bool SellItemAuto(FItemInstance& itemWantAdd);

    FORCEINLINE const TArray<UInventory*>& GetInven() const
    {
        return *m_PtrAryStorages;
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

public:
    void UpdateShop(int panelIndex, TArray<FItemInstance>& itemAdd);
    void UpdatePanel(AShopKeeper* shop_keeper);
    void ClearPanel();
};
