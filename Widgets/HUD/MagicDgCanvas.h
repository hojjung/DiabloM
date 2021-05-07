// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Characters/Pawns/MonsterPawn.h"
#include "Widgets/SkillHotkeyPanel.h"
#include "Widgets/CommonElement/MaterialProgressBar.h"

#include "MagicDgCanvas.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UMagicDgCanvas : public UUserWidget
{
	GENERATED_BODY()
protected:
	FDelegateHandle m_Handle;
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
	UMaterialProgressBar* m_BarBossHp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UTextBlock* m_TxtBossHp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	USkillHotkeyPanel* m_SkillPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UTextBlock* m_TxtRequestedInfo;
	
	
public:
	virtual void NativeOnInitialized() override;

	void UpdateDragonHp(float percentOne);
	
	void UpdateTimer(float timer);

	void OnBattleEnd(bool isPlayerWon);

	void PlayerUpdateRageBar(float cV,float mV);

	void OnDragonSpawned(AMonsterPawn* pawn);
};
