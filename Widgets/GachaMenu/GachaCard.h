// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "GachaCard.generated.h"

struct FGachaTableRow;

UCLASS()
class DIABLOM_API UGachaCard : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextTierName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgGlow;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgIcon;
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* m_AnimFlash;

public:
	void SetVisual(FGachaTableRow* tableData);

	void ShowSpecialEffect();

	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;
};
