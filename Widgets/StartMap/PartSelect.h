// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "PartSelect.generated.h"

UCLASS()
class DIABLOM_API UPartSelect : public UUserWidget
{
	GENERATED_BODY()

	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextTypeName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* m_TextTypeCount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnLeft;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRight;
	
protected:
	virtual void NativePreConstruct() override;

};
