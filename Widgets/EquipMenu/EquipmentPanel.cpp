// My First Hack n Slash


#include "EquipmentPanel.h"

#include "Managers/DiabloGameInstance.h"
#include "Managers/EquipManager.h"

void UEquipmentPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_AryPlSkinBtn.Reset();

	FMargin MargW = FMargin(12.f,12.f,12.f,12.f);

	int Index = 0;

	APlayerController* PlCon = UGameplayStatics::GetPlayerController(UDiabloGameInstance::Get->GetWorld(),0);

	for(const FPlayerClassSpec& PlSpec : UDiabloGameInstance::Get->m_EquipManager->m_AryPlayerSkin)
	{
		UPlayerSkinEquipButton* EqBtn1 = CreateWidget<UPlayerSkinEquipButton>(PlCon,m_ClassPlayerEquipBtn);

		EqBtn1->Init(PlSpec,Index++);
		
		m_AryPlSkinBtn.Add(EqBtn1);

		m_VertiSkin->AddChild(EqBtn1);

		EqBtn1->SetPadding(MargW);

		if(PlSpec.m_nIsUnlocked<1)
		{
			//EqBtn1->m_BtnEquip->SetIsEnabled(false);
		}
	}

	Index = 0;

	for(const FWingSpec& EqSpec2 : UDiabloGameInstance::Get->m_EquipManager->m_AryWings)
	{
		UWingEquipButton* EqBtn2 = CreateWidget<UWingEquipButton>(PlCon,m_ClassWingEquipBtn);

		EqBtn2->Init(EqSpec2,Index++);
		
		m_AryEqWingBtn.Add(EqBtn2);

		m_VertiWing->AddChild(EqBtn2);

		EqBtn2->SetPadding(MargW);

		if(EqSpec2.m_nIsUnlocked<1)
		{
			//EqBtn->m_BtnEquip->SetIsEnabled(false);
		}
	}

	Index = 0;

	for(const FWeaponSpec& EqSpec3 : UDiabloGameInstance::Get->m_EquipManager->m_AryWeapons)
	{
		UWeaponEquipButton* EqBtn3 = CreateWidget<UWeaponEquipButton>(PlCon,m_ClassWeaponEquipBtn);

		EqBtn3->Init(EqSpec3,Index++);
		
		m_AryEqWeaponBtn.Add(EqBtn3);

		m_VertiWeapon->AddChild(EqBtn3);

		EqBtn3->SetPadding(MargW);

		if(EqSpec3.m_nLv<1)
		{
			//EqBtn->m_BtnEquip->SetIsEnabled(false);
		}
	}

	Index = 0;

	for(const FPetSpec& EqSpec4 : UDiabloGameInstance::Get->m_EquipManager->m_AryPets)
	{
		UPetEquipButton* EqBtn4 = CreateWidget<UPetEquipButton>(PlCon,m_ClassPetEquipBtn);
		
		EqBtn4->Init(EqSpec4,Index++);
		
		m_AryEqPetBtn.Add(EqBtn4);

		m_VertiPet->AddChild(EqBtn4);

		EqBtn4->SetPadding(MargW);

		if(EqSpec4.m_nLv<1)
		{
			//EqBtn->m_BtnEquip->SetIsEnabled(false);
		}
	}

	Index = 0;
	
	for(const FAccessorySpec& EqSpec5 : UDiabloGameInstance::Get->m_EquipManager->m_AryAcce)
	{
		UAccessoryEquipButton* EqBtn5 = CreateWidget<UAccessoryEquipButton>(PlCon,m_ClassAccessoryEquipBtn);

		EqBtn5->Init(EqSpec5,this,Index++);
		
		m_AryEqAccessoryBtn.Add(EqBtn5);

		m_VertiAccessory->AddChild(EqBtn5);

		EqBtn5->SetPadding(MargW);

		if(EqSpec5.m_nLv<1)
		{
			//EqBtn->m_BtnEquip->SetIsEnabled(false);
		}
	}

	m_BtnClose->OnClicked.AddDynamic(this,&UEquipmentPanel::ClosePanel);

	m_BtnClassSkin->OnClicked.AddDynamic(this,&UEquipmentPanel::SetPanelSkin);
	m_BtnWeapon->OnClicked.AddDynamic(this,&UEquipmentPanel::UEquipmentPanel::SetPanelWeapon);
	m_BtnWing->OnClicked.AddDynamic(this,&UEquipmentPanel::UEquipmentPanel::SetPanelWing);
	m_BtnPet->OnClicked.AddDynamic(this,&UEquipmentPanel::UEquipmentPanel::SetPanelPet);
	m_BtnAccessory->OnClicked.AddDynamic(this,&UEquipmentPanel::SetPanelAccessory);
	//
	UDiabloGameInstance::Get->m_EquipManager->m_OnAccessoryChanged.AddUObject(this,&UEquipmentPanel::OnAccessoryChanged);
	UDiabloGameInstance::Get->m_EquipManager->m_OnPlSkinChanged.AddUObject(this,&UEquipmentPanel::OnSkinChanged);
	UDiabloGameInstance::Get->m_EquipManager->m_OnPetChanged.AddUObject(this,&UEquipmentPanel::OnPetChanged);
	UDiabloGameInstance::Get->m_EquipManager->m_OnWeaponChanged.AddUObject(this,&UEquipmentPanel::OnWeaponChanged);
	UDiabloGameInstance::Get->m_EquipManager->m_OnWingChanged.AddUObject(this,&UEquipmentPanel::OnWingChanged);

	//SetPanelSkin();
}

