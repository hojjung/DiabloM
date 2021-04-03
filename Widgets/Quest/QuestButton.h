// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CommonElement/ImageAndText.h"
#include "Widgets/CommonElement/MaterialProgressBar.h"
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextQuestName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_ProgressBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextQuestGauge;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnComplete;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_TextImgComplete;

	int m_nIndex;

	FQuestDataSpec* m_CurrentSpec;

	float m_fDeltaCounter;

	bool m_bChargeUpgrade;
	
public:
	void Init(int index,FQuestDataSpec* dataSpecPTr);

	void UpdateQuestWidget();

	UFUNCTION()
	void OnClickButton();

	bool TryComplete(int index);


	UFUNCTION()
    void ChargeStart();

	UFUNCTION()
    void ChargeEnd();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	
};
