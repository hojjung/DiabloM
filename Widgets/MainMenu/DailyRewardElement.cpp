// My First Hack n Slash


#include "DailyRewardElement.h"

void UDailyRewardElement::SetDailyRewardElement(int dday, int gemstoneAmount)
{
	m_TextDay->SetText(FText::AsNumber(dday));

	m_nGemStones = gemstoneAmount;
	
	m_TextGemStones->SetText(FText::AsNumber(m_nGemStones));
}

void UDailyRewardElement::Claimed()
{
	FString Str= FString::Printf(TEXT("획득!:%d"),m_nGemStones);
	m_TextGemStones->SetText(FText::FromString(Str));
}
