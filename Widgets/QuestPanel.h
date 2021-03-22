#pragma once

#include "CoreMinimal.h"
#include "QuestButton.h"
#include "WrapBox.h"
#include "Blueprint/UserWidget.h"
#include "QuestPanel.generated.h"

UCLASS()
class DIABLOM_API UQuestPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UQuestButton> m_ClassQuest;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* m_GridPanel;
	UPROPERTY()
	TArray<UQuestButton*> m_AryQuestBtns;
	
public:
	virtual void NativeOnInitialized() override;

	
};
