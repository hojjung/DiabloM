#pragma once

#include "DiabloM.h"
#include  "Widgets/WorldMap/DefaultMenu/ImageAndText.h"
#include "ImageAndTextAndCompare.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UImageAndTextAndCompare : public UImageAndText
{
	GENERATED_BODY()

public:
	UImageAndTextAndCompare(const FObjectInitializer& objInit);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "ImageText")
	USizeBox* m_SizeBoxForCompare;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "ImageText")
	UImage* m_ImageCompareArrow;//show high low value
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "ImageText")
	UTextBlock* m_TextCompareResult;//how much difference

protected:
	UPROPERTY()
	UTexture2D* m_IconUpArrow;
	UPROPERTY()
	UTexture2D* m_IconDownArrow;

protected:
	void SetCompareColor(FLinearColor colorWant);



public:
	UFUNCTION(BlueprintCallable, Category = "Compare")
	void ShowCompare(int preValue, int nxtValue); // show green high red low

	UFUNCTION(BlueprintCallable, Category = "Compare")
	void SetTextDiffValue(float diff);

	UFUNCTION(BlueprintCallable, Category = "Compare")
	void HideCompare();//just hide all arrow and text

};
