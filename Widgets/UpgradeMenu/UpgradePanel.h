// My First Hack n Slash

#pragma once

#include "DiabloM.h"


#include "Button.h"
#include "SkillUpgradeButton.h"
#include "UpgradeButton.h"
#include "WidgetSwitcher.h"
#include "Blueprint/UserWidget.h"
#include "Managers/PlayerUpgradeManager.h"
#include "Widgets/SkillHotkeyPanel.h"
#include "UpgradePanel.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UUpgradePanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUpgradePanel(const FObjectInitializer& objInit);

protected:
	virtual void NativeOnInitialized() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnDefaultStat;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnSkill01;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetSwitcher* m_SwitcherUpgradePanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_PanelDefaultStat;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_PanelDefaultSkill;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUpgradeButton* m_UpgradeAtkDmg01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUpgradeButton* m_UpgradeAtkCri01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUpgradeButton* m_UpgradeAtkCDmg01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUpgradeButton* m_UpgradeAtkCri02;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUpgradeButton* m_UpgradeAtkCDmg02;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUpgradeButton* m_UpgradeAtkMagic01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUpgradeButton* m_UpgradeAtkMDmg01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUpgradeButton* m_UpgradeAtkMagic02;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUpgradeButton* m_UpgradeAtkMDmg02;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USkillUpgradeButton* m_UpgradeSkill01;//배우기 제한있음
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USkillUpgradeButton* m_UpgradeSkill02;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USkillUpgradeButton* m_UpgradeSkill03;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USkillUpgradeButton* m_UpgradeSkill04;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USkillUpgradeButton* m_UpgradeSkill05;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_TxtSkillStones;
	//switch 대상
	UPROPERTY()
	UPlayerUpgradeManager* m_PlUpgrade;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USkillHotkeyPanel* m_SkillEquipBtnProxy;


public:
	UFUNCTION()
	void SetPanel1();
	UFUNCTION()
    void SetPanel2();
	UFUNCTION()
	void ClosePanel();
	
public:
	UFUNCTION()
	void UpgradeAtkDmg01();
	UFUNCTION()
    void UpgradeAtkCri01();
	UFUNCTION()
    void UpgradeAtkCDmg01();
	UFUNCTION()
    void UpgradeAtkCri02();
	UFUNCTION()
    void UpgradeAtkCDmg02();
	UFUNCTION()
    void UpgradeAtkMagic01();
	UFUNCTION()
    void UpgradeAtkMDmg01();
	UFUNCTION()
    void UpgradeAtkMagic02();
	UFUNCTION()
    void UpgradeAtkMDmg02();
	//
	UFUNCTION()
    void UpgradeSkill01();
	UFUNCTION()
    void UpgradeSkill02();
	UFUNCTION()
    void UpgradeSkill03();
	UFUNCTION()
    void UpgradeSkill04();
	UFUNCTION()
    void UpgradeSkill05();
	//
	UFUNCTION()
	void OnUpgradeChanged();
	void CloseSkillHotkeyPanel();
	UFUNCTION()
	void OnSkillEquipPressed(USkillUpgradeButton* btn);
	
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	void UpdateSkillStones();
};
