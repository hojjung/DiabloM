// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/ImageAndTextAndCompare.h"
#include "Components/VerticalBox.h"
#include "Datas/ItemDataTable.h"
#include "ItemPopupInfo.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UItemPopupInfo : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	USizeBox* m_SizeInfoPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UVerticalBox* m_VerticalOptionList;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UTextBlock* m_TextItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UTextBlock* m_TextItemTierAndType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImage* m_ImageItemTierColorSmall;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImage* m_ImageItemTierColorLarge;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImage* m_ImageItemVisualIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImageAndTextAndCompare* m_MainOptionAttack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImageAndTextAndCompare* m_MainOptionDefense;
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

protected:
	UPROPERTY()
	TArray< UImageAndText*> m_AryOptions;
	UPROPERTY()
	UEnum* m_ItemTypeString;

	int m_nOptionCount;

protected:
	FText GetItemTypeTxt(EItemType typeV) const;
	void SetIcon(const FItemInstance& itemInst);
	void SetColorTier(const FItemInstance& itemInst);
	
	void SetItemText(const FItemInstance& itemInst);
	void SetFlavorText(const FItemInstance& itemInst);
	void SetOptionTexts(const FItemInstance& itemInst);
public:
	UFUNCTION(BlueprintCallable, Category = "Info")
	void HideAllSubOptions();
	UFUNCTION(BlueprintCallable,Category="Info")
	void SetInfoPanel(const FItemInstance& itemInst);
	UFUNCTION(BlueprintCallable, Category = "Info")
	void SetPanelSizeByOption(int optionCount);
	//void SetMainOptions();
};
