#pragma once

#include "CoreMinimal.h"
#include "ScrollBox.h"
#include "StageBtn.h"
#include "VerticalBox.h"
#include "Blueprint/UserWidget.h"
#include "StageSelectPanel.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UStageSelectPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnShowNormalDg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnShowGoldDg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_DgGoldStageBtnListVert;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_StageBtnListVert;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UStageBtn> m_ClassStageBtn;
	UPROPERTY()
	TArray<UStageBtn*> m_AryStageBtn;

protected:
	virtual void NativeOnInitialized() override;

	void UpdateBtnUI(int index);

public:
	UFUNCTION()
	void OpenNormalDg();
	UFUNCTION()
    void OpenGoldDg();
};


