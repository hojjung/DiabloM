#include "PVPPanel.h"
#include "Managers/DiabloGameInstance.h"

void UPVPPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	UDiabloGameInstance::Get->m_PVPManager->m_OnOtherPlayerFound.BindUObject(this,&UPVPPanel::OnNicknameFound);

	UDiabloGameInstance::Get->m_PVPManager->m_OnMatchFail.BindUObject(this,&UPVPPanel::OnMatchFail);
	
	m_BtnMatching->OnClicked.AddDynamic(this,&UPVPPanel::RequestMatch);

	m_TextPlayerID->SetText(FText::FromString(UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname));
	//
	UpdateStatusText();
}

void UPVPPanel::RequestMatch()
{
	m_BtnMatching->SetIsEnabled(false);
	
	UDiabloGameInstance::Get->m_PVPManager->RequestPVPMatching();	
}

void UPVPPanel::OnNicknameFound(const FString otherPlayerName)
{
	m_TextOtherPlayerID->SetText(FText::FromString(otherPlayerName));
}

void UPVPPanel::OnMatchFail()
{
	//m_BtnMatching->SetIsEnabled(true);
	UDiabloGameInstance::Get->m_PVPManager->MatchFail();
}

void UPVPPanel::UpdateStatusText()
{
	int MMR = UDiabloGameInstance::Get->m_PVPManager->GetMMR();

	FText MMRTxt =FText::FromString(FString::Printf(TEXT("MMR:%d"),MMR));
	
	m_TextMMR->SetText(MMRTxt);
	
	int Win = UDiabloGameInstance::Get->m_PVPManager->GetWin();
	int Lose = UDiabloGameInstance::Get->m_PVPManager->GetLose();
	
	FText WinLose =FText::FromString(FString::Printf(TEXT("%d승 %d패"),Win,Lose));
	
	m_TextWinLose->SetText(WinLose);
}
