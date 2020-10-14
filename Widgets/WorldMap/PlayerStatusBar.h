// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/DiabloPlayerController.h"
#include "DefaultMenu/MaterialProgressBar.h"

#include "PlayerStatusBar.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPlayerStatusBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerStatusBar(const FObjectInitializer& objInit);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_InvenOpenButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextHp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_HpBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_StaminaBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_ManaBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMaterialProgressBar* m_RageBar;

	TWeakObjectPtr<UMaterialProgressBar> m_SelectedBar;

	const FGameplayAttributeData* m_SelectedCurAttribute;
	
	const FGameplayAttributeData* m_SelectedMaxAttribute;
	
	FTextFormat m_HpFormat;
public:
	void Init(ADiabloPlayerController* diaCon);

	void SetHealthBarProgressV(AUnitPawn* pawn);

	void SetResourceBarProgressV(AUnitPawn* pawn);

};
