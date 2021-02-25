// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "CommonElement/ImageAndText.h"

#include "EquipmentButton.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UEquipmentButton : public UUserWidget
{
	GENERATED_BODY()
public:
	UEquipmentButton(const FObjectInitializer& objInit);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_TextCost;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnLvUp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCombine;

protected:
	FTextFormat m_FormatName;	

	BigInt m_BigIntCost;

	int m_nCurrentLevel;

	int m_nMaxLevel;

	FText m_NameText;

	FString m_DescFormat;
	
	void SetDescPreviewText(const FUpgradeSpec& data);
	
public:
	void UpdateUpgradeable();
	
	void SetUpgradeVisual(const FUpgradeSpec& data);

	void SetCostText(const BigInt& v);

	void UpdateLevelText(const FUpgradeSpec& data);
};
