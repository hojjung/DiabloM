// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "CheckBox.h"
#include "Slider.h"
#include "TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundMix.h"

#include "OptionPanel.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UOptionPanel : public UUserWidget
{
	GENERATED_BODY()
public:
	UOptionPanel(const FObjectInitializer& ObjectInitializer);
	void ApplyLoadOption();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCheckBox* m_ToggleFrameLimit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextVersionName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USlider* m_SoundControlBGM;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USlider* m_SoundControlSFX;
	UPROPERTY()
	USoundMix* m_SoundMixBGM;
	UPROPERTY()
	USoundClass* m_SoundClassBGM;
	UPROPERTY()
	USoundMix* m_SoundMixSFX;
	UPROPERTY()
	USoundClass* m_SoundClassSFX;
	
public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnToggleFrameLimit(bool v);

	void UpdateVersionNameText();

	UFUNCTION()
	void OnBGMSliderChanged(float v);

	UFUNCTION()
	void OnSFXSliderChanged(float v);

	virtual void SetVisibility(ESlateVisibility InVisibility) override;
};
