// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "PopupTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPopupTextWidget : public UUserWidget
{
	GENERATED_BODY()
	//anim
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextPopup;
public:
	void ShowText(const FText& txt);

};
