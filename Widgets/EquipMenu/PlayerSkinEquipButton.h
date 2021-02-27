// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Managers/EquipManager.h"

#include "PlayerSkinEquipButton.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPlayerSkinEquipButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerSkinEquipButton(const FObjectInitializer& objInit);
	

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
	FTextFormat m_FormatCombine;

	const FPlayerClassSpec* m_SkinSpec;

	int m_nIndex;

protected:
	void SetDescPreviewText(const FPlayerClassSpec& data);

	void SetEquipped(bool b);

	void SetCombineText(int stack);

public:
	void Init(const FPlayerClassSpec& data, int index);

	void UpdateEquipSkin();

	UFUNCTION()
	void TryEquip();
};
