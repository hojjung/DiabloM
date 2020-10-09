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

	m_PercentFormat = FText::FromString("{0}%");
}

void UImageAndTextAndCompare::SetCompareColor(FLinearColor colorWant)
{
	m_TextCompareResult->SetColorAndOpacity(colorWant);
	m_ImageCompareArrow->SetColorAndOpacity(colorWant);
}

void UImageAndTextAndCompare::ShowCompare(float originalValue, float wantCompare)
{
	m_TextCompareResult->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_ImageCompareArrow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	FSlateBrush BrushWant;

	BrushWant.SetImageSize(FVector2D(64.f, 64.f));

	float Diff = wantCompare - originalValue;

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

	float Percent = originalValue / wantCompare;
	float PerOne = 1 - Percent;
	PerOne *= 100.f;

	SetTextDiffValue(PerOne);
	m_ImageCompareArrow->SetBrush(BrushWant);

	
}

void UImageAndTextAndCompare::SetTextDiffValue(float diff)
{
	diff=UDiaBlueprintFunctionLibrary::SetFloatPrecision(diff,1);
	
	FFormatOrderedArguments Args;
	
	Args.Add(diff);

	m_TextCompareResult->SetText(FText::Format(m_PercentFormat, Args));
}

void UImageAndTextAndCompare::HideCompare()
{
	m_TextCompareResult->SetVisibility(ESlateVisibility::Hidden);
	m_ImageCompareArrow->SetVisibility(ESlateVisibility::Hidden);
}
