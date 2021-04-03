#include "SetNickname.h"
#include "Managers/DiabloGameInstance.h"

USetNickname::~USetNickname()
{
	UDiabloGameInstance::Get->m_PlayfabManager->m_OnPlayfabError.Remove(m_Dele);
}

void USetNickname::ReqeustPopupText(FString str)
{
	m_TextAPI->SetText(FText::FromString(str));
}

void USetNickname::ReqeustPopupText(FText str)
{
	m_TextAPI->SetText(str);
}

void USetNickname::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_Nickname->OnTextChanged.AddDynamic(this,&USetNickname::OnTextChanged);
	
	m_BtnConfirm->OnClicked.AddDynamic(this,&USetNickname::TryConfirm);
	
	m_BtnNotificationConfirm->OnClicked.AddDynamic(this,&USetNickname::ConfirmNoti);

	m_CanvasNotification->SetVisibility(ESlateVisibility::Collapsed);

	 m_Dele = UDiabloGameInstance::Get->m_PlayfabManager->m_OnPlayfabError.AddUObject(
		this, &USetNickname::ShowNotification);
}

void USetNickname::TryConfirm()
{
	if(m_bRequestLock)
	{
		return;
	}
	if(m_Nickname->GetText().IsEmpty())
	{
		return;
	}
	m_bRequestLock = true;
	UDiabloGameInstance::Get->m_PlayfabManager->RequestSetNickname(m_Nickname->GetText().ToString());
}

void USetNickname::ConfirmNoti()
{
	m_CanvasNotification->SetVisibility(ESlateVisibility::Collapsed);
	
}

void USetNickname::ShowNotification(FString& str)
{
	m_bRequestLock=false;
	m_CanvasNotification->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_TextNotification->SetText(FText::FromString(str));
	m_Nickname->SetText(FText());
}

void USetNickname::HideNicknameSet()
{
	m_CanvasNicknamePanel->SetVisibility(ESlateVisibility::Collapsed);
}

void USetNickname::ShowNicknameSet()
{
	m_CanvasNicknamePanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void USetNickname::OnTextChanged(const FText& text)
{
	FString Str = text.ToString();
	int Diff = Str.Len() - 15; 
	if(Diff>0)
	{
		Str = Str.LeftChop(Diff);
	}

	m_Nickname->SetText(FText::FromString(Str));
}

