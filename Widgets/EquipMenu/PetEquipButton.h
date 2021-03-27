// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Managers/EquipManager.h"
#include "Widgets/CommonElement/ImageAndText.h"


#include "PetEquipButton.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPetEquipButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UPetEquipButton(const FObjectInitializer& objInit);
	
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

	
	const FPetSpec* m_PetSpec;

	int m_nIndex;

protected:
	void SetDescPreviewText(const FPetSpec& data);

	void SetEquipped(bool b);

	void SetCombineText(int stack);

	void SetLevelNameText(const FPetSpec& data);

	void SetCostText();
	
public:
	void Init(const FPetSpec& data, int index);

	void UpdateEquipPet();

	UFUNCTION()
	void TryEquip();
	UFUNCTION()
    void TryCombine();
	UFUNCTION()
    void TryLvUp();
};
