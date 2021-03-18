// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "ChatText.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UChatText : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextChat;
public:
	void SetNormalChat(const FString& chat);
};
