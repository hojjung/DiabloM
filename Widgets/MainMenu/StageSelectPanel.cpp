#include "StageSelectPanel.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"


void UStageSelectPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	int Len = UDiabloGameInstance::Get->m_DungeonManager->GetAryDgData().Num();
	
	int MaxLevel = UDiabloGameInstance::Get->m_DungeonManager->GetMaxStage();

	int Current =  UDiabloGameInstance::Get->m_DungeonManager->GetCurrentStage();

	for(int i=0; i< Len; i++)
	{
		UStageBtn* CreatedStageBtn = CreateWidget<UStageBtn>(this,m_ClassStageBtn);

		CreatedStageBtn->Init(i<=MaxLevel,i);

		m_StageBtnListVert->AddChild(CreatedStageBtn);

		CreatedStageBtn->SetPadding(FMargin(10,10,10,10));

		m_AryStageBtn.Add(CreatedStageBtn);
	}

	m_AryStageBtn[Current]->SetCurrent();

	UDiabloGameInstance::Get->m_DungeonManager->m_OnDgOpen.AddUObject(this,&UStageSelectPanel::UpdateBtnUI);
	//
	// m_StageBtnListVert->SetVisibility(ESlateVisibility::Collapsed);
	// m_DgGoldStageBtnListVert->SetVisibility(ESlateVisibility::Collapsed);
	// //
	// m_BtnShowNormalDg->OnClicked.AddDynamic(this,&UStageSelectPanel::OpenNormalDg);
	// m_BtnShowGoldDg->OnClicked.AddDynamic(this,&UStageSelectPanel::OpenGoldDg);
}

void UStageSelectPanel::UpdateBtnUI(int index)
{
	m_AryStageBtn[index]->SetCurrent();
}

void UStageSelectPanel::OpenNormalDg()
{
	m_StageBtnListVert->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UStageSelectPanel::OpenGoldDg()
{
	m_DgGoldStageBtnListVert->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
