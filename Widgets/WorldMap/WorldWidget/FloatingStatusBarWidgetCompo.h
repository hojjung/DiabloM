// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FloatingStatusBarWidgetCompo.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UFloatingStatusBarWidgetCompo : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetHealthPercentage(float HealthPercentage);

};
