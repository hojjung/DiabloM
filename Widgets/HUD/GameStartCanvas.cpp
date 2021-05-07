#include "GameStartCanvas.h"
#include "Managers/DiabloGameInstance.h"

void UGameStartCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	m_AryNewsBody.Reset();
	m_AryNewsButtons.Reset();
	
	m_bRequestLock=false;
	m_bTitleNewsLock=false;
	m_bSetNicknameLock=false;
	m_bGameStartLock=false;
	m_bProcceedLoginLock=false;
	m_bIsVersionWrongLock=false;
	m_bIsServerClosedLock=false;
	
	m_BtnPrivacyPolicy->OnClicked.AddDynamic(this,&UGameStartCanvas::OpenPrivacyPolicy);
	m_BtnGameUsePolicy->OnClicked.AddDynamic(this,&UGameStartCanvas::OpenGameUsePolicy);
	m_BtnRefund->OnClicked.AddDynamic(this,&UGameStartCanvas::OpenRefund);
	m_BtnStore->OnClicked.AddDynamic(this,&UGameStartCanvas::OpenStore);
	
	//
	m_BtnNews01->OnClicked.AddDynamic(this,&UGameStartCanvas::ShowNews01);
	m_BtnNews02->OnClicked.AddDynamic(this,&UGameStartCanvas::ShowNews02);
	m_BtnNews03->OnClicked.AddDynamic(this,&UGameStartCanvas::ShowNews03);
	m_BtnNews04->OnClicked.AddDynamic(this,&UGameStartCanvas::ShowNews04);
	m_BtnNews05->OnClicked.AddDynamic(this,&UGameStartCanvas::ShowNews05);
	m_AryNewsButtons.Add(m_BtnNews01);
	m_AryNewsButtons.Add(m_BtnNews02);
	m_AryNewsButtons.Add(m_BtnNews03);
	m_AryNewsButtons.Add(m_BtnNews04);
	m_AryNewsButtons.Add(m_BtnNews05);
	m_BtnNews01->SetIsEnabled(false);
	m_BtnNews02->SetIsEnabled(false);
	m_BtnNews03->SetIsEnabled(false);
	m_BtnNews04->SetIsEnabled(false);
	m_BtnNews05->SetIsEnabled(false);
	//

	m_Nickname->OnTextChanged.AddDynamic(this,&UGameStartCanvas::OnTextChanged);

	m_BtnConfirmNickname->OnClicked.AddDynamic(this,&UGameStartCanvas::ConfirmNickname);

	m_PlayfabManager = UDiabloGameInstance::Get->m_PlayfabManager;
	//
	m_CanvasNicknameSet->SetVisibility(ESlateVisibility::Collapsed);
	
	m_CanvasPopupConfirm->SetVisibility(ESlateVisibility::Collapsed);

	m_ShowLoadingThrobber->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnEnterLevel->OnClicked.AddDynamic(this,&UGameStartCanvas::EnterLevel);

	m_BtnConfirm->OnClicked.AddDynamic(this,&UGameStartCanvas::ConfirmPopup);
	

	m_Handle = m_PlayfabManager.Get()->m_OnPlayfabError.AddUObject(this,&UGameStartCanvas::OpenErrorPopupPanel);

	m_BtnEnterLevel->SetIsEnabled(false);
	//
	m_TextClientVersion->SetText(FText::FromString(FString::Printf(TEXT("버전:%s"),*m_PlayfabManager.Get()->m_CurrentVersionName)));
}

void UGameStartCanvas::NativeDestruct()
{
	Super::NativeDestruct();

	m_PlayfabManager.Get()->m_OnPlayfabError.Remove(m_Handle);
}

