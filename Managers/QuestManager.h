// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "UObject/NoExportTypes.h"
#include "Datas/QuestData.h"
#include "QuestManager.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UQuestManager : public UObject
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FQuestUpdate,int);

	FQuestUpdate m_OnQuestUpdate;
	
protected:
	TArray<FQuestDataSpec> m_AryQuestData;
	
public:
	void SetQuestDataFromServer(const FString& strQuest);

	FORCEINLINE TArray<FQuestDataSpec>& GetQuestData()
	{
		return m_AryQuestData;
	}

	bool CompleteQuest(int index);
};
