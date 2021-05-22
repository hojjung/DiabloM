// My First Hack n Slash


#include "InboxElement.h"

void UInboxElement::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnClaim->OnClicked.AddDynamic(this,&UInboxElement::OnClick);
}

void UInboxElement::SetInboxSpec(const FInboxSpec& element)
{
	m_TextMailName->SetText(FText::FromString(element.MailName));
	m_TextMailDesc->SetText(FText::FromString(element.MailDesc));

	m_ImgItemIcon->SetBrushFromTexture(element.m_Data->m_RewardIcon);
	m_TextItemName->SetText(element.m_Data->m_TextRewardName);
	
	m_TextExpireTime->SetText(FText::FromString(element.m_StrRemainTime));
	m_TextItemAmount->SetText(FText::FromString(element.ItemAmount));
}

void UInboxElement::OnClick()
{
	m_OnClaim.Broadcast(m_nIndex);
}
