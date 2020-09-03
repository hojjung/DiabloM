// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"


#include "Components/TextBlock.h"
#include "Components/Image.h"


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
	void SetItemName(const FText& itemName);
	void SetItemColor(const FLinearColor& colorWant);
};
