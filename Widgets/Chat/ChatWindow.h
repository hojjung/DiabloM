// My First Hack n Slash

#pragma once

#include "ChatText.h"
#include "DiabloM.h"
#include "ScrollBox.h"
#include "Blueprint/UserWidget.h"
#include "ChatWindow.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UChatWindow : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UChatText> m_ClassTextWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UEditableText* m_MesageEditableText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_MessageScrollBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_SendButton;
	UPROPERTY()
	TArray<UChatText*> m_AryChatText;

	int m_nTopIndex;

	float m_fMaxOffset;
protected:
	UChatText* GetTopText();
	
	virtual void NativeOnInitialized() override;

	void ClearChat();

	
	
public:
	UFUNCTION()
	void OnReceiveTotalChatList(const FString& chat);
	
	UFUNCTION()
	void SendText();

	UFUNCTION()
	void AddTextWidget(const FString& chat);

	UFUNCTION()
	void OnChatTextChanged(const FText& text);
};
