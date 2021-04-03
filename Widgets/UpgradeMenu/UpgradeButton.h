// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Button.h"
#include "Image.h"
#include "Blueprint/UserWidget.h"
#include "Managers/PlayerUpgradeManager.h"
#include "Widgets/CommonElement/ImageAndText.h"



#include "UpgradeButton.generated.h"

struct FUpgradeDataRow;
DECLARE_DELEGATE(FOnCharge);
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

	FOnCharge m_OnCharge;
protected:
	FTextFormat m_FormatName;	

	BigInt m_BigIntCost;

	int m_nCurrentLevel;

	int m_nMaxLevel;

	FText m_NameText;

	FString m_DescFormat;

	float m_fDeltaCounter;

	bool m_bChargeUpgrade;
	
	const FUpgradeSpec* m_CurrentUpgrade; 

protected:
	void SetDescPreviewText(const FUpgradeSpec& data);
	
	void UpdateUpgradeable();
	
	void SetCostText(const BigInt& v);
	
public:
	virtual void NativeOnInitialized() override;
	
	void SetUpgradeVisual(const FUpgradeSpec& data);

	void UpdateLevelText();
	

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION()
    void OnClickButton();

	UFUNCTION()
	void ChargeStart();

	UFUNCTION()
    void ChargeEnd();
};
