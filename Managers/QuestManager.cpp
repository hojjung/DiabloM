#include "QuestManager.h"


void UQuestManager::SetQuestDataFromServer(const FString& strQuest)
{
	m_AryQuestData.Reset();
	
	TArray<FString> AryQuest;

	strQuest.ParseIntoArray(AryQuest,TEXT("/"));

	for(auto& QuestStr : AryQuest)
	{
		FQuestDataSpec QuestDataSpec;

		QuestDataSpec.ParseFromStr(QuestStr);
		
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
