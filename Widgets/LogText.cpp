#include "LogText.h"

ULogText* ULogText::Get = nullptr;

void ULogText::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Get = this;
	
	m_AryText.Reset();
	
	int i=0;

	while (i++<m_nMaxCount)
	{
		//UTextBlock* CreatedText =CreateWidget<UTextBlock>(this,UTextBlock::StaticClass());

		//m_VerticalHolder->AddChildToVerticalBox(CreatedText);

		// FSlateFontInfo ff;
		//
		// ff.Size=64;
		// ff.OutlineSettings.OutlineSize=2;
		//
		// CreatedText->SetFont(ff);
		//
		// CreatedText->SetVisibility(ESlateVisibility::Collapsed);
		//
		// m_AryText.Add(CreatedText);
	}
}

void ULogText::PrintText(FString str)
{
	m_AryText[m_nCurrentCount]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_AryText[m_nCurrentCount]->SetText(FText::FromString(str));
	
	m_nCurrentCount++;

	if(m_nCurrentCount>=m_nMaxCount)
	{
		m_nCurrentCount=0;
	}
}

void ULogText::NativeDestruct()
{
	Super::NativeDestruct();

	ULogText::Get= nullptr;
}
