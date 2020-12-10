#pragma once

#include "DiabloM.h"
#include "Image.h"
#include "ObjectMacros.h"
#include "Blueprint/UserWidget.h"
#include "CooldownProgress.generated.h"


class UMaterialInstanceDynamic;


UCLASS()
class DIABLOM_API UCooldownProgress : public UUserWidget
{
	GENERATED_BODY()

public:
    UCooldownProgress(const FObjectInitializer& obj);
    virtual void NativePreConstruct() override;
    virtual void NativeOnInitialized() override;
protected:
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    UMaterialInterface* m_MatTemplate;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    UImage* m_ImageCooldown;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* m_TextCooldown;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FLinearColor m_ColorText;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float m_fFontSize;
    
protected:
    UPROPERTY(Transient)
    UMaterialInstanceDynamic* m_MatInst;

public:
    UFUNCTION(BlueprintCallable)
    void CreateRenderMat();
    UFUNCTION(BlueprintCallable)
    void StartCooldown();
    UFUNCTION(BlueprintCallable)
    void SetCooldownProgress(float currentCD,float maxCd);
};
