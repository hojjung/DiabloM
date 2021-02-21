// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FloatingStatusBarWidgetCompo.generated.h"

class UMaterialProgressBar;
/**
 * 
 */
UCLASS()
class DIABLOM_API UFloatingStatusBarWidgetCompo : public UWidgetComponent
{
	GENERATED_BODY()

	
public:
	virtual void BeginPlay() override;
	
    void SetHealthPercentage(float HealthPercentage);

};
