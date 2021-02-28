// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Managers/EquipManager.h"

#include "AccessoryEquipButton.generated.h"

class UEquipmentPanel;
/**
 * 
 */
UCLASS()
class DIABLOM_API UAccessoryEquipButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UAccessoryEquipButton(const FObjectInitializer& objInit);
	
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
	UTextBlock* m_TextCost;
	UPROPERTY()
	UEquipmentPanel* m_ParentEquip;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEquip;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCombine;

protected:
	FTextFormat m_FormatName;
	
	FTextFormat m_FormatCost;

	const FAccessorySpec* m_AccessorySpec;

	int m_nIndex;

protected:
	void SetDescPreviewText(const FAccessorySpec& data);

	void SetEquipped(bool b);

	void SetCostText(int stack);

	void SetLevelNameText(const FAccessorySpec& data);

public:
	void Init(const FAccessorySpec& data, UEquipmentPanel* equipPanel, int index);

	void UpdateEquipAccessory();

	UFUNCTION()
	void TryEquip();
};
