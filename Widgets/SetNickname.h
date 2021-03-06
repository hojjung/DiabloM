// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"



#include "Button.h"
#include "CanvasPanel.h"
#include "EditableText.h"
#include "TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "SetNickname.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API USetNickname : public UUserWidget
{
	GENERATED_BODY()

public:
	~USetNickname();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UEditableText* m_Nickname;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnConfirm;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_CanvasNotification;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextNotification;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnNotificationConfirm;

	FDelegateHandle m_Dele;
public:
	virtual void NativeOnInitialized() override;


	UFUNCTION()
	void TryConfirm();

	UFUNCTION()
	void ConfirmNoti();

	void ShowNotification(FString& str);
};
