#include "ImageAndTextAndCompare.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"



UImageAndTextAndCompare::UImageAndTextAndCompare(const FObjectInitializer& objInit) :Super(objInit)
{
	//white arrow
	static ConstructorHelpers::FObjectFinder<UTexture2D> FoundTexture1(
		TEXT("Texture2D'/Game/Sprite/UI/fg4_iconsFlat_upArrow.fg4_iconsFlat_upArrow'"));

	m_IconUpArrow = FoundTexture1.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> FoundTexture2(
		TEXT("Texture2D'/Game/Sprite/UI/fg4_iconsFlat_downArrow.fg4_iconsFlat_downArrow'"));

	m_IconDownArrow = FoundTexture2.Object;


}

void UImageAndTextAndCompare::SetCompareColor(FLinearColor colorWant)
{
	m_TextCompareResult->SetColorAndOpacity(colorWant);
	m_ImageCompareArrow->SetColorAndOpacity(colorWant);
}

void UImageAndTextAndCompare::ShowCompare(int preValue, int nxtValue)
{
	FSlateBrush BrushWant;

	BrushWant.SetImageSize(FVector2D(64.f, 64.f));

	int Diff = nxtValue - preValue;

	if (Diff>0)
	{
		BrushWant.SetResourceObject(m_IconUpArrow);
		SetCompareColor(FLinearColor::Green);
		
	}
	else if(Diff==0)
	{
		HideCompare();
	}
	else if(Diff<0)
	{
		BrushWant.SetResourceObject(m_IconDownArrow);
		SetCompareColor(FLinearColor::Red);
	}

	float Percent = (float)preValue / (float)nxtValue;
	float PerOne = 1 - Percent;
	PerOne *= 100.f;

	SetTextDiffValue(PerOne);
	m_ImageCompareArrow->SetBrush(BrushWant);
}

void UImageAndTextAndCompare::SetTextDiffValue(float diff)
{
	FText NumbetT = UKismetTextLibrary::Conv_FloatToText(diff, ERoundingMode::FromZero, false, true, 1);

	FFormatOrderedArguments Args;
	Args.Add(NumbetT);

	FTextFormat PercentFormat = FText::FromString("{0}%");

	m_TextCompareResult->SetText(FText::Format(PercentFormat, Args));
}

void UImageAndTextAndCompare::HideCompare()
{
	m_TextCompareResult->SetVisibility(ESlateVisibility::Hidden);
	m_ImageCompareArrow->SetVisibility(ESlateVisibility::Hidden);
}
