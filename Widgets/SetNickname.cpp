#include "SetNickname.h"
#include "Managers/DiabloGameInstance.h"

USetNickname::~USetNickname()
{
	UDiabloGameInstance::Get->m_PlayfabManager->m_OnPlayfabError.Remove(m_Dele);
}

void USetNickname::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	m_BtnConfirm->OnClicked.AddDynamic(this,&USetNickname::TryConfirm);
	m_BtnNotificationConfirm->OnClicked.AddDynamic(this,&USetNickname::ConfirmNoti);

	m_CanvasNotification->SetVisibility(ESlateVisibility::Collapsed);

	 m_Dele = UDiabloGameInstance::Get->m_PlayfabManager->m_OnPlayfabError.AddUObject(
		this, &USetNickname::ShowNotification);
}

void USetNickname::TryConfirm()
{
	UDiabloGameInstance::Get->m_PlayfabManager->RequestSetNickname(m_Nickname->GetText().ToString());
}

void USetNickname::ConfirmNoti()
{
	m_CanvasNotification->SetVisibility(ESlateVisibility::Collapsed);
}

void USetNickname::ShowNotification(FString& str)
{
	m_CanvasNotification->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_TextNotification->SetText(FText::FromString(str));
}
