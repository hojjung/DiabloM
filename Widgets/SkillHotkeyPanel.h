// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "CheckBox.h"
#include "Blueprint/UserWidget.h"
#include "CommonElement/DiaSkillUseButton.h"

#include "SkillHotkeyPanel.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API USkillHotkeyPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UDiaSkillUseButton* m_BtnSkillUse1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UDiaSkillUseButton* m_BtnSkillUse2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UDiaSkillUseButton* m_BtnSkillUse3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UDiaSkillUseButton* m_BtnSkillUse4;
	UPROPERTY(Transient,meta = (BindWidgetAnim,AllowPrivateAccess = "true"))
	UWidgetAnimation* m_FlashAnim;
	UPROPERTY()
	TArray<UDiaSkillUseButton*> m_ArySkillUse;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UCheckBox* m_BtnAutoUseSkill;
	
public:
	virtual void NativeOnInitialized() override;
	
	void PlayBlink();

	void OnUpdateSkill(int index,FSkillSpec* skill_spec);

	void OnCooldownStart(int index,FSkillSpec* skill_spec);

	UFUNCTION()
	void ToggleUseAutoSkill(bool isChecked);
};


