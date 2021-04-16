// My First Hack n Slash

#pragma once

#include "DailyRewardElement.h"
#include "DiabloM.h"
#include "UniformGridPanel.h"
#include "Blueprint/UserWidget.h"
#include "DailyRewardPanel.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UDailyRewardPanel : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDailyRewardElement> m_ClassDailyElement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUniformGridPanel* m_Grid;
	UPROPERTY()
	TArray<UDailyRewardElement*> m_AryElements;

protected:
	virtual void NativeOnInitialized() override;

	void CreateDailyButton(int maxCount);

	UFUNCTION()
	void ClaimReward();
};
