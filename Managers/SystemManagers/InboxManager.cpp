#include "InboxManager.h"

#include "Managers/DiabloGameInstance.h"

void UInboxManager::ClaimItem(FString itemID, int amount)
{
	if (itemID == TEXT("GG"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(amount);
	}
	else
	{
		UDiabloGameInstance::Get->m_ShopManager->OnPurchasedGainItem(itemID,true);
	}
}

void UInboxManager::SetInboxManager(TArray<FInboxSpec>& inboxSpec)
{
	m_AryInbox.Reset();

	for (auto& Inbox : inboxSpec)
	{
		if (!Inbox.m_bIsExpired)
		{
			m_AryInbox.Add(Inbox);
		}
	}

	m_OnInboxUpdated.Broadcast(m_AryInbox);
}

void UInboxManager::ClaimInbox(int index)
{
	if (index < 0 || index >= m_AryInbox.Num() || m_AryInbox[index].m_bIsExpired)
	{
		return;
	}

	m_AryInbox[index].m_bIsExpired = true;

	ClaimItem(m_AryInbox[index].ItemID, FCString::Atoi(*m_AryInbox[index].ItemAmount));

	m_AryInbox.RemoveAt(index);
	m_AryInbox.Shrink();

	//우편도 다받았다고 업데이트
	//우편에서 아이템을 받은것도 적용해서 업데이트 필요
	UDiabloGameInstance::Get->RequestPopupText(TEXT("우편 적용중"));
	UDiabloGameInstance::Get->GetHud()->ShowTouchBan(3);
	UDiabloGameInstance::Get->m_PlayfabManager->RequestClaimInbox(index);
}

void UInboxManager::ClaimAllItems()
{
	if(m_AryInbox.Num()<1)
	{
		return;
	}
	
	int GemStones=0;
	
	for(FInboxSpec& InboxSpec  : m_AryInbox)
	{
		if( InboxSpec.ItemID==TEXT("GG"))
		{
			GemStones+=FCString::Atoi(*InboxSpec.ItemAmount);
		}
		else
		{
			UDiabloGameInstance::Get->m_ShopManager->OnPurchasedGainItem(InboxSpec.ItemID,false);
		}
	}
	
	if(GemStones>0)
	{
		UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(GemStones);
	}

	UDiabloGameInstance::Get->m_PlayfabManager->UploadMainData();
}

void UInboxManager::ClaimAllInbox()
{
	if (m_AryInbox.Num() < 1)
	{
		return;
	}

	ClaimAllItems();
	
	UDiabloGameInstance::Get->RequestPopupText(TEXT("우편 적용중"));
	UDiabloGameInstance::Get->m_PlayfabManager->RequestClaimAllInbox();
	UDiabloGameInstance::Get->GetHud()->ShowTouchBan(3);
}
