#include "StageSelectPanel.h"
#include "PVPPanel.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DgContentManagers/DungeonManager.h"
#include "Widgets/GachaMenu/GachaPanel.h"
#include "Widgets/HUD/MainCanvas.h"

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

	m_BtnOpenDgKeyRefill->OnClicked.AddDynamic(this, &UStageSelectPanel::OpenDgKey);
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
	UpdateResourceTxt();

	UDiabloGameInstance::Get->m_GoldManager->m_OnCurrencyChanged.AddUObject(this,&UStageSelectPanel::UpdateGoldOnly);
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

void UStageSelectPanel::OpenDgKey()
{
	UDiabloGameInstance::Get->GetHud()->m_Canvas->OpenDgKeyRegfill();
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

void UStageSelectPanel::UpdateResourceTxt()//무기강화할때,티켓돌릴때,날개소모할때,골드소모할때
{
	m_ImgTxtDgTicket->SetText(FText::FromString(FString::Printf(TEXT("던전 열쇠 %d/20"),UDiabloGameInstance::Get->m_GoldManager->GetDungeonKeys())));
	m_TxtGold->SetText(FText::FromString(FString::Printf(TEXT("보유 골드:%s"),*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(UDiabloGameInstance::Get->m_GoldManager->GetCurrentGold(),2))));
	m_TxtWeaponStone->SetText(FText::FromString(FString::Printf(TEXT("보유 무기강화석:%d"),UDiabloGameInstance::Get->m_GoldManager->GetWeaponStone())));
	m_TxtSkillStone->SetText(FText::FromString(FString::Printf(TEXT("보유 스킬마정석:%d"),UDiabloGameInstance::Get->m_GoldManager->GetSkillStone())));
	m_TxtPetGachaTicket->SetText(FText::FromString(FString::Printf(TEXT("보유 펫가챠티켓:%d"),UDiabloGameInstance::Get->m_GoldManager->GetPetGachaTicket())));

	int W = UDiabloGameInstance::Get->m_GoldManager->GetWingTicket();
	m_TxtWingTicket->SetText(FText::FromString(FString::Printf(TEXT("보유 날개티켓:%d"),W)));
}

void UStageSelectPanel::UpdateGoldOnly()
{
	m_TxtGold->SetText(FText::FromString(FString::Printf(TEXT("보유 골드:%s"),*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(UDiabloGameInstance::Get->m_GoldManager->GetCurrentGold(),2))));
}

void UStageSelectPanel::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if(InVisibility==ESlateVisibility::SelfHitTestInvisible || InVisibility==ESlateVisibility::Visible || InVisibility==ESlateVisibility::HitTestInvisible)
	{
		UpdateResourceTxt();
	}
}
