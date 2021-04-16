// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "DailyRewardElement.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UDailyRewardElement : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClaimReward;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextDay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextGemStones;

	UPROPERTY()
	int m_nGemStones;
public:
	void SetDailyRewardElement(int dday,int gemstoneAmount);

	UFUNCTION()
	void Claimed();
	
	void SetClaimAble();
};
