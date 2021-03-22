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
	if(!m_AryQuestData.MyRangeCheck(index))
	{
		return false;
	}

	if(!m_AryQuestData[index].TryComplete())
	{
		return false;
	}

	m_OnQuestUpdate.Broadcast(index);

	return true;
}
