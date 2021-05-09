// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Button.h"
#include "Blueprint/UserWidget.h"
#include "PetDgPanel.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPetDgPanel : public UUserWidget
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPetDg01;

	public:
	virtual void NativeOnInitialized() override;
	public:
	UFUNCTION()
	void EnterPetDg01();
	
};
