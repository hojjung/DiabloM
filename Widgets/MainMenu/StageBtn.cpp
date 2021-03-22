#include "StageBtn.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"

#define LOCTEXT_NAMESPACE "StageButton"

void UStageBtn::Init(bool dgUnlock,int index)
{
	m_nIndex = index;

	m_FormatStageName = LOCTEXT("StageNameFormat","Stage:{0}");

	FFormatOrderedArguments Args;
	
	Args.Add(m_nIndex);

	m_TxtStageName->SetText(FText::Format(m_FormatStageName,Args));

	SetUnlock(dgUnlock);

	m_StageBtn->OnClicked.AddDynamic(this,&UStageBtn::MoveToDungeon);
}

void UStageBtn::SetUnlock(bool b)
{
	if(!b)
	{
		m_TxtStageClear->SetText(LOCTEXT("NotCleared","Not Cleared"));
		m_StageBtn->SetIsEnabled(false);
	}
	else
	{
		m_TxtStageClear->SetText(LOCTEXT("Cleared","Cleared"));
		m_StageBtn->SetIsEnabled(true);
	}
}

void UStageBtn::SetCurrent()
{
	m_TxtStageClear->SetText(LOCTEXT("Here","Here"));
	m_StageBtn->SetIsEnabled(false);
}



void UStageBtn::MoveToDungeon()
{
	SetCurrent();
	UDiabloGameInstance::Get->m_DungeonManager->SelectDungeon(m_nIndex);
	UDiabloGameInstance::Get->m_DungeonManager->OpenLevel();
}
#undef LOCTEXT_NAMESPACE
