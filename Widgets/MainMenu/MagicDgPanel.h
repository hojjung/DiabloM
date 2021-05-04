// My First Hack n Slash

#pragma once

#include "DiabloM.h"
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnMagicDg01;

public:
	virtual void NativeOnInitialized() override;
public:
	UFUNCTION()
	void EnterMagicDg01();
	
};
