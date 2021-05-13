// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/WidgetComponent.h"
#include "DamageTextWidgetComponent.generated.h"

/**
 * 
 */

UENUM(Blueprintable)
enum class EDamagePopup: uint8
{
    NormalRight,
    CritcalRight,
	CritcalRight2,
    Miss,
	ObtainWeaponStone,
    Length
};
UCLASS()
class DIABLOM_API UDamageTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UDamageTextWidgetComponent();
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetDamageText(const FText& Damage);
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void StartAnimation(EDamagePopup dmg);
    UFUNCTION(BlueprintCallable)
    void EndAnimation();

};
