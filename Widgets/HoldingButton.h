// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "HoldingButton.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UHoldingButton : public UButton
{
	GENERATED_BODY()

public:
    UHoldingButton();
protected:
    float m_fDeltaTimer;

public:
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    float m_fHoldingTime;

    FDelegateHandle m_TickDeleHandle;
    
    UPROPERTY(BlueprintAssignable, Category="Button|Event")
    FOnButtonClickedEvent OnHoldTimeDone;

    UFUNCTION(BlueprintCallable)
    void UseHoldEvent(float holdTime);

    UFUNCTION()
    void OnPressHold();

    UFUNCTION()
    void OnReleaseHold();

    void CustomTick(float deltatime);

};
