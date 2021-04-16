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
	
	void ReqeustPopupText(FString str);

	void ReqeustPopupText(FText str);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UEditableText* m_Nickname;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnConfirm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextAPI;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_CanvasNotification;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextNotification;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnNotificationConfirm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_CanvasNicknamePanel;
	FDelegateHandle m_Dele;

	bool m_bRequestLock = false;
public:
	virtual void NativeOnInitialized() override;


	UFUNCTION()
	void TryConfirm();

	UFUNCTION()
	void ConfirmNoti();

	void ShowNotification(const FString& str);

	void HideNicknameSet();

	void ShowNicknameSet();

	UFUNCTION()
	void OnTextChanged(const FText& text);
};
