// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "PVPCanvas.generated.h"

class UMaterialProgressBar;
/**
 * 
 */
UCLASS()
class DIABLOM_API UPVPCanvas : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_BarPlayerDmg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TxtPlayerDmgValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TxtOtherPlayerDmgValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TxtRequestedInfo;
	
public:
	virtual void NativeOnInitialized() override;

	void UpdateDmgGauge(float percentOne,BigInt playerDmg,BigInt otherPlayerDmg);
	
	void ReqeustPopupText(const FText& str);
};
