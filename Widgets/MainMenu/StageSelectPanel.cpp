#include "StageSelectPanel.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"


void UStageSelectPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	int Len = UDiabloGameInstance::Get->m_DungeonManager->GetAryDgData().Num();
	
	int MaxLevel = UDiabloGameInstance::Get->m_DungeonManager->GetMyMaxStageLevel();

	int Current =  UDiabloGameInstance::Get->m_DungeonManager->GetCurrentStageLevel();

	for(int i=0; i< Len; i++)
	{
		UStageBtn* CreatedStageBtn = CreateWidget<UStageBtn>(this,m_ClassStageBtn);

		CreatedStageBtn->Init(i<MaxLevel,i);

		m_StageBtnListVert->AddChild(CreatedStageBtn);

		CreatedStageBtn->SetPadding(FMargin(10,10,10,10));

		m_AryStageBtn.Add(CreatedStageBtn);
	}

	m_AryStageBtn[Current]->SetCurrent();

	UDiabloGameInstance::Get->m_DungeonManager->m_OnDgOpen.AddUObject(this,&UStageSelectPanel::UpdateBtnUI);
}

void UStageSelectPanel::UpdateBtnUI(int index)
{
	m_AryStageBtn[index]->SetCurrent();
}
