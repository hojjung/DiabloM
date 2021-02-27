// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Managers/EquipManager.h"

#include "WeaponEquipButton.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UWeaponEquipButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UWeaponEquipButton(const FObjectInitializer& objInit);
	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextEquip;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextCombine;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextLevel;
	
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

	int m_nIndex;
	
protected:
	void SetDescPreviewText(const FWeaponSpec& data);

	void SetEquipped(bool b);

	void SetCombineText(int stack);

	void SetLevelNameText(const FWeaponSpec& data);
	
public:
	void Init(const FWeaponSpec& data,int index);

	void UpdateEquipWeapon();
	
	UFUNCTION()
    void TryEquip();
};
