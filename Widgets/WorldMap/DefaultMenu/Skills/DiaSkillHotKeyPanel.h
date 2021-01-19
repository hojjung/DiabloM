// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"


#include "DiaSkillUseButton.h"
#include "ObjectMacros.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CommonElement/PotionUseBtn.h"

#include "DiaSkillHotKeyPanel.generated.h"

class ASkillIndicator;
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UPotionUseBtn* m_PotionButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_InteractButton;
	UPROPERTY()
	TArray<UDiaSkillUseButton*> m_ArySkillButtons;
	UPROPERTY()
	TArray<UWidget*> m_AryWidgets;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ASkillIndicator> m_ClassIndicator;
	UPROPERTY()
	ASkillIndicator* m_Indicator;

public:
	void DisableAllKeys(UWidget* exceptThis);

	void EnableAllKeys(UWidget* source);

	virtual void NativeDestruct() override;
};
