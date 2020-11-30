 #pragma once

#include "DiabloM.h"
#include "CharCreate.h"
#include "CharSelect.h"
#include "Blueprint/UserWidget.h"
#include "StartMenuCanvas.generated.h"

 class APlayerCreateController;
/**
 * StartMenuMaiNCanvas
 */
UCLASS()
class DIABLOM_API UStartMenuCanvas : public UUserWidget
{
	GENERATED_BODY()
    
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    UCharCreate* m_CharCreate;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    UCharSelect* m_CharSelect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnExit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPlay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnBackCancel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnOption;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_BG;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextTitle;
	
protected:
	bool m_bIsCreationOpened;

	bool m_bIsSelectionOpened;

private:
	UFUNCTION()
    void ShowMainMenu();

	UFUNCTION()
    void HideMainMenu();
	
public:
    void InitStartMenu();

    bool HaveEmptySlot();

	UFUNCTION()
	void PlayPressed();
	
	

    UFUNCTION()
    void CreationStart();

    UFUNCTION()
    void CreationEnd();

	UFUNCTION()
	void ExitGame();

	UFUNCTION()
	void BackCancel();
	

	friend APlayerCreateController;
};
