// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "SkillHotkeyPanel.h"
#include "Blueprint/UserWidget.h"
#include "Datas/PlayerUpgradeData.h"
#include "Managers/GameMode/PVPGameMode.h"

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
	UPROPERTY(Transient,meta = (BindWidgetAnim),meta=(AllowPrivateAccess = "true"))
	UWidgetAnimation* ShowResult;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UMaterialProgressBar* m_BarPlayerDmg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UTextBlock* m_TxtPlayerDmgValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UTextBlock* m_TxtOtherPlayerDmgValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UTextBlock* m_TxtRequestedInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UTextBlock* m_TxtPlayerName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UTextBlock* m_TxtOtherPlayerName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UTextBlock* m_TxtTimer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	USkillHotkeyPanel* m_PlayerEquippedSkill;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UDiaSkillUseButton* m_OtherPlayerSkill01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UDiaSkillUseButton* m_OtherPlayerSkill02;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UDiaSkillUseButton* m_OtherPlayerSkill03;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UDiaSkillUseButton* m_OtherPlayerSkill04;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_PlayerBarRage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TxtPlayerRageValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_OtherPlayerBarRage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TxtOtherPlayerRageValue;

public:
	virtual void NativeOnInitialized() override;

	virtual void NativeDestruct() override;

	void UpdateDmgGauge(float percentOne,BigInt playerDmg,BigInt otherPlayerDmg);
	
	void ReqeustPopupText(const FText& str);

	void UpdateTimer(float timer);

	void OnBattleEnd(bool isPlayerWon);

	void OnEquipSkillSet(TArray<FSkillSpec*>& equippedSkill);

	void PlayerUpdateRageBar(float cV,float mV);

	void OtherPlayerUpdateRageBar(float cV,float mV);

	void OnOtherPlayerSpawned(AOtherPlayerPawn* pawn);	
};

