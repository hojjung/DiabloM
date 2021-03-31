#pragma once

#include "DiabloM.h"
#include "PlayFabClientDataModels.h"
#include "RichTextBlock.h"
#include "Blueprint/UserWidget.h"
#include "RankingElement.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API URankingElement : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	URichTextBlock* m_TextRankPosition;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextRankDisplayName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextStageLevel;
	
public:
	void SetRankElement(const PlayFab::ClientModels::FPlayerLeaderboardEntry& rankInfo);
};