void UGameStartCanvas::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!m_bSetNicknameLock && m_PlayfabManager.Get()->m_bShowNicknameSet)
	{
		m_bSetNicknameLock = true;

		m_CanvasNicknameSet->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		return;
	}

	if(!m_bProcceedLoginLock && m_PlayfabManager.Get()->m_bIsNicknameSet)
	{
		m_bProcceedLoginLock=true;

		m_CanvasNicknameSet->SetVisibility(ESlateVisibility::Collapsed);

		//m_ShowLoadingThrobber->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(!m_bIsServerClosedLock && m_PlayfabManager.Get()->m_bIsServerClosed)
	{
		m_bIsServerClosedLock = true;

		OpenErrorPopupPanel(FString::Printf(TEXT("서버 점검 중:%s"),*m_PlayfabManager.Get()->m_ServerCloseOpenTime));//시간
		
		return;
	}
	
	if(!m_bIsVersionWrongLock && m_PlayfabManager.Get()->m_bIsVersionWrong)
	{
		m_bIsVersionWrongLock = true;//버전도 띄워줄것

		OpenErrorPopupPanel(FString::Printf(TEXT("게임버전:%s,서버버전:%s"),*m_PlayfabManager.Get()->m_CurrentVersionName,*m_PlayfabManager.Get()->m_ServerVersion));//시간

		return;
	}

	if(!m_bTitleNewsLock && m_PlayfabManager.Get()->m_TitleNews.Num() > 0)
	{
		m_bTitleNewsLock = true;

		int Index=0;

		for(const auto& NewsItem : m_PlayfabManager.Get()->m_TitleNews)
		{
			m_AryNewsBody.Add(NewsItem.Body);

			m_AryNewsButtons[Index]->SetIsEnabled(true);

			Index++;
		}

		if(Index>0)
		{
			ShowNews01();
		}
	}

	

	
	//check server open

	//check version check

	if(!m_bGameStartLock && m_PlayfabManager.Get()->m_bIsLoginCompleted)
	{
		m_bGameStartLock = true;

		m_ShowLoadingThrobber->SetVisibility(ESlateVisibility::Collapsed);

		m_BtnEnterLevel->SetIsEnabled(true);
	}
}

void UGameStartCanvas::ReqeustPopupText(const FText& str)
{
	m_TextAPIInfo->SetText(str);	
}

void UGameStartCanvas::OpenErrorPopupPanel(const FString& str)
{
	m_bRequestLock = false;
	
	m_CanvasPopupConfirm->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_ShowLoadingThrobber->SetVisibility(ESlateVisibility::Collapsed);
	
	m_TextAPIInfoInPopup->SetText(FText::FromString(str));
}

void UGameStartCanvas::OpenPrivacyPolicy()
{
	UKismetSystemLibrary::LaunchURL(TEXT("https://sites.google.com/view/hereticbyteprivacypolicy/%ED%99%88"));
}

void UGameStartCanvas::OpenGameUsePolicy()
{
	UKismetSystemLibrary::LaunchURL(TEXT("https://sites.google.com/view/hereticbytegameplaypolicy/%ED%99%88"));
}

void UGameStartCanvas::OpenRefund()
{
	UKismetSystemLibrary::LaunchURL(TEXT("https://sites.google.com/view/hereticbyterefundpolicy/%ED%99%88"));
}

void UGameStartCanvas::OpenStore()
{
	UKismetSystemLibrary::LaunchURL(TEXT("https://play.google.com/store/apps/details?id=com.hereticbyte.dungeonslasher"));
}

void UGameStartCanvas::ConfirmNickname()
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
	
	m_PlayfabManager.Get()->RequestSetNickname(m_Nickname->GetText().ToString());

	m_ShowLoadingThrobber->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGameStartCanvas::EnterLevel()
{
	UDiabloGameInstance::Get->m_DungeonManager->OpenLevel(UDiabloGameInstance::Get->m_NormalDgManager);
}
void UGameStartCanvas::OnTextChanged(const FText& text)
{
	FString Str = text.ToString();
	int Diff = Str.Len() - 15; 
	if(Diff>0)
	{
		Str = Str.LeftChop(Diff);
	}

	m_Nickname->SetText(FText::FromString(Str));
}

void UGameStartCanvas::ShowNews01()
{
	m_TextNewsBody->SetText(FText::FromString(m_AryNewsBody[0]));	
}

void UGameStartCanvas::ShowNews02()
{
	m_TextNewsBody->SetText(FText::FromString(m_AryNewsBody[1]));
}

void UGameStartCanvas::ShowNews03()
{
	m_TextNewsBody->SetText(FText::FromString(m_AryNewsBody[2]));
}

void UGameStartCanvas::ShowNews04()
{
	m_TextNewsBody->SetText(FText::FromString(m_AryNewsBody[3]));
}

void UGameStartCanvas::ShowNews05()
{
	m_TextNewsBody->SetText(FText::FromString(m_AryNewsBody[4]));
}

void UGameStartCanvas::ConfirmPopup()
{
	m_CanvasPopupConfirm->SetVisibility(ESlateVisibility::Collapsed);

	if(m_bIsVersionWrongLock)
	{
		OpenStore();
	}
}
