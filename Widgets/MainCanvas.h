// My First Hack n Slash

#pragma once

#include "DiabloM.h"

#include "Blueprint/UserWidget.h"
#include "CommonElement/ImageAndText.h"
#include "CommonElement/MaterialProgressBar.h"
#include "UpgradeMenu/UpgradePanel.h"


#include "MainCanvas.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UMainCanvas : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
protected:	
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
	UUserWidget*     m_PanelQuest;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUpgradePanel*   m_PanelUpgrade;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget*	 m_PanelEquipment;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget*	 m_PanelGacha;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget*	 m_PanelShop;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget*     m_PanelMenu;
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
	//
	float m_fMaxBossTime;
	float m_fTimeCounter;
	FTextFormat m_Format;
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void ShowBossUI();
	
	void HideBossUI();

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
};


