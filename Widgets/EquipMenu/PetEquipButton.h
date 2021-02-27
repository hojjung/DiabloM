// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Managers/EquipManager.h"

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
	UTextBlock* m_TextName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextEquip;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextCombine;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEquip;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCombine;

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
	
public:
	void Init(const FPetSpec& data, int index);

	void UpdateEquipAccessory();

	UFUNCTION()
	void TryEquip();
};
