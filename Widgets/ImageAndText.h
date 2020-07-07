// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Font.h"
#include "Engine/Texture.h"
#include "ImageAndText.generated.h"

/**
 * UDiaInvenGridPanel(const FObjectInitializer& objInit
 */

UCLASS()
class DIABLOM_API UImageAndText : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UImageAndText(const FObjectInitializer& objInit);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "ImageText")
	UHorizontalBox* m_HorizonParent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "ImageText")
	USizeBox* m_SizeBoxWrap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "ImageText")
	UImage* m_ImageIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "ImageText")
	UTextBlock* m_TextStringShow;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImageText")
	UTexture* m_IconWant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImageText")
	UFont* m_FontWant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "ImageText")
	FString m_StringWant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImageText")
	int m_nFontSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImageText")
	float m_fIconSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ImageText")
	FName m_NameTypeFace;

protected:
	virtual void NativePreConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "ImageText")
	void SetString(FText stringWant);
	UFUNCTION(BlueprintCallable, Category = "ImageText")
	void SetStringColor(FLinearColor colorWant);
};
