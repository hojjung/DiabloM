#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "GameExitWindow.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UGameExitWindow : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCancel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnExitGame;

protected:
	bool m_bIsOpened;
	
public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnBackkeyPressed();
	
	UFUNCTION()
	void Cancel();

	UFUNCTION()
    void Exit();
};
