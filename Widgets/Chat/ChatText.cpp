#include "ChatText.h"

#include "Managers/DiabloGameInstance.h"

FString UChatText::m_RichIDRed = "RichText.Red";
FString UChatText::m_RichIDBlue = "RichText.Blue";
FString UChatText::m_RichIDYellow = "RichText.Yellow";
FString UChatText::m_RichIDOrange = "RichText.Orange";
FString UChatText::m_RichIDGreen = "RichText.Green";
FString UChatText::m_RichIDPurple = "RichText.Purple";

void UChatText::SetChat(const FString& chat)
{
	m_TextChat->SetDefaultColorAndOpacity(FLinearColor::White);
	FString CachedChat = chat;

	//CachedChat = CachedChat.TrimStart();
	CachedChat.RemoveFromEnd(TEXT("\r"));
	
	TArray<FString> OutStrAry;
	
	CachedChat.ParseIntoArray(OutStrAry,TEXT(":"));

	if(OutStrAry.Num()>2)
	{
		//normalchat
		FString Ranking = OutStrAry[0];
	
		FString Nickname = OutStrAry[1];
	
		FString ChatText = OutStrAry[2];

		FString FormatStr = FString::Printf(
            TEXT("<%s>[%sst]</>:%s:%s"),*GetRankColor(Ranking),*Ranking,*Nickname,*ChatText);
	
		m_TextChat->SetText(FText::FromString(FormatStr));
	}
	else
	{
		m_TextChat->SetText(FText::FromString(CachedChat));
		
		m_TextChat->SetDefaultColorAndOpacity(FLinearColor::Yellow);
	}
}

void UChatText::SetChatForClient(const FString& chat)
{
	m_TextChat->SetDefaultColorAndOpacity(FLinearColor::White);
	
	int Ranking = UDiabloGameInstance::Get->m_PlayfabManager->GetSafeRanking();
	
	FString Nickname = UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname;
	
	FString FormatStr = FString::Printf(
        TEXT("<%s>[%d st]</>: %s : %s"),*GetRankColor(Ranking),Ranking,*Nickname,*chat);
	
	m_TextChat->SetText(FText::FromString(FormatStr));
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
	else if(RankNumber<=2 || RankNumber>=10)
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

FString UChatText::GetRankColor(int RankNumber)
{
	if(RankNumber==1)
	{
		return m_RichIDPurple;
	}
	else if(RankNumber<=2|| RankNumber>=10)
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
