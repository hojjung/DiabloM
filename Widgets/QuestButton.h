// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Datas/QuestData.h"

#include "QuestButton.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UQuestButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	int m_nIndex;

	TWeakObjectPtr<FQuestDataSpec> m_CurrentSpec;
	
public:
	void Init(int index,FQuestDataSpec* dataSpecPTr);
	
};
