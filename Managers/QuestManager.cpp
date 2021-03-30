#include "QuestManager.h"

#include "DiabloGameInstance.h"


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
	int GemStone = m_AryQuestData[index].GetCompletePrize();
	
	if(!m_AryQuestData[index].TryComplete())
	{
		return false;
	}

	AddGemStones(GemStone);

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

void UQuestManager::AddGemStones(int gemStone)
{
	m_nWaitingGemStones+=gemStone;
}

void UQuestManager::RequestGemStoneUploadToServer()
{
	if(m_nWaitingGemStones<1)
	{
		return;
	}
	
	PRINTF("UploadTemStones:%d",m_nWaitingGemStones);
	
	UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(m_nWaitingGemStones);
	
	UDiabloGameInstance::Get->m_PlayfabManager->UploadQuestData(GetQuestDataStr());
	
	m_nWaitingGemStones = 0;
}

FString UQuestManager::GetQuestDataStr()
{
	FString ResultStr;
	
	for(auto& QuestData : m_AryQuestData)
	{
		FString Data = QuestData.ParseToStr();
		
		Data.AppendChar(TEXT('/'));

		ResultStr.Append(Data);
	}

	return ResultStr;
}
