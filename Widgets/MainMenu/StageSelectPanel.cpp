#include "StageSelectPanel.h"
#include "PVPPanel.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DgContentManagers/DungeonManager.h"
#include "Widgets/GachaMenu/GachaPanel.h"

void UStageSelectPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InitNormalDungeon();

	UDiabloGameInstance::Get->m_NormalDgManager->m_OnDgOpen.AddUObject(this, &UStageSelectPanel::UpdateBtnUI);
	//
	m_DgStageBtnListVert->SetVisibility(ESlateVisibility::Collapsed);
	m_DgMagicPanel->SetVisibility(ESlateVisibility::Collapsed); //골드던전 가림
	m_DgPetPanel->SetVisibility(ESlateVisibility::Collapsed);
	m_DgWeaponPanel->SetVisibility(ESlateVisibility::Collapsed);
	//
	m_DgPVPPanel->SetVisibility(ESlateVisibility::Collapsed);
	// //
	m_BtnShowNormalDg->OnClicked.AddDynamic(this, &UStageSelectPanel::OpenNormalDg);
	m_BtnShowMagicDg->OnClicked.AddDynamic(this, &UStageSelectPanel::OpenGoldDg);
	m_BtnShowPetDg->OnClicked.AddDynamic(this, &UStageSelectPanel::OpenPetDg);
	m_BtnShowWeaponDg->OnClicked.AddDynamic(this, &UStageSelectPanel::OpenWeaponDg);
	m_BtnPVP->OnClicked.AddDynamic(this, &UStageSelectPanel::OpenPVPDg);
	m_BtnBack->OnClicked.AddDynamic(this, &UStageSelectPanel::Back);
	//
	m_BtnBack->SetVisibility(ESlateVisibility::Collapsed);
	//
	
	int DgKey = UDiabloGameInstance::Get->m_DungeonManager->GetDgKey();
	
	UpdateDgKey(DgKey);
	
	UDiabloGameInstance::Get->m_PlayfabManager->m_OnDgKeyChanged.AddUObject(this,&UStageSelectPanel::UpdateDgKey);
}

void UStageSelectPanel::InitNormalDungeon()
{
	int Len = UDiabloGameInstance::Get->m_NormalDgManager->GetAryDgData().Num();

	int MaxLevel = UDiabloGameInstance::Get->m_NormalDgManager->m_nMyMaxStageLevel.GetValue();

	int Current = UDiabloGameInstance::Get->m_NormalDgManager->m_nCurrentStageLevel.GetValue();

	for (int i = 0; i < Len; i++)
	{
		UStageBtn* CreatedStageBtn = CreateWidget<UStageBtn>(this, m_ClassStageBtn);

		CreatedStageBtn->Init(i <= MaxLevel, i);

		m_DgStageBtnListVert->AddChild(CreatedStageBtn);

		CreatedStageBtn->SetPadding(FMargin(10, 10, 10, 10));

		m_AryStageBtn.Add(CreatedStageBtn);
	}

	m_AryStageBtn[Current]->SetCurrent();
}

void UStageSelectPanel::UpdateBtnUI(int index)
{
	m_AryStageBtn[index]->SetCurrent();
}

void UStageSelectPanel::OpenNormalDg()
{
	m_DgStageBtnListVert->SetVisibility(ESlateVisibility::Visible);

	m_StageTypeSelectVert->SetVisibility(ESlateVisibility::Collapsed);
	
	m_BtnBack->SetVisibility(ESlateVisibility::Visible);
}

void UStageSelectPanel::OpenGoldDg()
{
	m_DgMagicPanel->SetVisibility(ESlateVisibility::Visible);

	m_StageTypeSelectVert->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnBack->SetVisibility(ESlateVisibility::Visible);
}

void UStageSelectPanel::OpenPetDg()
{
	m_DgPetPanel->SetVisibility(ESlateVisibility::Visible);

	m_StageTypeSelectVert->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnBack->SetVisibility(ESlateVisibility::Visible);
}

void UStageSelectPanel::OpenWeaponDg()
{
	m_DgWeaponPanel->SetVisibility(ESlateVisibility::Visible);

	m_StageTypeSelectVert->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnBack->SetVisibility(ESlateVisibility::Visible);
}

void UStageSelectPanel::OpenPVPDg()
{
	m_DgPVPPanel->SetVisibility(ESlateVisibility::Visible);

	m_StageTypeSelectVert->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnBack->SetVisibility(ESlateVisibility::Visible);
}


void UStageSelectPanel::Back()
{
	m_StageTypeSelectVert->SetVisibility(ESlateVisibility::SelfHitTestInvisible); //전체버튼 다시보여줌
	//
	m_DgStageBtnListVert->SetVisibility(ESlateVisibility::Collapsed);
	m_DgMagicPanel->SetVisibility(ESlateVisibility::Collapsed); //골드던전 가림
	m_DgPetPanel->SetVisibility(ESlateVisibility::Collapsed);
	m_DgWeaponPanel->SetVisibility(ESlateVisibility::Collapsed);
	m_DgPVPPanel->SetVisibility(ESlateVisibility::Collapsed);
	//
	m_BtnBack->SetVisibility(ESlateVisibility::Collapsed);
}

void UStageSelectPanel::UpdateDgKey(int kV)
{
	m_ImgTxtDgTicket->SetText(FText::FromString(FString::Printf(TEXT("던전 열쇠 %d/15"),kV)));
}
