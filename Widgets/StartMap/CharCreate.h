// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PartSelect.h"
#include "Blueprint/UserWidget.h"
#include "Managers/StartMap/PlayerCreateManager.h"

#include "CharCreate.generated.h"

/**
 * 캐릭터 제작 패널
 */
 
 
UCLASS()
class DIABLOM_API UCharCreate : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UPartSelect* m_HairSelect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UPartSelect* m_FaceSelect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UPartSelect* m_ArmorSelect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UPartSelect* m_WeaponSelect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UPartSelect* m_ItemSelect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UPartSelect* m_PerkSelect;
protected:
	PlayerCreateManager* m_PlManager;

	FTextFormat m_FormatT;
public:
	virtual void NativePreConstruct() override;
	
	void Init(PlayerCreateManager* plManager);
	void OnVisualChanged(const FCurrentCharData& visual_change);

protected:
	FText GetFormatCount(int index,int aryMax); 
};
