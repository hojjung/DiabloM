// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "CommonElement/ImageAndText.h"

#include "EquipmentButton.generated.h"

struct FEquipmentSpec;
struct FPlayerClassSpec;
struct FUpgradeSpec;
/**
 * 
 */
UCLASS()
class DIABLOM_API UEquipmentButton : public UUserWidget
{
	GENERATED_BODY()
public:
	UEquipmentButton(const FObjectInitializer& objInit);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_TextCost;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextEquip;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextCombine;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEquip;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnLvUp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCombine;

	
protected:
	FTextFormat m_FormatName;

	FTextFormat m_FormatCombine;

	BigInt m_BigIntCost;

	int m_nCurrentLevel;

	int m_nMaxLevel;

	FText m_NameText;

	FString m_DescFormat;

	const FPlayerClassSpec* m_PlSkinData;
	
	const FEquipmentSpec* m_EquipData;

protected:
	void SetDescPreviewText(const FPlayerClassSpec& data);

	void SetDescPreviewText(const FEquipmentSpec& data);
	
	void SetCostText(const BigInt& v);
	
	void UpdateUpgradeable();
	
public:
	void SetUpgradeVisual(const FEquipmentSpec& data);
	
	void UpdateLevelText(const FEquipmentSpec& data);
	
	void SetUpgradeVisual(const FPlayerClassSpec& data);
	
	void UpdateLevelText(const FPlayerClassSpec& data);

	void SetEquipped(bool b);

	void SetCombineText(int stack);

	UFUNCTION()
	void TryEquip();

};
