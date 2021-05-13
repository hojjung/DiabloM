#pragma once

#include "CoreMinimal.h"

#include "MagicDgPanel.h"
#include "PetDgPanel.h"
#include "ScrollBox.h"
#include "StageBtn.h"
#include "VerticalBox.h"
#include "WeaponDgPanel.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CommonElement/ImageAndText.h"

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
	UImageAndText* m_ImgTxtDgTicket;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnShowNormalDg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnShowPetDg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnShowMagicDg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnShowWeaponDg;
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
	UPetDgPanel* m_DgPetPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWeaponDgPanel* m_DgWeaponPanel;
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
	void OpenPetDg();
	UFUNCTION()
    void OpenWeaponDg();
	UFUNCTION()
    void OpenPVPDg();
	UFUNCTION()
	void Back();

	void UpdateDgKey(int kV);
};


