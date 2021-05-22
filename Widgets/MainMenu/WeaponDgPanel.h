// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Button.h"
#include "ScrollBox.h"
#include "WeaponDgBtn.h"
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWeaponDgBtn> m_ClassWeaponDgBtn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_VertiList;

public:
	virtual void NativeOnInitialized() override;
public:
	UFUNCTION()
	void EnterWeaponDg01();
	
};
