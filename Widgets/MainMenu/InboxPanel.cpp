// My First Hack n Slash


#include "InboxPanel.h"
#include "ScrollBox.h"
#include "Managers/DiabloGameInstance.h"

void UInboxPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CreateInboxElement(30);

	SetInboxElement(UDiabloGameInstance::Get->m_InboxManager->GetAryInbox());
	
	UDiabloGameInstance::Get->m_InboxManager->m_OnInboxUpdated.AddUObject(this,&UInboxPanel::SetInboxElement);

	m_BtnClaimAll->OnClicked.AddDynamic(this,&UInboxPanel::ClaimAll);
}

void UInboxPanel::CreateInboxElement(int count)
{
	m_AryInboxElement.Reset();
	
	int i=0;

	while(i<count)
	{
		UInboxElement* CreatedInbox = CreateWidget<UInboxElement>(this, m_ClassInboxElement);

		CreatedInbox->m_nIndex = i;

		m_VerticalInboxList->AddChild(CreatedInbox);

		CreatedInbox->SetPadding(FMargin(10, 10, 10, 10));

		m_AryInboxElement.Add(CreatedInbox);

		CreatedInbox->SetVisibility(ESlateVisibility::Collapsed);

		CreatedInbox->m_OnClaim.AddUObject(this,&UInboxPanel::ClaimInbox);

		i++;
	}
}

void UInboxPanel::SetInboxElement(const TArray<FInboxSpec>& aryInboxSpecs)
{
	for(auto* Element : m_AryInboxElement)
	{
		Element->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(aryInboxSpecs.Num()<1)
	{
		m_TextMailCount->SetText(FText::AsNumber(0));
		m_TextLoading->SetText(FText::FromString(TEXT("우편 없음")));
		m_TextLoading->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		return;
	}

	m_TextLoading->SetVisibility(ESlateVisibility::Collapsed);
	
	int i=0;

	int MaxIter = m_AryInboxElement.Num();
	
	MaxIter = FMath::Min(MaxIter,aryInboxSpecs.Num());

	while(i<MaxIter)
	{
		m_AryInboxElement[i]->SetInboxSpec(aryInboxSpecs[i]);
		m_AryInboxElement[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		i++;
	}

	m_TextMailCount->SetText(FText::AsNumber(i));
}

void UInboxPanel::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if(InVisibility==ESlateVisibility::SelfHitTestInvisible || InVisibility==ESlateVisibility::Visible)
	{
		m_TextLoading->SetText(FText::FromString(TEXT("우편 로딩중")));
		m_TextLoading->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UDiabloGameInstance::Get->m_PlayfabManager->RequestInboxList();	
	}
}

void UInboxPanel::ClaimAll()
{
	PRINTF("ClaimALL - InboxPanel");
	UDiabloGameInstance::Get->m_InboxManager->ClaimAll();
}

void UInboxPanel::ClaimInbox(int index)
{
	UDiabloGameInstance::Get->m_InboxManager->ClaimInbox(index);
}

