#include "MainMenuPanel.h"


#include "InboxPanel.h"
#include "Managers/DiabloGameInstance.h"


void UMainMenuPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//
	m_BtnPlayerInfo->OnClicked.AddDynamic(this, &UMainMenuPanel::OpenPlayerInfoPanel);
	m_BtnMapSelect->OnClicked.AddDynamic(this, &UMainMenuPanel::OpenMapSelectPanel);
	m_BtnRanking->OnClicked.AddDynamic(this, &UMainMenuPanel::OpenRankingPanel);
	m_BtnMail->OnClicked.AddDynamic(this, &UMainMenuPanel::OpenMailPanel);
	m_BtnDaily->OnClicked.AddDynamic(this, &UMainMenuPanel::OpenDailyPanel);
	m_BtnOption->OnClicked.AddDynamic(this, &UMainMenuPanel::OpenOptionPanel);
	//
	m_PanelPlayerInfo->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelStageSelect->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelRanking->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelMail->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelDaily->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelOption->SetVisibility(ESlateVisibility::Collapsed);
	//
	m_BtnClose->OnClicked.AddDynamic(this, &UMainMenuPanel::ClosePanel);
}

void UMainMenuPanel::ClosePanel()
{
	SetVisibility(ESlateVisibility::Collapsed);

	UDiabloGameInstance::Get->m_AdverManager->ShowBannerAD(true);
}

void UMainMenuPanel::OpenPlayerInfoPanel()
{
	if (m_PanelPlayerInfo->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelPlayerInfo->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		m_PanelPlayerInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	m_PanelStageSelect->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelRanking->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelMail->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelDaily->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelOption->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainMenuPanel::OpenMapSelectPanel()
{
	if (m_PanelStageSelect->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelStageSelect->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		m_PanelStageSelect->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	m_PanelPlayerInfo->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelRanking->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelMail->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelDaily->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelOption->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainMenuPanel::OpenRankingPanel()
{
	if (m_PanelRanking->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelRanking->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		m_PanelRanking->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}


	m_PanelPlayerInfo->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelStageSelect->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelMail->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelDaily->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelOption->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainMenuPanel::OpenMailPanel()
{
	if (m_PanelMail->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelMail->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		m_PanelMail->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	m_PanelPlayerInfo->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelStageSelect->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelRanking->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelDaily->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelOption->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainMenuPanel::OpenDailyPanel()
{
	if (m_PanelDaily->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelDaily->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		m_PanelDaily->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	m_PanelPlayerInfo->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelStageSelect->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelRanking->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelMail->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelOption->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainMenuPanel::OpenOptionPanel()
{
	if (m_PanelOption->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelOption->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		m_PanelOption->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	m_PanelPlayerInfo->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelStageSelect->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelRanking->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelMail->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelDaily->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainMenuPanel::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if (InVisibility == ESlateVisibility::Collapsed || InVisibility == ESlateVisibility::Hidden)
	{
		m_PanelPlayerInfo->SetVisibility(InVisibility);
		m_PanelStageSelect->SetVisibility(InVisibility);
		m_PanelRanking->SetVisibility(InVisibility);
		m_PanelMail->SetVisibility(InVisibility);
		m_PanelDaily->SetVisibility(InVisibility);
		m_PanelOption->SetVisibility(InVisibility);

		
	}
}

FReply UMainMenuPanel::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	return FReply::Handled();
}

FReply UMainMenuPanel::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	return FReply::Handled();
}

FReply UMainMenuPanel::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	return FReply::Handled();
}
