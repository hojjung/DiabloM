// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Button.h"
#include "Blueprint/UserWidget.h"
#include "WeaponDgPanel.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UWeaponDgPanel : public UUserWidget
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnWeaponDg01;

public:
	virtual void NativeOnInitialized() override;
public:
	UFUNCTION()
	void EnterWeaponDg01();
	
};
