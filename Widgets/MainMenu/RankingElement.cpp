#include "RankingElement.h"

#include "Widgets/Chat/ChatText.h"

void URankingElement::SetRankElement(const PlayFab::ClientModels::FPlayerLeaderboardEntry& rankInfo)
{
	int Rank =rankInfo.Position+1;
	
	FString FormatStr = FString::Printf(TEXT("<%s>[%dst]</>:"),*UChatText::GetRankColor(Rank),Rank);
	
	m_TextRankPosition->SetText(FText::FromString(FormatStr));
	m_TextRankDisplayName->SetText(FText::FromString(FString::Printf(TEXT("%s님"),*rankInfo.DisplayName)));
	m_TextStageLevel->SetText(FText::FromString(FString::Printf(TEXT(":%d"),rankInfo.StatValue)));
}
