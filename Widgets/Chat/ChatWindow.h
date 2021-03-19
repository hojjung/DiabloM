// My First Hack n Slash

#pragma once

#include "DiabloM.h"

#include "Blueprint/UserWidget.h"
#include "ChatWindow.generated.h"

class UChatText;
class UScrollBox;
/**
 * 
 */
UCLASS()
class DIABLOM_API UChatWindow : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_ChatTotalWindow;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UEditableText* m_MesageEditableText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_MessageScrollBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_SendButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_ChatWindowToggle;
	UPROPERTY()
	TArray<UChatText*> m_AryChatText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UChatText> m_ClassTextWidget;

	int m_nTopIndex;

	float m_fMaxOffset;

	bool m_bIsOpened;
	
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

	UFUNCTION()
	void SetChatWindowOpenClose();
};
