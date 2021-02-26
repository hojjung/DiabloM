// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Button.h"
#include "EquipmentButton.h"
#include "Overlay.h"
#include "ScrollBox.h"
#include "VerticalBox.h"
#include "WidgetSwitcher.h"
#include "Blueprint/UserWidget.h"
#include "Managers/EquipManager.h"

#include "EquipmentPanel.generated.h"

struct FPlayerClassSpec;
/**
 * 
 */
UCLASS()
class DIABLOM_API UEquipmentPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetSwitcher* m_SwitcherPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClassSkin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnWing;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnAccessory1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnAccessory2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UOverlay* m_OverlaySkin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_VertiSkin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UOverlay* m_OverlayWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_VertiWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UOverlay* m_OverlayWing;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_VertiWing;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UOverlay* m_OverlayPet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_VertiPet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UOverlay* m_OverlayAccessory;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_VertiAccessory;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UEquipmentButton> m_ClassEquipBtn;
	UPROPERTY()
	TArray<UEquipmentButton*> m_AryPlSkinBtn;
	UPROPERTY()
	TArray<UEquipmentButton*> m_AryEqWingBtn;
	UPROPERTY()
	TArray<UEquipmentButton*> m_AryEqWeaponBtn;
	UPROPERTY()
	TArray<UEquipmentButton*> m_AryEqAccessoryBtn;
	UPROPERTY()
	TArray<UEquipmentButton*> m_AryEqPetBtn;
	
protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void ClosePanel();
	UFUNCTION()
    void SetPanelSkin();
	UFUNCTION()
    void SetPanelWing();
	UFUNCTION()
    void SetPanelPet();
	UFUNCTION()
    void SetPanelWeapon();
	UFUNCTION()
    void SetPanelAccessory();
	
	// UFUNCTION()
 //    void UpgradePlSkin();
	// UFUNCTION()
 //    void UpgradeAtkCri01();
	// UFUNCTION()
 //    void UpgradeAtkCDmg01();
	// UFUNCTION()
 //    void UpgradeSkill01();
	// UFUNCTION()
 //    void UpgradeSkill02();
	// UFUNCTION()
 //    void UpgradeSkill03();
	UFUNCTION()
    void OnUpgradeChanged();
	UFUNCTION()
	void OnEquipPlayerSkin();
};
