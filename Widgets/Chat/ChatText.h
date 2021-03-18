// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"


#include "RichTextBlock.h"
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
	static FString m_RichIDRed;
	static FString m_RichIDBlue;
	static FString m_RichIDYellow;
	static FString m_RichIDOrange;
    static FString m_RichIDGreen;
    static FString m_RichIDPurple;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	URichTextBlock* m_TextChat;
public:
	void SetNormalChat(const FString& chat);

	void SetGachaNotify(const FString& chat);
	
	void Clear();

	FString GetRankColor(const FString& rankNumber);
};