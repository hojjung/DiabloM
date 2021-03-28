#include "RankingPanel.h"

#include "Managers/DiabloGameInstance.h"


void URankingPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_AryRankElement.Reset();
	
	int i=0;

	while(i<100)
	{
		URankingElement* CreatedBtn = CreateWidget<URankingElement>(this, m_ClassRanking);

		m_Verti->AddChild(CreatedBtn);

		CreatedBtn->SetPadding(FMargin(10, 10, 10, 10));

		m_AryRankElement.Add(CreatedBtn);

		CreatedBtn->SetVisibility(ESlateVisibility::Collapsed);
		
		i++;
	}

	UDiabloGameInstance::Get->m_PlayfabManager->m_OnTotalRankReceived.AddUObject(this,&URankingPanel::UpdateTotalRankingPanel);
	
	UDiabloGameInstance::Get->m_PlayfabManager->m_OnPlayerRankReceived.AddUObject(this,&URankingPanel::UpdateSelfRanking);

	if(UDiabloGameInstance::Get->m_PlayfabManager->GetTotalRank().Num()>0)
	{
		UpdateTotalRankingPanel(UDiabloGameInstance::Get->m_PlayfabManager->GetTotalRank());
	}
	
	if(UDiabloGameInstance::Get->m_PlayfabManager->GetPlayerRank().Num()>0)
	{
		UpdateSelfRanking(UDiabloGameInstance::Get->m_PlayfabManager->GetPlayerRank());
	}
}

void URankingPanel::UpdateTotalRankingPanel(const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& totalRanks)
{
	for(auto* RR : m_AryRankElement)
	{
		RR->SetVisibility(ESlateVisibility::Collapsed);
	}

	int Iter = m_AryRankElement.Num();

	Iter = FMath::Min(Iter,totalRanks.Num());
	
	for(int i=0; i< Iter;i++)
	{
		m_AryRankElement[i]->SetRankElement(totalRanks[i]);
		m_AryRankElement[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void URankingPanel::UpdateSelfRanking(const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& serverRank)
{
	m_SelfRanking->SetRankElement(serverRank[0]);
}