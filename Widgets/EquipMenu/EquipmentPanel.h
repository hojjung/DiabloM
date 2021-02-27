// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"


#include "AccessoryEquipButton.h"
#include "Button.h"
#include "Overlay.h"
#include "PetEquipButton.h"
#include "PlayerSkinEquipButton.h"
#include "ScrollBox.h"
#include "VerticalBox.h"
#include "WeaponEquipButton.h"
#include "WidgetSwitcher.h"
#include "WingEquipButton.h"
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
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UPlayerSkinEquipButton> m_ClassPlayerEquipBtn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWeaponEquipButton> m_ClassWeaponEquipBtn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWingEquipButton> m_ClassWingEquipBtn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UPetEquipButton> m_ClassPetEquipBtn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAccessoryEquipButton> m_ClassAccessoryEquipBtn;
	UPROPERTY()
	TArray<UPlayerSkinEquipButton*> m_AryPlSkinBtn;
	UPROPERTY()
	TArray<UWingEquipButton*> m_AryEqWingBtn;
	UPROPERTY()
	TArray<UWeaponEquipButton*> m_AryEqWeaponBtn;
	UPROPERTY()
	TArray<UAccessoryEquipButton*> m_AryEqAccessoryBtn;
	UPROPERTY()
	TArray<UPetEquipButton*> m_AryEqPetBtn;
	
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
	
};
