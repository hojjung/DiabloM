#include "ChatWindow.h"


#include "ScrollBoxSlot.h"
#include "Managers/DiabloGameInstance.h"


void UChatWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_SendButton->OnClicked.AddDynamic(this,&UChatWindow::SendText);
	
	UDiabloGameInstance::Get->m_ChatManager->m_OnChatReceive.AddUObject(this,&UChatWindow::OnReceiveTotalChatList);
	//
	for(int i=0; i< 10; i++)
	{
		UChatText* CreatedChat = CreateWidget<UChatText>(this, m_ClassTextWidget);

		auto* SlotUni = m_MessageScrollBox->AddChild(CreatedChat);//20//15

		m_AryChatText.Add(CreatedChat);
		
		CreatedChat->SetVisibility(ESlateVisibility::Collapsed);
	}

	m_nTopIndex=0;
	
	m_MessageScrollBox->ScrollToEnd();
}

UChatText* UChatWindow::GetTopText()
{
	UChatText* Text = m_AryChatText[m_nTopIndex];

	m_nTopIndex++;

	if(m_nTopIndex>=m_AryChatText.Num())
	{
		m_nTopIndex=0;
	}

	Text->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	return Text;
}


void UChatWindow::OnReceiveTotalChatList(const FString& chat)
{
	TArray<FString> OutStrAry;
	
	chat.ParseIntoArray(OutStrAry,TEXT("\n"));

	for(auto& Str : OutStrAry)
	{
		AddTextWidget(Str);	
	}

	
}
void UChatWindow::SendText()
{
	UDiabloGameInstance::Get->m_ChatManager->ChatPost(m_MesageEditableText->GetText());

	m_MesageEditableText->SetText(FText());

	m_MessageScrollBox->ScrollToEnd();
}

void UChatWindow::AddTextWidget(const FString& chat)
{
	UChatText* TopText = GetTopText();
	
	TopText->SetNormalChat(chat);

	m_MessageScrollBox->InsertChildAt(9,TopText);
}