void UEquipmentPanel::OnSkinChanged(int pre, int next)
{
	if(pre>-1)
	{
		m_AryPlSkinBtn[pre]->UpdateEquipSkin();
	}
	m_AryPlSkinBtn[next]->UpdateEquipSkin();
}

void UEquipmentPanel::OnWeaponChanged(int pre, int next)
{
	if(pre>-1)
	{
		m_AryEqWeaponBtn[pre]->UpdateEquipWeapon();
	}
	m_AryEqWeaponBtn[next]->UpdateEquipWeapon();
}

void UEquipmentPanel::OnWingChanged(int pre, int next)
{
	if(pre>-1)
	{
		m_AryEqWingBtn[pre]->UpdateEquipWing();
	}
	m_AryEqWingBtn[next]->UpdateEquipWing();
}

void UEquipmentPanel::OnPetChanged(int pre, int next)
{
	if(pre>-1)
	{
		m_AryEqPetBtn[pre]->UpdateEquipPet();
	}
	m_AryEqPetBtn[next]->UpdateEquipPet();
}

void UEquipmentPanel::OnAccessoryChanged(int pre, int next)
{
	if(pre>-1)
	{
		m_AryEqAccessoryBtn[pre]->UpdateEquipAccessory();
	}
	m_AryEqAccessoryBtn[next]->UpdateEquipAccessory();
}

void UEquipmentPanel::SetHoverImage(UButton** btnWant)
{
	m_PtrBtn = btnWant;
	
	auto& Style = (*m_PtrBtn)->WidgetStyle;
	
	m_CachedNormal = Style.Normal; 
	
	Style.Normal = Style.Hovered;

	(*m_PtrBtn)->SetStyle(Style);
}

void UEquipmentPanel::SetNormalImage()
{
	if(!m_PtrBtn)
	{
		return;
	}
	
	(*m_PtrBtn)->WidgetStyle.Normal = m_CachedNormal;
	
	(*m_PtrBtn)->SetStyle((*m_PtrBtn)->WidgetStyle);
}

void UEquipmentPanel::ClosePanel()
{
	SetVisibility(ESlateVisibility::Collapsed);

	UDiabloGameInstance::Get->m_AdverManager->ShowBannerAD(true);
}

void UEquipmentPanel::SetPanelSkin()
{
	if(m_CurrentOpenedWidget)
	{
		m_CurrentOpenedWidget->SetVisibility(ESlateVisibility::Collapsed);

		SetNormalImage();

		if(m_CurrentOpenedWidget==m_OverlaySkin)
		{
			m_CurrentOpenedWidget = nullptr;
			
			return;
		}
	}
	
	m_OverlaySkin->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_CurrentOpenedWidget=m_OverlaySkin;

	SetHoverImage(&m_BtnClassSkin);
}

void UEquipmentPanel::SetPanelWing()
{
	if(m_CurrentOpenedWidget)
	{
		m_CurrentOpenedWidget->SetVisibility(ESlateVisibility::Collapsed);

		SetNormalImage();

		if(m_CurrentOpenedWidget==m_OverlayWing)
		{
			m_CurrentOpenedWidget = nullptr;

			return;
		}
	}

	m_OverlayWing->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_CurrentOpenedWidget=m_OverlayWing;
	
	SetHoverImage(&m_BtnWing);
}

void UEquipmentPanel::SetPanelPet()
{
	if(m_CurrentOpenedWidget)
	{
		m_CurrentOpenedWidget->SetVisibility(ESlateVisibility::Collapsed);
		
		SetNormalImage();
		
		if(m_CurrentOpenedWidget==m_OverlayPet)
		{
			m_CurrentOpenedWidget = nullptr;

			return;
		}

	}

	m_OverlayPet->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_CurrentOpenedWidget=m_OverlayPet;
	
	SetHoverImage(&m_BtnPet);
}

void UEquipmentPanel::SetPanelWeapon()
{
	if(m_CurrentOpenedWidget)
	{
		m_CurrentOpenedWidget->SetVisibility(ESlateVisibility::Collapsed);

		SetNormalImage();
		
		if(m_CurrentOpenedWidget==m_OverlayWeapon)
		{
			m_CurrentOpenedWidget = nullptr;

			return;
		}
	}

	m_OverlayWeapon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_CurrentOpenedWidget=m_OverlayWeapon;
	
	//SetNormalImage();
	
	SetHoverImage(&m_BtnWeapon);
}

void UEquipmentPanel::SetPanelAccessory()
{
	if(m_CurrentOpenedWidget)
	{
		m_CurrentOpenedWidget->SetVisibility(ESlateVisibility::Collapsed);

		SetNormalImage();
		
		if(m_CurrentOpenedWidget==m_OverlayAccessory)
		{
			m_CurrentOpenedWidget = nullptr;

			return;
		}
	}

	m_OverlayAccessory->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_CurrentOpenedWidget=m_OverlayAccessory;
	
	SetHoverImage(&m_BtnAccessory);
}

