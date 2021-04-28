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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TxtPlayerName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TxtOtherPlayerName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TxtTimer;
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* ShowResult;

	
public:
	virtual void NativeOnInitialized() override;

	void UpdateDmgGauge(float percentOne,BigInt playerDmg,BigInt otherPlayerDmg);
	
	void ReqeustPopupText(const FText& str);

	void UpdateTimer(float timer);

	void OnBattleEnd(bool isPlayerWon);
};

