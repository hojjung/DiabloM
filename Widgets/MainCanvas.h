// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "GameExitWindow.h"
#include "PopupTextWidget.h"
#include "Quest/QuestPanel.h"
#include "SkillHotkeyPanel.h"
#include "Blueprint/UserWidget.h"
#include "CommonElement/CooldownProgress.h"
#include "CommonElement/ImageAndText.h"
#include "CommonElement/MaterialProgressBar.h"
#include "GachaMenu/GachaPanel.h"
#include "MainMenu/MainMenuPanel.h"
#include "UpgradeMenu/UpgradePanel.h"


#include "MainCanvas.generated.h"

class UShopPanel;
/**
 * 
 */
UCLASS()
class DIABLOM_API UMainCanvas : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMenuOpen,bool);
	
	FOnMenuOpen m_OnMenuVisibleChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_MainCanvas;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_CanvasBannerAds;
	//top
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock*	     m_TextRanking;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText*   m_TextGemStone;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton*	     m_BtnGemStone;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText*   m_TextGold;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton*	     m_BtnGold;
	//bar
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnBoss;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UQuestPanel*     m_PanelQuest;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUpgradePanel*   m_PanelUpgrade;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget*	 m_PanelEquipment;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UGachaPanel*	 m_PanelGacha;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UShopPanel*	 m_PanelShop;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMainMenuPanel*  m_PanelMenu;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnQuest;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnUpgrade;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEquipment;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnGacha;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnShop;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnMenu;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_BarRage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TxtRageValue;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UOverlay* m_OverlayTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_BarTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TxtTime;
	//	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UOverlay* m_OverlayBossHp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_BarBossHp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TxtBossHp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCooldownProgress* m_CDBoss;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USkillHotkeyPanel* m_SkillPanel;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UGameExitWindow* m_GameExitWindow;
protected:
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* m_ShowText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UPopupTextWidget* m_PopupText;

public:
	void RequestText(FText txt);

	void RequestText(FString txt);
	
protected:
	FTimerHandle m_TextTimer;
	
	float m_fMaxBossCooldownTime;
	
	float m_fBossCooldownTimeCounter;
	
	float m_fMaxBossDurationTime;
	
	float m_fBossDurationTimeCounter;
	
	FTextFormat m_Format;
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void ShowBossUI();
	
	void HideBossUI();

	void UpdateRageBar(float cV,float mV);

	void UpdateBossHP(float per);

	void UpdateBossText(BigInt cHp);

	void UpdateTimer(float per,float cTime);

	void SetBossTimer();
	
public:
	UFUNCTION()
	void SetActiveQuestPanel();
	UFUNCTION()
	void SetActiveUpgradePanel();
	UFUNCTION()
	void SetActiveEquipmentPanel();
	UFUNCTION()
	void SetActiveGachaPanel();
	UFUNCTION()
	void SetActiveShopPanel();
	UFUNCTION()
	void SetActiveMenuPanel();

	UFUNCTION()
	void UpdateGoldUI();
	UFUNCTION()
	void SummonBoss();

	void OnBossBattleEnd(bool b);

	void SetPlayerNicknameRanking();

	UFUNCTION()
	void HideTextWidget();

	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;

	void UpdateGemStone(int v);

	UFUNCTION()
	void OpenGoldShop();
	
	UFUNCTION()
    void OpenGemStoneShop();

	void OnShowAdBanner(bool b);

	UFUNCTION()
	void OnPressBackkey();

	void OnMenuPanelVisibleChanged(bool b);

	virtual void NativeOnInitialized() override;
};



