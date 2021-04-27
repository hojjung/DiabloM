#include "PVPPanel.h"
#include "Managers/DiabloGameInstance.h"

void UPVPPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	UDiabloGameInstance::Get->m_PVPManager->m_OnOtherPlayerFound.AddUObject(this,&UPVPPanel::OnNicknameFound);

	UDiabloGameInstance::Get->m_PVPManager->m_OnMatchFail.AddUObject(this,&UPVPPanel::OnMatchFail);
	
	m_BtnMatching->OnClicked.AddDynamic(this,&UPVPPanel::RequestMatch);

	m_TextPlayerID->SetText(FText::FromString(UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname));
	//
	
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
