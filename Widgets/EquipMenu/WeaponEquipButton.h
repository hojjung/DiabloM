// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Managers/EquipManager.h"
#include "Widgets/CommonElement/ImageAndText.h"

#include "WeaponEquipButton.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UWeaponEquipButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgTierColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextEquip;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextCombine;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_ImagTxtCost;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEquip;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCombine;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnLvUp;

protected:
	FTextFormat m_FormatName;
	
	FTextFormat m_FormatCombine;

	const FWeaponSpec* m_WeaponSpec;

	UPROPERTY()
	int m_nIndex;

	UPROPERTY()
	float m_fDeltaCounter;
	UPROPERTY()
	bool m_bChargeUpgrade;
	
protected:
	void SetDescPreviewText(const FWeaponSpec& data);

	void SetEquipped(bool b);

	void SetCombineText(int stack);

	void SetLevelNameText(const FWeaponSpec& data);
	
	void SetCostText();

public:
	void Init(const FWeaponSpec& data,int index);

	void UpdateEquipWeapon();
	
	UFUNCTION()
    void TryEquip();
	UFUNCTION()
    void TryCombine();
	UFUNCTION()
    void TryLvUp();
	

	UFUNCTION()
    void ChargeStart();

	UFUNCTION()
    void ChargeEnd();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
