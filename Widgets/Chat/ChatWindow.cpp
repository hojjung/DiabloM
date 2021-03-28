#include "ChatWindow.h"
#include "ChatText.h"
#include "ScrollBox.h"

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

		UScrollBoxSlot* SlotUni = Cast<UScrollBoxSlot>( m_MessageScrollBox->AddChild(CreatedChat));//20//15

		SlotUni->SetPadding(FMargin(0,15.f,0,0));

		m_AryChatText.Add(CreatedChat);
		
		//CreatedChat->SetVisibility(ESlateVisibility::Collapsed);
	}


	m_nTopIndex=0;

	m_MesageEditableText->OnTextChanged.AddDynamic(this,&UChatWindow::OnChatTextChanged);

	m_bIsOpened=true;

	SetChatWindowOpenClose();

	m_ChatWindowToggle->OnClicked.AddDynamic(this,&UChatWindow::SetChatWindowOpenClose);

}

void UChatWindow::ClearChat()
{
	for(UChatText* TextChat : m_AryChatText)
	{
		TextChat->Clear();
	}
}

UChatText* UChatWindow::GetTopText()
{
	UChatText* Text = m_AryChatText[m_nTopIndex];

	m_nTopIndex++;

	if(m_nTopIndex>=m_AryChatText.Num())
	{
		m_nTopIndex=0;
	}

	//Text->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	return Text;
}


void UChatWindow::OnReceiveTotalChatList(const FString& chat)
{
	ClearChat();
	
	TArray<FString> OutStrAry;
	
	chat.ParseIntoArray(OutStrAry,TEXT("\n"));

	for(FString& Str : OutStrAry)
	{
		AddTextWidget(Str);
	}
}
void UChatWindow::SendText()
{
	FText ChatText = m_MesageEditableText->GetText();

	if(ChatText.IsEmpty())
	{
		return;
	}

	AddTextWidgetClient(ChatText.ToString());
	
	UDiabloGameInstance::Get->m_ChatManager->ChatPost(ChatText);

	m_MesageEditableText->SetText(FText());

	m_MessageScrollBox->ScrollToEnd();
}

void UChatWindow::AddTextWidget(const FString& chat)
{
	UChatText* TopText = GetTopText();
	
	TopText->SetChat(chat);

	m_MessageScrollBox->AddChild(TopText);
}

void UChatWindow::AddTextWidgetClient(const FString& chat)
{
	UChatText* TopText = GetTopText();
	
	TopText->SetChatForClient(chat);

	m_MessageScrollBox->AddChild(TopText);
}

void UChatWindow::OnChatTextChanged(const FText& text)
{
	FString Str = text.ToString();
	
	int Diff = Str.Len() - 40;
	
	if(Diff>0)
	{
		Str = Str.LeftChop(Diff);
	}

	m_MesageEditableText->SetText(FText::FromString(Str));
}

void UChatWindow::SetChatWindowOpenClose()
{
	if(m_bIsOpened)
	{
		m_ChatTotalWindow->SetVisibility(ESlateVisibility::Collapsed);	
	}
	else
	{
		m_ChatTotalWindow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	
	m_bIsOpened =!m_bIsOpened;
	
	UDiabloGameInstance::Get->m_ChatManager->SetReceiveChat(m_bIsOpened);
}