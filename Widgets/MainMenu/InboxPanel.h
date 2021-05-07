// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "InboxElement.h"
#include "VerticalBox.h"
#include "Blueprint/UserWidget.h"

#include "InboxPanel.generated.h"

class UScrollBox;
/**
 * 
 */
UCLASS()
class DIABLOM_API UInboxPanel : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInboxElement> m_ClassInboxElement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextMailCount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextLoading;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClaimAll;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_VerticalInboxList;
	UPROPERTY()
	TArray<UInboxElement*> m_AryInboxElement;

public:
	virtual void NativeOnInitialized() override;
	
	void CreateInboxElement(int count);

	void SetInboxElement(const TArray<FInboxSpec>& aryInboxSpecs);

	virtual void SetVisibility(ESlateVisibility InVisibility) override;

	UFUNCTION()
	void ClaimAll();

	void ClaimInbox(int index);
};
