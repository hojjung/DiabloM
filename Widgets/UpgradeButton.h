// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Button.h"
#include "Image.h"
#include "Blueprint/UserWidget.h"
#include "CommonElement/ImageAndText.h"
#include "Managers/PlayerUpgradeManager.h"


#include "UpgradeButton.generated.h"

struct FUpgradeDataRow;
/**
 * 
 */
UCLASS()
class DIABLOM_API UUpgradeButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUpgradeButton(const FObjectInitializer& objInit);

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

protected:
	FTextFormat m_FormatName;	

	BigInt m_BigIntCost;

	int m_nCurrentLevel;

	int m_nMaxLevel;

	FText m_NameText;

	FString m_DescFormat;
	
	void SetDescPreviewText(const FUpgradeSpec& data);
	
	void UpdateUpgradeable();
	
	void SetCostText(const BigInt& v);
	
public:
	void SetUpgradeVisual(const FUpgradeSpec& data);

	void UpdateLevelText(const FUpgradeSpec& data);
};
