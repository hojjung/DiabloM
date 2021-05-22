#pragma once

#include "DiabloM.h"
#include "WidgetSwitcher.h"
#include "Blueprint/UserWidget.h"
#include "ShopPanel.generated.h"

class UShopManager;
UCLASS()
class DIABLOM_API UShopPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	UShopManager* m_ShopManager;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPackage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnGemStone;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnGold;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetSwitcher* m_SwitcherPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPurchasePackage01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    UButton* m_BtnPurchasePackage02;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPurchasePackage03;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPurchasePackage04;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPurchasePackage05;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPurchaseGemStone01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPurchaseGemStone02;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPurchaseGemStone03;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPurchaseGemStone04;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPurchaseGemStone05;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPurchaseGemStone06;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPurchaseDgKeyRefill;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPurchaseGold01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPurchaseGold02;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPurchaseGold03;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextGoldSmallAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextGoldMidiumAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextGoldLargeAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_PackageItem01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_PackageItem02;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_PackageItem03;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_PackageItem04;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_PackageItem05;


public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void PurchasePacakge01();
	UFUNCTION()
    void PurchasePacakge02();
	UFUNCTION()
    void PurchasePacakge03();
	UFUNCTION()
    void PurchasePacakge04();
	UFUNCTION()
    void PurchasePacakge05();
	UFUNCTION()
    void PurchaseGemStone01();
	UFUNCTION()
    void PurchaseGemStone02();
	UFUNCTION()
    void PurchaseGemStone03();
	UFUNCTION()
    void PurchaseGemStone04();
	UFUNCTION()
    void PurchaseGemStone05();
	UFUNCTION()
    void PurchaseGemStone06();
	UFUNCTION()
    void PurchaseGold01();
	UFUNCTION()
    void PurchaseGold02();
	UFUNCTION()
    void PurchaseGold03();
	UFUNCTION()
	void PurchaseDgKey();
	UFUNCTION()
	void ClosePanel();
	UFUNCTION()
	void ShowPackagePanel();
	UFUNCTION()
    void ShowGemStonePanel();
	UFUNCTION()
    void ShowGoldPanel();

	void ShowDgKeyPanel();
	
	void OnUpdateGoldShop(const BigInt&,const BigInt&,const BigInt&);

	void UpdateShowPackage();
	
};
