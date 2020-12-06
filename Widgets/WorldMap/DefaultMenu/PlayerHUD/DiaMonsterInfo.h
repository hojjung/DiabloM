// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "DiaMonsterInfo.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UDiaMonsterInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetHealthPercentage(float HealthPercentage);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealthFormat(const FText& healthFormatted);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetCharacterName(const FText& NewName);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCharacterLevel(int lv);
};
