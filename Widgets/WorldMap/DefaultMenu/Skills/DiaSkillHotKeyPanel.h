// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"


#include "DiaSkillUseButton.h"
#include "ObjectMacros.h"
#include "Blueprint/UserWidget.h"
#include "DiaSkillHotKeyPanel.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UDiaSkillHotKeyPanel : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaSkillUseButton* m_SkillBtn1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaSkillUseButton* m_SkillBtn2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaSkillUseButton* m_SkillBtn3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaSkillUseButton* m_SkillBtn4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaSkillUseButton* m_SkillBtn5;
	UPROPERTY()
	TArray<UDiaSkillUseButton*> m_AryButtons;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_InteractButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_PotionButton;
};
