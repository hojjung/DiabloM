// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "UpgradeButton.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CommonElement/ImageAndText.h"

#include "SkillUpgradeButton.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API USkillUpgradeButton : public UUserWidget
{
	GENERATED_BODY()


public:
	USkillUpgradeButton(const FObjectInitializer& objInit);

	static USkillUpgradeButton* CurrentSelected;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMyClick,USkillUpgradeButton*);

	FOnMyClick m_OnClicked;
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
	UButton* m_BtnEquip;

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

	FButtonStyle m_BtnStyle;

	FSkillSpec* m_EquippedSkillSpec;

protected:
	void SetDescPreviewText(const FSkillSpec& data);

	void UpdateUpgradeable();

	void SetCostText(const BigInt& v);

protected:

	FSlateBrush m_CachedNormal;

public:
	virtual void NativeOnInitialized() override;

	void Init(FSkillSpec& data);

	void SetHoverImage();

	void SetNormalImage();

	void UpdateSkillButton();
	UFUNCTION()
	void OnClickButton();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void ChargeStart();

	UFUNCTION()
	void ChargeEnd();

	FORCEINLINE FSkillSpec* GetSkillSpec() 
	{
		return m_EquippedSkillSpec;
	}

	UFUNCTION()
    void OnClick();
};
