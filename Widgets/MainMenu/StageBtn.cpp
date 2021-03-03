#include "StageBtn.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"


#define LOCTEXT_NAMESPACE "StageButton"
void UStageBtn::Init(int dgUnlock,int index)
{
	m_nIndex = index;

	m_FormatStageName = LOCTEXT("StageNameFormat","Stage:{0}");

	FFormatOrderedArguments Args;
	
	Args.Add(m_nIndex);

	m_TxtStageName->SetText(FText::Format(m_FormatStageName,Args));
	
	switch (dgUnlock)
	{
	case 0:
		m_TxtStageClear->SetText(LOCTEXT("NotCleared","Not Cleared"));
		m_StageBtn->SetIsEnabled(false);
		break;
	case 1:
		m_TxtStageClear->SetText(LOCTEXT("Cleared","Cleared"));
		m_StageBtn->SetIsEnabled(true);
		break;
	case 2:
		m_TxtStageClear->SetText(LOCTEXT("Current","Here"));
		m_StageBtn->SetIsEnabled(true);
		break;
	default: ;
	}

	m_StageBtn->OnClicked.AddDynamic(this,&UStageBtn::MoveToDungeon);
}

void UStageBtn::MoveToDungeon()
{
	UDiabloGameInstance::Get->m_DungeonManager->SelectDungeon(m_nIndex);
	UDiabloGameInstance::Get->m_DungeonManager->OpenLevel();
}
#undef LOCTEXT_NAMESPACE
