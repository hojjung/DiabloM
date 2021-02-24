// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"


#include "Button.h"
#include "UpgradeButton.h"
#include "WidgetSwitcher.h"
#include "Blueprint/UserWidget.h"
#include "Managers/PlayerUpgradeManager.h"

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
	UUpgradeButton* m_UpgradeSkill01;//배우기 제한있음
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUpgradeButton* m_UpgradeSkill02;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUpgradeButton* m_UpgradeSkill03;
	//switch 대상
	UPROPERTY()
	UPlayerUpgradeManager* m_PlUpgrade;
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
	void OnUpgradeAtkDmg01();
};
