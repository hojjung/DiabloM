// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"


#include "TextBlock.h"
#include "VerticalBox.h"
#include "Blueprint/UserWidget.h"
#include "LogText.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API ULogText : public UUserWidget
{
	GENERATED_BODY()

public:
	static ULogText* Get;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UVerticalBox* m_VerticalHolder;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nMaxCount = 5;
	UPROPERTY()
	int m_nCurrentCount =0;
	UPROPERTY()
	TArray<UTextBlock*> m_AryText;
	
public:
	virtual void NativeOnInitialized() override;
	
	void PrintText(FString str);

	virtual void NativeDestruct() override;
};
