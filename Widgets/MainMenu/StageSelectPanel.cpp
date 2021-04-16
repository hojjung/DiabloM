#include "StageSelectPanel.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"


void UStageSelectPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	int Len = UDiabloGameInstance::Get->m_DungeonManager->GetAryDgData().Num();

	int MaxLevel = UDiabloGameInstance::Get->m_DungeonManager->GetMaxStage();

	int Current = UDiabloGameInstance::Get->m_DungeonManager->GetCurrentStage();

	for (int i = 0; i < Len; i++)
	{
		UStageBtn* CreatedStageBtn = CreateWidget<UStageBtn>(this, m_ClassStageBtn);

		CreatedStageBtn->Init(i <= MaxLevel, i);

		m_DgStageBtnListVert->AddChild(CreatedStageBtn);

		CreatedStageBtn->SetPadding(FMargin(10, 10, 10, 10));

		m_AryStageBtn.Add(CreatedStageBtn);
	}

	m_AryStageBtn[Current]->SetCurrent();

	UDiabloGameInstance::Get->m_DungeonManager->m_OnDgOpen.AddUObject(this, &UStageSelectPanel::UpdateBtnUI);
	//
	m_DgStageBtnListVert->SetVisibility(ESlateVisibility::Collapsed);
	m_DgGoldStageBtnListVert->SetVisibility(ESlateVisibility::Collapsed); //골드던전 가림
	m_DgAcceStageBtnListVert->SetVisibility(ESlateVisibility::Collapsed);
	m_DgStatBtnListVert->SetVisibility(ESlateVisibility::Collapsed);
	// //
	m_BtnShowNormalDg->OnClicked.AddDynamic(this, &UStageSelectPanel::OpenNormalDg);
	m_BtnShowGoldDg->OnClicked.AddDynamic(this, &UStageSelectPanel::OpenGoldDg);
	m_BtnShowStatDg->OnClicked.AddDynamic(this, &UStageSelectPanel::OpenStatDg);
	m_BtnShowAcceDg->OnClicked.AddDynamic(this, &UStageSelectPanel::OpenAcceDg);
	m_BtnBack->OnClicked.AddDynamic(this, &UStageSelectPanel::Back);
}

void UStageSelectPanel::UpdateBtnUI(int index)
{
	m_AryStageBtn[index]->SetCurrent();
}

void UStageSelectPanel::OpenNormalDg()
{
	m_DgStageBtnListVert->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_StageTypeSelectVert->SetVisibility(ESlateVisibility::Collapsed);
}

void UStageSelectPanel::OpenGoldDg()
{
	m_DgGoldStageBtnListVert->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_StageTypeSelectVert->SetVisibility(ESlateVisibility::Collapsed);
}

void UStageSelectPanel::OpenStatDg()
{
	m_DgStatBtnListVert->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_StageTypeSelectVert->SetVisibility(ESlateVisibility::Collapsed);
}

void UStageSelectPanel::OpenAcceDg()
{
	m_DgAcceStageBtnListVert->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_StageTypeSelectVert->SetVisibility(ESlateVisibility::Collapsed);
}

void UStageSelectPanel::Back()
{
	m_StageTypeSelectVert->SetVisibility(ESlateVisibility::SelfHitTestInvisible); //전체버튼 다시보여줌
	//
	m_DgStageBtnListVert->SetVisibility(ESlateVisibility::Collapsed);
	m_DgGoldStageBtnListVert->SetVisibility(ESlateVisibility::Collapsed); //골드던전 가림
	m_DgAcceStageBtnListVert->SetVisibility(ESlateVisibility::Collapsed);
	m_DgStatBtnListVert->SetVisibility(ESlateVisibility::Collapsed);
}
