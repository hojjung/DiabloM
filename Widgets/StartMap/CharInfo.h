#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "SaveLoad/SaveCharacterStatus.h"
#include "CharInfo.generated.h"

class UCharInfo;

DECLARE_DELEGATE_OneParam(FOnSelected,int);

UCLASS()
class DIABLOM_API UCharInfo : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgClassBadge;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextArea;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnDelete;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnSelect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgFocus;

	int m_nSlotIndex=-1;
	
public:
	FOnSelected m_OnSelect;

	void Init(const USaveCharacterStatus* char_stats);

	
public:
	UFUNCTION()
	void SelectSlot();
	UFUNCTION()
    void DeselectSlot();
	UFUNCTION()
    void DeleteSlot();
};
