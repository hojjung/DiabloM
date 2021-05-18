#include "RankingPanel.h"

#include "Managers/DiabloGameInstance.h"


void URankingPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnBack->OnClicked.AddDynamic(this,&URankingPanel::CloseRankPanels);

	m_BtnOpenNormalRank->OnClicked.AddDynamic(this,&URankingPanel::OpenStageRank);

	m_BtnOpenPVPRank->OnClicked.AddDynamic(this,&URankingPanel::OpenPVPRank);

	SetStageRank();

	SetPVPRank();
}

void URankingPanel::CloseRankPanels()
{
	m_RankNormalPanel->SetVisibility(ESlateVisibility::Hidden);
	
	m_RankPVPPanel->SetVisibility(ESlateVisibility::Hidden);
	
	m_BackBtnParent->SetVisibility(ESlateVisibility::Hidden);

	m_RankOpenVert->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void URankingPanel::OpenStageRank()
{
	m_RankNormalPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_BackBtnParent->SetVisibility(ESlateVisibility::Visible);

	m_RankOpenVert->SetVisibility(ESlateVisibility::Hidden);
}

void URankingPanel::OpenPVPRank()
{
	m_RankPVPPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_BackBtnParent->SetVisibility(ESlateVisibility::Visible);

	m_RankOpenVert->SetVisibility(ESlateVisibility::Hidden);
}

void URankingPanel::SetStageRank()
{
	m_AryStageRankElement.Reset();

	int i = 0;

	while (i < 100)
	{
		URankingElement* CreatedBtn = CreateWidget<URankingElement>(this, m_ClassRanking);

		m_VertiNormal->AddChild(CreatedBtn);

		CreatedBtn->SetPadding(FMargin(10, 10, 10, 10));

		m_AryStageRankElement.Add(CreatedBtn);

		CreatedBtn->SetVisibility(ESlateVisibility::Collapsed);

		i++;
	}

	UDiabloGameInstance::Get->m_PlayfabManager->m_OnTotalStageRankReceived.AddUObject(
		this, &URankingPanel::UpdateTotalStageRankingPanel);

	UDiabloGameInstance::Get->m_PlayfabManager->m_OnPlayerStageRankReceived.AddUObject(
		this, &URankingPanel::UpdateSelfStageRanking);

	if (UDiabloGameInstance::Get->m_PlayfabManager->GetTotalStageRank().Num() > 0) //유저가 1명이상 존재할경우
	{
		UpdateTotalStageRankingPanel(UDiabloGameInstance::Get->m_PlayfabManager->GetTotalStageRank());
	}

	if (UDiabloGameInstance::Get->m_PlayfabManager->GetStagePlayerRank().Num() > 0) //플레이어가 랭킹등록된 경우
	{
		UpdateSelfStageRanking(UDiabloGameInstance::Get->m_PlayfabManager->GetStagePlayerRank());
	}
}

void URankingPanel::UpdateTotalStageRankingPanel(
	const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& totalRanks)
{
	for (auto* RR : m_AryStageRankElement)
	{
		RR->SetVisibility(ESlateVisibility::Collapsed);
	}

	int Iter = m_AryStageRankElement.Num();

	Iter = FMath::Min(Iter, totalRanks.Num());

	for (int i = 0; i < Iter; i++)
	{
		m_AryStageRankElement[i]->SetRankElement(totalRanks[i]);

		m_AryStageRankElement[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void URankingPanel::UpdateSelfStageRanking(const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& serverRank)
{
	m_SelfRanking->SetRankElement(serverRank[0]);
}

void URankingPanel::SetPVPRank()
{
	m_AryPVPStageRankElement.Reset();

	int i = 0;

	while (i < 100)
	{
		URankingElement* CreatedBtn = CreateWidget<URankingElement>(this, m_ClassRanking);

		m_VertiPVP->AddChild(CreatedBtn);

		CreatedBtn->SetPadding(FMargin(10, 10, 10, 10));

		m_AryPVPStageRankElement.Add(CreatedBtn);

		CreatedBtn->SetVisibility(ESlateVisibility::Collapsed);

		i++;
	}

	UDiabloGameInstance::Get->m_PlayfabManager->m_OnTotalPVPRankReceived.AddUObject(
		this, &URankingPanel::UpdatePVPStageRankingPanel);

	UDiabloGameInstance::Get->m_PlayfabManager->m_OnPlayerPVPRankReceived.AddUObject(
		this, &URankingPanel::UpdateSelfPVPRanking);

	if (UDiabloGameInstance::Get->m_PlayfabManager->GetTotalPVPRank().Num() > 0) //유저가 1명이상 존재할경우
	{
		UpdatePVPStageRankingPanel(UDiabloGameInstance::Get->m_PlayfabManager->GetTotalPVPRank());
	}

	if (UDiabloGameInstance::Get->m_PlayfabManager->GetPVPPlayerRank().Num() > 0) //플레이어가 랭킹등록된 경우
	{
		UpdateSelfPVPRanking(UDiabloGameInstance::Get->m_PlayfabManager->GetPVPPlayerRank());
	}
}

void URankingPanel::UpdatePVPStageRankingPanel(const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& totalRanks)
{
	for (auto* RR : m_AryPVPStageRankElement)
	{
		RR->SetVisibility(ESlateVisibility::Collapsed);
	}

	int Iter = m_AryPVPStageRankElement.Num();

	Iter = FMath::Min(Iter, totalRanks.Num());

	for (int i = 0; i < Iter; i++)
	{
		m_AryPVPStageRankElement[i]->SetRankElement(totalRanks[i]);

		m_AryPVPStageRankElement[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void URankingPanel::UpdateSelfPVPRanking(const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& serverRank)
{
	m_PVPSelfRanking->SetRankElement(serverRank[0]);
}
