// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Button.h"
#include "Image.h"
#include "Blueprint/UserWidget.h"
#include "CommonElement/ImageAndText.h"

#include "UpgradeButton.generated.h"

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
	
private:
	void UpdateUpgradeable();
	
public:
	void SetUpgradeVisual(UTexture2D* icon,const FText& tName,int currentLevel,int maxLevel);

	void SetCostText(const BigInt& v);

	void SetDescPreviewText(const FText& tDesc);

	void UpdateLevelText(int newV);
};
