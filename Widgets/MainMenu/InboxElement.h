// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"



#include "Button.h"
#include "Image.h"
#include "TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Managers/InboxManager.h"

#include "InboxElement.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UInboxElement : public UUserWidget
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnBtnClaimClicked,int);

	FOnBtnClaimClicked m_OnClaim;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextMailName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextMailDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextExpireTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgItemIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextItemAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClaim;
public:
	UPROPERTY()
	int m_nIndex;

	virtual void NativeOnInitialized() override;
	
	void SetInboxSpec(const FInboxSpec& element);

	UFUNCTION()
	void OnClick();
};
