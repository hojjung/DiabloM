// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Datas/GachaDataTable.h"

#include "GachaInfoElement.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UGachaInfoElement : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextGachaName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextGachaPercent;
	
public:
	void SetInfoText(const FGachaTableRow* gachaTable,float totalPercent);
};
