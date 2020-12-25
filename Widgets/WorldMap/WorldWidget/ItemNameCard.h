// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "ItemNameCard.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UItemNameCard : public UUserWidget
{
	GENERATED_BODY()
	protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* m_ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* m_ImgBackground;

public:
	UFUNCTION(BlueprintCallable)
	void SetItemName(const FText& itemName);
	UFUNCTION(BlueprintCallable)
	void SetItemColor(const FLinearColor& colorWant);
};
