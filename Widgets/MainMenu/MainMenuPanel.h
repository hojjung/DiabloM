#pragma once

#include "DiabloM.h"
#include "GridPanel.h"
#include "StageSelectPanel.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuPanel.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UMainMenuPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPlayerInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnMapSelect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRanking;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnMail;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnDaily;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnOption;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget* m_PanelPlayerInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UStageSelectPanel* m_PanelStageSelect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget* m_PanelRanking;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget* m_PanelMail;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget* m_PanelDaily;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget* m_PanelOption;

protected:
	virtual void NativeOnInitialized() override;
	
public:
	UFUNCTION()
    void OpenPlayerInfoPanel();
	UFUNCTION()
	void OpenMapSelectPanel();
	UFUNCTION()
    void OpenRankingPanel();
	UFUNCTION()
    void OpenMailPanel();
	UFUNCTION()
    void OpenDailyPanel();
	UFUNCTION()
    void OpenOptionPanel();
	
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

	
};
