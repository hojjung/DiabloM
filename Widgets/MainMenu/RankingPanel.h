#pragma once

#include "DiabloM.h"
#include "PlayFabClientDataModels.h"
#include "RankingElement.h"
#include "ScrollBox.h"
#include "Blueprint/UserWidget.h"
#include "RankingPanel.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API URankingPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_Verti;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	URankingElement* m_SelfRanking;
	UPROPERTY(EditAnywhere)
	TSubclassOf<URankingElement> m_ClassRanking;
	UPROPERTY()
	TArray<URankingElement*> m_AryRankElement;

public:
	virtual void NativeOnInitialized() override;

	void UpdateTotalRankingPanel(const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& totalRanks);

	void UpdateSelfRanking(const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& serverRank);
};


