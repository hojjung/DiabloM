// My First Hack n Slash


#include "DailyRewardElement.h"

void UDailyRewardElement::SetDailyRewardElement(int dday, int gemstoneAmount)
{
	m_bIsAvailable =false;
	
	m_TextDay->SetText(FText::AsNumber(dday));

	m_nGemStones = gemstoneAmount;
	
	m_TextGemStones->SetText(FText::AsNumber(m_nGemStones));

	m_BtnClaimReward->OnClicked.AddDynamic(this,&UDailyRewardElement::Claimed);
}

void UDailyRewardElement::Claimed()
{
	if(!m_bIsAvailable)
	{
		return;
	}
	m_bIsAvailable=false;
	m_BtnClaimReward->SetIsEnabled(false);
	FString Str= FString::Printf(TEXT("획득!:%d"),m_nGemStones);
	m_TextGemStones->SetText(FText::FromString(Str));
}

void UDailyRewardElement::SetClaimAble()
{
	m_bIsAvailable=true;
	m_ImgFocused->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	FString Str= FString::Printf(TEXT("획득가능:%d"),m_nGemStones);
	m_TextGemStones->SetText(FText::FromString(Str));
}
