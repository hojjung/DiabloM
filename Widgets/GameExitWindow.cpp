#include "GameExitWindow.h"
#include "Characters/DiabloPlayerController.h"
#include "Managers/DiabloGameInstance.h"

void UGameExitWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnCancel->OnClicked.AddDynamic(this, &UGameExitWindow::Cancel);
	m_BtnExitGame->OnClicked.AddDynamic(this, &UGameExitWindow::Exit);

	m_bIsOpened = false;
}

void UGameExitWindow::OnBackkeyPressed()
{
	m_bIsOpened = !m_bIsOpened;

	if (m_bIsOpened)
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
		
	}
	UDiabloGameInstance::Get->GetPlCon()->SetPause(m_bIsOpened);
}

void UGameExitWindow::Cancel()
{
	SetVisibility(ESlateVisibility::Collapsed);
	m_bIsOpened = false;
	UDiabloGameInstance::Get->GetPlCon()->SetPause(m_bIsOpened);
}

void UGameExitWindow::Exit()
{
	PRINTF("Exit");

	UKismetSystemLibrary::QuitGame(GetWorld(), UDiabloGameInstance::Get->GetPlCon(), EQuitPreference::Quit, true);
}
