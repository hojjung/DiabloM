#include "RankingElement.h"

void URankingElement::SetRankElement(const PlayFab::ClientModels::FPlayerLeaderboardEntry& rankInfo)
{
	m_TextRankPosition->SetText(FText::FromString(FString::Printf(TEXT("%dst"),rankInfo.Position+1)));
	m_TextRankDisplayName->SetText(FText::FromString(FString::Printf(TEXT("%s님"),*rankInfo.DisplayName)));
	m_TextStageLevel->SetText(FText::FromString(FString::Printf(TEXT("던전:%d"),rankInfo.StatValue)));
}
