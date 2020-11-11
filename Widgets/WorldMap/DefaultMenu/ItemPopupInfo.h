#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/WorldMap/DefaultMenu/ImageAndTextAndCompare.h"
#include "Widgets/WorldMap/DefaultMenu/ImageAndText.h"
#include "Datas/ItemDataTable.h"
#include "Animation/WidgetAnimation.h"
#include "ItemPopupInfo.generated.h"



UENUM(BlueprintType)//need string
enum class EPopupType: uint8
{
	None,
	Deposite,
	Withdraw,
	Equip,
	Unequip,
	Sell,
	Buy
};

DECLARE_MULTICAST_DELEGATE(FOnActionEnd);

UCLASS()
class DIABLOM_API UItemPopupInfo : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized()override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	USizeBox* m_SizeInfoPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UVerticalBox* m_VerticalOptionList;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UTextBlock* m_TextItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UTextBlock* m_TextItemTier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UTextBlock* m_TextItemType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImage* m_BGForTouch;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImage* m_ImageItemTierColorSmall;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImage* m_ImageItemTierColorLarge;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImage* m_ImageItemVisualIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImageAndTextAndCompare* m_MainOption;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImageAndText* m_SubOption1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImageAndText* m_SubOption2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImageAndText* m_SubOption3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImageAndText* m_SubOption4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImageAndText* m_SubOption5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImageAndText* m_SubOption6;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImageAndText* m_SubOption7;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImageAndText* m_SubOption8;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImageAndText* m_SubOption9;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImageAndText* m_SubOption10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImageAndText* m_SubOption11;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UTextBlock* m_TextFlavor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImageAndText* m_TextSellValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UButton* m_UseButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UButton* m_EquipButton;//equip패널은 그냥 unequip만있으면 된다,인벤은 둘다있어야한다
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UButton* m_UnequipButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UButton* m_WithdrawButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UButton* m_DepositeButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UButton* m_SellButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UButton* m_BuyButton;
protected:
	UPROPERTY()
	TArray< UImageAndText*> m_AryOptions;
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* m_FadeAnimation;

	FItemInstance m_SelectedItem;

	FTimerHandle m_TimerHandle;

	FOnActionEnd m_OnActionEnd;

	FVector2D m_InitPos;

protected:
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )override;
	
	void SetIcon(const FItemInstance& itemInst);
	
	void SetColorTier(const FItemInstance& itemInst);
	
	void SetItemText(const FItemInstance& itemInst);

	float SetFlavorText(const FItemInstance& itemInst);
	
	float SetOptionTexts(const FItemInstance& itemInst);

	void PopupBtnToggle(EPopupType popup);
	
public:
	FOnActionEnd& GetOnActionEnd()
	{
		return m_OnActionEnd;
	}
	FItemInstance& GetSelectedItem()
	{
		return m_SelectedItem;
	}
	UFUNCTION()
    void UseItem();
	UFUNCTION()
    void EquipItem();
	UFUNCTION()
    void UnequipItem();
	UFUNCTION()
	void DepositeItem();
	UFUNCTION()
    void WithdrawItem();
	UFUNCTION()
    void BuyItem();
	UFUNCTION()
    void SellItem();
	
public:
	UFUNCTION(BlueprintCallable, Category = "Info")
	void HideAllSubOptions();
	UFUNCTION(BlueprintCallable, Category = "Info")
	void HideFlavorText();
	UFUNCTION(BlueprintCallable,Category="Info")
	void SetPanelPosition(const FGeometry & theInstigator,int countSpace=0,bool bLeft=true);
	UFUNCTION(BlueprintCallable,Category="Info")
	void ShowInfoPanel(EPopupType popupType,FItemInstance& itemInst);

	UFUNCTION(BlueprintCallable,Category="Info")
	void PlayHideInfoAnim(float delay=0.f);

	UFUNCTION(BlueprintCallable,Category="Info")
	void HideInfoPanel();

	FORCEINLINE UButton* GetEquipButton()
	{
		return m_EquipButton;
	}

	FORCEINLINE UButton* GetUseButton()
	{
		return m_UseButton;
	}

	void CompareItem(float origin,float wantCompareOther);
};


