#pragma once
#include "CoreMinimal.h"
#include "Button.h"
#include "TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "StageBtn.generated.h"

UCLASS()
class DIABLOM_API UStageBtn : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TxtStageName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TxtStageClear;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_StageBtn;
	UPROPERTY()
	int m_nIndex;
	
	FTextFormat m_FormatStageName;
public:
	void SetUnlock(bool b);
	
	void SetCurrent();
	
	void Init(bool dgUnlock,int index);

	UFUNCTION()
	void MoveToDungeon();
};
