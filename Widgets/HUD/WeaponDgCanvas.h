#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Managers/DgContentManagers/WeaponScrollDgManager.h"
#include "Widgets/SkillHotkeyPanel.h"
#include "Widgets/CommonElement/MaterialProgressBar.h"

#include "WeaponDgCanvas.generated.h"

/**
 * 
 */
UCLASS() 
class DIABLOM_API UWeaponDgCanvas : public UUserWidget
{
	GENERATED_BODY()
protected:
	//킬,강화석 개수 보여주기
	//타이머?
	
	UPROPERTY(Transient,meta = (BindWidgetAnim),meta=(AllowPrivateAccess = "true"))
	UWidgetAnimation* ShowResult;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UMaterialProgressBar* m_RageBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UTextBlock* m_TxtRageValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UMaterialProgressBar* m_BarTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UTextBlock* m_TxtTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	USkillHotkeyPanel* m_SkillPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UTextBlock* m_TxtRequestedInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UImageAndText* m_TxtMobCount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UImageAndText* m_TxtWStoneCount;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UImageAndText* m_ImgTxtKilledMonsters;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UImageAndText* m_ImgTxtBeforeWeaponStones;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UImageAndText* m_ImgTxtRewardWeaponStones;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UImageAndText* m_ImgTxtAfterWeaponStones;
	
	FDelegateHandle m_Handle;

	TWeakObjectPtr<UWeaponScrollDgManager> m_WeaponDgManager;

	int m_nBeforeWeaponStones;

	int m_nMonsterCount;

	int m_nGainedWeaponStones;
	
public:
	virtual void NativeOnInitialized() override;

	void UpdateMobWeaponCount(int mobCount,int earnedStoneCount);

	void UpdateTimer(float timer);

	void PlayerUpdateRageBar(float cV,float mV);
	
	void OnBattleEnd(bool isPlayerWon);
};
