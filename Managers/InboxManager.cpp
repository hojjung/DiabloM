// My First Hack n Slash


#include "InboxManager.h"

#include "DiabloGameInstance.h"

void UInboxManager::SetInboxManager(TArray<FInboxSpec>& inboxSpec)
{
	m_AryInbox.Reset();
	
	m_AryInbox=inboxSpec;
	
	m_OnInboxUpdated.Broadcast(m_AryInbox);
}

void UInboxManager::RefreshInbox()
{
	UDiabloGameInstance::Get->m_PlayfabManager->RequestInboxList();
}
