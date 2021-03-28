#include "QuestManager.h"


void UQuestManager::SetQuestDataFromServer(const FString& strQuest)
{
	m_AryQuestData.Reset();
	
	TArray<FString> AryQuest;

	strQuest.ParseIntoArray(AryQuest,TEXT("/"));

	TArray<FQuestDataRow*> AryQuestRow;

	UQuestData::GetQuestData->GetAllRows("",AryQuestRow);

	for(int i=0;i<AryQuestRow.Num();i++)
	{
		FQuestDataSpec QuestDataSpec;

		QuestDataSpec.ParseFromStr(AryQuest[i]);

		QuestDataSpec.m_Data = AryQuestRow[i];
		
		m_AryQuestData.Add(QuestDataSpec);
	}
}

bool UQuestManager::CompleteQuest(int index)
{
	if(!m_AryQuestData[index].TryComplete())
	{
		return false;
	}

	m_OnQuestUpdate.Broadcast(index);

	return true;
}

FQuestDataSpec& UQuestManager::GetQuest(EQuestType type)
{
	return m_AryQuestData[(int)type];
}

void UQuestManager::AddQuestCount(EQuestType type)
{
	GetQuest(type).m_nCurrentRequirePoint++;
	m_OnQuestUpdate.Broadcast((int)type);
}
