// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Button.h"
#include "TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CommonElement/ImageAndText.h"

#include "PVPPanel.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPVPPanel : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextPlayerID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextOtherPlayerID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnMatching;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_TextMMR;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_TextWinLose;

public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	void RequestMatch();

	void OnNicknameFound(const FString otherPlayerName);

	void OnMatchFail();

	void UpdateStatusText();
};

