#include "ChatWindow.h"

#include "Managers/DiabloGameInstance.h"

void UChatWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_SendButton->OnClicked.AddDynamic(this,&UChatWindow::SendText);
	
	UDiabloGameInstance::Get->m_ChatManager->m_OnChatReceive.AddUObject(this,&UChatWindow::OnReceiveTotalChatList);
}

void UChatWindow::OnReceiveTotalChatList(const FString& chat)
{
	PRINTF("Chat:%s",*chat);
}

void UChatWindow::SendText()
{
	UDiabloGameInstance::Get->m_ChatManager->ChatPost(m_MesageEditableText->GetText());

	m_MesageEditableText->SetText(FText());

	UDiabloGameInstance::Get->m_ChatManager->RequestGetChatFromServer();
}
