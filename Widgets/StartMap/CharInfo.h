// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Image.h"
#include "TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "SaveLoad/SaveCharacterStatus.h"

#include "CharInfo.generated.h"

/**
 * 로딩한 캐릭터 버튼
 */
class UCharInfo;
DECLARE_DELEGATE_TwoParams(FOnSelected,int,UCharInfo*);
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
