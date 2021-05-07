#include "QuestManager.h"
#include "DiabloGameInstance.h"
#include "JsonObjectConverter.h"


void UQuestManager::SetQuestDataFromServer(const FString& strQuest)
{
	m_AryQuestData.Reserve(30);
	
	if(!FJsonObjectConverter::JsonArrayStringToUStruct(strQuest, &m_AryQuestData, 0, 0))
	{
		return;
	}
	
	TArray<FQuestDataRow*> AryQuestRow;

	UQuestData::GetQuestData->GetAllRows("",AryQuestRow);

	int IterMax = FMath::Min(AryQuestRow.Num(),m_AryQuestData.Num());
	
	for(int i=0;i<IterMax; i++)
	{
		m_AryQuestData[i].m_Data = AryQuestRow[i];
	}

	int MaxStage = UDiabloGameInstance::Get->m_NormalDgManager->GetMaxStage();
	
	if(MaxStage>m_AryQuestData[12].QuestAmount)//던전 데이터 업로드 실패 예외처리
	{
		m_AryQuestData[12].QuestAmount =MaxStage;	
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
	GetQuest(type).QuestAmount++;
	m_OnQuestUpdate.Broadcast((int)type);
}

void UQuestManager::AddGemStones(int gemStone)
{
	m_nWaitingGemStones+=gemStone;
}

void UQuestManager::UploadQuestData()
{
	UDiabloGameInstance::Get->m_PlayfabManager->UploadQuestData(GetQuestDataStr());
}

void UQuestManager::RequestGemStoneUploadToServer(bool bForce)
{
	if(!bForce&&m_nWaitingGemStones<1)
	{
		return;
	}
	
	PRINTF("UploadTemStones:%d",m_nWaitingGemStones);
	
	UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(m_nWaitingGemStones);
	
	UploadQuestData();

	UDiabloGameInstance::Get->RequestPopupText(FString::Printf(TEXT("젬스톤%d개 업로드중"),m_nWaitingGemStones));
	
	m_nWaitingGemStones = 0;
}

FString UQuestManager::GetQuestDataStr()
{
	TSharedPtr<FJsonObject> AryQuestJsonObj = MakeShareable(new FJsonObject);

	TArray<TSharedPtr<FJsonValue>> AryQuestSpec;
	AryQuestSpec.Empty(30);
	//<TCHAR, TCondensedJsonPrintPolicy<TCHAR> >
	for(const FQuestDataSpec& QuestSpec : m_AryQuestData)
	{
		TSharedRef<FJsonObject> CurrentQuestJson= MakeShareable(new FJsonObject);
		
		if(!FJsonObjectConverter::UStructToJsonObject(FQuestDataSpec::StaticStruct(),&QuestSpec,CurrentQuestJson,0,0))
		{
			break; 
		}

		TSharedRef< FJsonValueObject > QuestJsonValue = MakeShareable( new FJsonValueObject( CurrentQuestJson) );
		
		AryQuestSpec.Add(QuestJsonValue);
	}
	
	FString QuestJsonValueStr;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter=
	TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&QuestJsonValueStr);
	FJsonSerializer::Serialize(AryQuestSpec, JsonWriter);
	JsonWriter->Close();

	return QuestJsonValueStr;
}
