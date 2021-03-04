// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaSkillUseButton* m_BtnSkillUse1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaSkillUseButton* m_BtnSkillUse2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaSkillUseButton* m_BtnSkillUse3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaSkillUseButton* m_BtnSkillUse4;
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* m_FlashAnim;
	UPROPERTY()
	TArray<UDiaSkillUseButton*> m_ArySkillUse;
	
public:
	virtual void NativeOnInitialized() override;
	
	void PlayBlink();

	void OnUpdateSkill(int index,FSkillSpec* skill_spec);
};
