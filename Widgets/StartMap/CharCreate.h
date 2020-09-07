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

class UEditableText;
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
    UPartSelect* m_ItemSelect;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    UPartSelect* m_PerkSelect;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    UEditableText* m_NameBox;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    UButton* m_BtnContinue;
protected:
    FText m_TextTypedName;
    
    PlayerCreateManager* m_PlManager;

    FTextFormat m_FormatT;
public:
    virtual void NativePreConstruct() override;

    void Init(PlayerCreateManager* plManager);
    void OnVisualChanged(const FCurrentCharData& visual_change);

    UFUNCTION()
    void DecreaseHair();
    UFUNCTION()
    void IncreaseHair();
    UFUNCTION()
    void DecreaseFace();
    UFUNCTION()
    void IncreaseFace();
    UFUNCTION()
    void DecreaseArmor();
    UFUNCTION()
    void IncreaseArmor();
    UFUNCTION()
    void DecreaseItem();
    UFUNCTION()
    void IncreaseItem();
    UFUNCTION()
    void DecreasePerk();
    UFUNCTION()
    void IncreasePerk();

    UFUNCTION()
    void UpdateNameText(const FText& text);//ETextCommit::Type
    UFUNCTION()
    void UpdateNameTextCommit(const FText& text,ETextCommit::Type type);

    UFUNCTION()
    void Continue();
protected:
    FText GetFormatCount(int index, int aryMax);
};
