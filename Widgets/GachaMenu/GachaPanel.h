#pragma once
#include "CoreMinimal.h"
#include "Button.h"
#include "GachaCardGridPanel.h"
#include "GachaInfoPanel.h"
#include "Overlay.h"
#include "ScrollBox.h"
#include "TextBlock.h"
#include "WidgetSwitcher.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CommonElement/MaterialProgressBar.h"

#include "GachaPanel.generated.h"

class UGachaManager;
UCLASS()
class DIABLOM_API UGachaPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UGachaCardGridPanel* m_GachaGridPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UGachaInfoPanel* m_GachaInfoPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_VertiGacha;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnViewAds;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextViewCounts;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextViewCooldown;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextGachaWeaponNameLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_BarGachaLevelExp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnShowWeaponLevelInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnShowWeaponGachaRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRollGachaWeaponOneTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRollGachaWeaponElevenTime;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextGachaSkinNameLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_BarGachaLevelExpSkin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnShowSkinLevelInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnShowSkinGachaRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRollGachaSkinOneTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRollGachaSkinElevenTime;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextGachaPetName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnShowPetGachaRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRollGachaPetOneTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRollGachaPetElevenTime;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextGachaWingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnShowWingGachaRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRollGachaWingOneTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRollGachaWingElevenTime;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextGachaAccesoryName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnShowAccesoryGachaRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRollGachaAccesoryOneTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRollGachaAccesoryElevenTime;
	//
	TWeakObjectPtr<UGachaManager> m_GachaManager; 
protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void ClosePanel();
	//
	UFUNCTION()
	void ShowWeaponLevelInfo();
	UFUNCTION()
    void ShowWeaponGachaInfo();
	UFUNCTION()
	void RollGachaWeaponOneTime();
	UFUNCTION()
    void RollGachaWeaponElevenTimes();
	//
	UFUNCTION()
    void ShowSkinLevelInfo();
	UFUNCTION()
    void ShowSkinGachaInfo();
	UFUNCTION()
    void RollGachaSkinOneTime();
	UFUNCTION()
    void RollGachaSkinElevenTimes();
	//
   	UFUNCTION()
    void ShowPetGachaInfo();
   	UFUNCTION()
    void RollGachaPetOneTime();
   	UFUNCTION()
    void RollGachaPetElevenTimes();
	//
	UFUNCTION()
    void ShowWingGachaInfo();
	UFUNCTION()
	void RollGachaWingOneTime();
	UFUNCTION()
	void RollGachaWingElevenTimes();
	//
	UFUNCTION()
    void ShowAccessoryGachaInfo();
	UFUNCTION()
    void RollGachaAccessoryOneTime();
	UFUNCTION()
    void RollGachaAccessoryElevenTimes();
	//
};
