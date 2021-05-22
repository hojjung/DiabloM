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
	USizeBox* m_BackBtnParent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_RankNormalPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_RankPVPPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* m_RankOpenVert; 
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_VertiNormal;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_VertiPVP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	URankingElement* m_SelfRanking;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	URankingElement* m_PVPSelfRanking;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnOpenNormalRank;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnOpenPVPRank;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnBack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRankInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_RankInfoPanel;
	//
	UPROPERTY(EditAnywhere)
	TSubclassOf<URankingElement> m_ClassRanking;
	UPROPERTY()
	TArray<URankingElement*> m_AryStageRankElement;
	UPROPERTY()
	TArray<URankingElement*> m_AryPVPStageRankElement;

protected:
	//normal stage
	void SetStageRank();
	
	void UpdateTotalStageRankingPanel(const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& totalRanks);

	void UpdateSelfStageRanking(const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& serverRank);

	//pvp
	void SetPVPRank();

	void UpdatePVPStageRankingPanel(const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& totalRanks);

	void UpdateSelfPVPRanking(const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& serverRank);
	
public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	void CloseRankPanels();
	UFUNCTION()
	void OpenStageRank();
	UFUNCTION()
	void OpenPVPRank();

	UFUNCTION()
	void OpenRankInfo();
};


