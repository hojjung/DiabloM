#pragma once

#include "DailyRewardPanel.h"
#include "DiabloM.h"
#include "GridPanel.h"
#include "RankingPanel.h"
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget* m_PanelPlayerInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UStageSelectPanel* m_PanelStageSelect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	URankingPanel* m_PanelRanking;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget* m_PanelMail;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDailyRewardPanel* m_PanelDaily;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget* m_PanelOption;

protected:
	virtual void NativeOnInitialized() override;
	
public:
	UFUNCTION()
	void ClosePanel();
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

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
};
