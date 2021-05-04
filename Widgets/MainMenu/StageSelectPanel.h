#pragma once

#include "CoreMinimal.h"

#include "MagicDgPanel.h"
#include "ScrollBox.h"
#include "StageBtn.h"
#include "VerticalBox.h"
#include "Blueprint/UserWidget.h"
#include "StageSelectPanel.generated.h"

class UPVPPanel;
/**
 * 
 */
UCLASS()
class DIABLOM_API UStageSelectPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnShowNormalDg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnShowMagicDg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnShowStatDg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnShowAcceDg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPVP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnBack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_StageTypeSelectVert;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMagicDgPanel* m_DgMagicPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_DgStageBtnListVert;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_DgAcceStageBtnListVert;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_DgStatBtnListVert;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UPVPPanel* m_DgPVPPanel;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UStageBtn> m_ClassStageBtn;
	UPROPERTY()
	TArray<UStageBtn*> m_AryStageBtn;

protected:
	void InitNormalDungeon();
	virtual void NativeOnInitialized() override;

	void UpdateBtnUI(int index);

public:
	UFUNCTION()
	void OpenNormalDg();
	UFUNCTION()
    void OpenGoldDg();
	UFUNCTION()
	void OpenStatDg();
	UFUNCTION()
    void OpenAcceDg();
	UFUNCTION()
    void OpenPVPDg();
	UFUNCTION()
	void Back();
};


