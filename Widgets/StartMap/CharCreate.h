#pragma once

#include "DiabloM.h"
#include "PartSelect.h"
#include "Blueprint/UserWidget.h"
#include "Managers/StartMap/PlayerCreateManager.h"
#include "CharCreate.generated.h"

class UCharSelect;
class UStartMenuCanvas;
class UEditableText;

UCLASS()
class DIABLOM_API UCharCreate : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY()
    UCharSelect* m_CharSelect;
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
    UPROPERTY()
    UPlayerCreateManager* m_PlManager;

    FTextFormat m_FormatT;
public:
    virtual void NativePreConstruct() override;

    void Init(UPlayerCreateManager* plManager);
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

    UFUNCTION()
    void BackCancel();

    void StartCreation();
protected:
    FText GetFormatCount(int index, int aryMax);


    friend UStartMenuCanvas;
};
