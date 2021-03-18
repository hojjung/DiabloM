#include "ChatText.h"

FString UChatText::m_RichIDRed = "RichText.Red";
FString UChatText::m_RichIDBlue = "RichText.Blue";
FString UChatText::m_RichIDYellow = "RichText.Yellow";
FString UChatText::m_RichIDOrange = "RichText.Orange";
FString UChatText::m_RichIDGreen = "RichText.Green";
FString UChatText::m_RichIDPurple = "RichText.Purple";


void UChatText::SetNormalChat(const FString& chat)
{
	TArray<FString> OutStrAry;
	
	chat.ParseIntoArray(OutStrAry,TEXT(":"));

	FString Ranking = OutStrAry[0];
	
	FString Nickname = OutStrAry[1];
	
	FString ChatText = OutStrAry[2];

	FString FormatStr = FString::Printf(
        TEXT("<%s>%s</>:%s:%s"),*GetRankColor(Ranking),*Ranking,*Nickname,*ChatText);
	
	m_TextChat->SetText(FText::FromString(FormatStr));
}

void UChatText::SetGachaNotify(const FString& chat)
{
	
}

void UChatText::Clear()
{
	m_TextChat->SetText(FText());
}

FString UChatText::GetRankColor(const FString& rankNumber)
{
	int RankNumber = FCString::Atoi(*rankNumber);

	if(RankNumber==1)
	{
		return m_RichIDPurple;
	}
	else if(RankNumber<=2 && RankNumber>=10)
	{
		return m_RichIDRed;
	}
	else if(RankNumber>=100)
	{
		return m_RichIDOrange;
	}
	else if(RankNumber>=1000)
	{
		return m_RichIDYellow;
	}
	else if(RankNumber>=5000)
	{
		return m_RichIDBlue;
	}
	else if(RankNumber>=10000)
	{
		return m_RichIDGreen;
	}

	return "Default";
}
