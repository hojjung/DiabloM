// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MinimapWidget.h"
#include "Blueprint/UserWidget.h"
#include "Characters/DiabloPlayerController.h"
#include "Widgets/WorldMap/DefaultMenu/Skills/DiaSkillHotKeyPanel.h"

#include "PlayerStatusBar.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPlayerStatusBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerStatusBar(const FObjectInitializer& objInit);
	void EquipFromSaveData();
	void StopAutoPlay();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_InvenOpenButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextHp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_HpBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_StaminaBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_ManaBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_RageBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMinimapWidget* m_Minimap;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCheckBox* m_AutoPlayButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_SkillMenuOpenButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaSkillHotKeyPanel* m_SkillUseCanvas;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaMonsterInfo* m_DiaMonInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_ExpBar;
	
	TWeakObjectPtr<UPlayerDiabloAbilitySystemComp> m_PlayerComp;

	TWeakObjectPtr<UMaterialProgressBar> m_SelectedBar;

	const FGameplayAttributeData* m_SelectedCurAttribute;
	
	const FGameplayAttributeData* m_SelectedMaxAttribute;
	
	FTextFormat m_HpFormat;
public:
	void Init(ADiabloPlayerController* diaCon,UMainCanvas* mainCanvas);

	void SetHealthBarProgressV(AUnitPawn* pawn);

	void SetResourceBarProgressV(AUnitPawn* pawn);
	
	void UpdateMinimap(UMaterialInterface* material_interface);
	
	void HideMinimap();
	
	void ShowMinimap();
	
	void ShowPlayerHUD();
	
	void HidePlayerHUD();

	void UpdateSkill(FSkillDataSpec* spec);

	void UpdateSkillBtn(FSkillDataSpec* spec,int index);

	UFUNCTION()
	void SetAutoPlay(bool isAuto);
	
	friend UMainCanvas;
};
