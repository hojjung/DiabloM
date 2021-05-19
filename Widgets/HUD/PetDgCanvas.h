// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "ScrollBar.h"
#include "ScrollBox.h"
#include "Blueprint/UserWidget.h"
#include "Characters/Pawns/MonsterPawn.h"
#include "Widgets/SkillHotkeyPanel.h"
#include "Managers/DgContentManagers/PetDgManager.h"

#include "PetDgCanvas.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPetDgCanvas : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Transient,meta = (BindWidgetAnim),meta=(AllowPrivateAccess = "true"))
	UWidgetAnimation* ShowResult;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UMaterialProgressBar* m_RageBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UTextBlock* m_TxtRageValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	USkillHotkeyPanel* m_SkillPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UMaterialProgressBar* m_BarTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UTextBlock* m_TxtTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UTextBlock* m_TxtRequestedInfo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UMaterialProgressBar* m_MonsterCountBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UTextBlock* m_TxtMonsterCountRemain;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UImageAndText* m_ImgTxtRemainTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UImageAndText* m_ImgTxtBeforePetGachaTickets;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UImageAndText* m_ImgTxtGainedPetGachaTickets;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UImageAndText* m_ImgTxtAfterPetGachaTickets;
	
	FDelegateHandle m_Handle;

	TWeakObjectPtr<UPetDgManager> m_PetDgManager;

	int m_nBeforeTickets;
	
public:
	virtual void NativeOnInitialized() override;

	void UpdateTimer(float timer);

	void OnBattleEnd(bool isPlayerWon);

	void PlayerUpdateRageBar(float cV,float mV);

	void UpdateMonsterCount();
};
