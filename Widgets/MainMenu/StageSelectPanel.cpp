#include "StageSelectPanel.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"


void UStageSelectPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	int Index = 0;

	for(int nDgUnlocked : UDiabloGameInstance::Get->m_DungeonManager->GetAryDgUnlocked())
	{
		UStageBtn* CreatedStageBtn = CreateWidget<UStageBtn>(this,m_ClassStageBtn);

		CreatedStageBtn->Init(nDgUnlocked,Index);
		
		m_StageBtnListVert->AddChild(CreatedStageBtn);

		CreatedStageBtn->SetPadding(FMargin(10,10,10,10));

		m_AryStageBtn.Add(CreatedStageBtn);

		Index++;
	}
}
