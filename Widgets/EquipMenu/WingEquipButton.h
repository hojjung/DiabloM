// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Managers/EquipManager.h"
#include "Widgets/CommonElement/ImageAndText.h"

#include "WingEquipButton.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UWingEquipButton : public UUserWidget
{
	GENERATED_BODY()
public:
	UWingEquipButton(const FObjectInitializer& objInit);
	
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

	const FWingSpec* m_WingSpec;

	int m_nIndex;
	
protected:
	void SetDescPreviewText(const FWingSpec& data);

	void SetEquipped(bool b);

	void SetCombineText(int stack);
	
public:
	void Init(const FWingSpec& data,int index);

	void UpdateEquipWing();
	
	UFUNCTION()
    void TryEquip();

	UFUNCTION()
    void TryCombine();
};
