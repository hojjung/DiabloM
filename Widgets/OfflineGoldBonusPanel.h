// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "CommonElement/MaterialProgressBar.h"

#include "OfflineGoldBonusPanel.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UOfflineGoldBonusPanel : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_OfflineTimeGauge;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextOfflineGoldGained;//format
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextOfflineGoldCurrent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextOfflineGoldMax;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnConfirm;

	FTextFormat m_FormatOfflineGold;

	FTextFormat m_FormatOfflineTime;
	
public:
	virtual void NativeOnInitialized() override;
	
	void SetOfflineGold(BigInt gold);

	UFUNCTION()
	void OnConfirm();
};
