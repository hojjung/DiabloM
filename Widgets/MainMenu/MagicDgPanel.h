// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "MagicDgBtn.h"
#include "ScrollBox.h"
#include "Blueprint/UserWidget.h"
#include "MagicDgPanel.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UMagicDgPanel : public UUserWidget
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UMagicDgBtn> m_ClassMagicDgBtn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_VertiList;
public:
	virtual void NativeOnInitialized() override;
public:
	UFUNCTION()
	void EnterMagicDg01();
	
};
