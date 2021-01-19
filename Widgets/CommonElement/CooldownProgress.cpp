#include "CooldownProgress.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"

UCooldownProgress::UCooldownProgress(const FObjectInitializer& obj):Super(obj)
{
    m_ColorText =FLinearColor::White;
    m_fFontSize=22.f;
}

void UCooldownProgress::SetFontSize(float size)
{
    m_TextCooldown->SetColorAndOpacity(m_ColorText);
    FSlateFontInfo Font =m_TextCooldown->Font;
    Font.Size=size;
    m_TextCooldown->SetFont(Font);
    
    m_fFontSize=size;
}

void UCooldownProgress::NativePreConstruct()
{
    Super::NativePreConstruct();
    SetFontSize(m_fFontSize);
}

void UCooldownProgress::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    CreateRenderMat();

    m_TextCooldown->SetText(FText());
}

void UCooldownProgress::CreateRenderMat()
{
    m_MatInst = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this,m_MatTemplate,"MatCreated");
    
    if(m_MatInst)
    {
        m_ImageCooldown->SetBrushFromMaterial(m_MatInst);
    }
}

void UCooldownProgress::StartCooldown()
{
    m_TextCooldown->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_ImageCooldown->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UCooldownProgress::SetCooldownProgress(float currentCD, float maxCd)
{
    if(currentCD<=0.f)
    {
        m_TextCooldown->SetVisibility(ESlateVisibility::Hidden);
        m_ImageCooldown->SetVisibility(ESlateVisibility::Hidden);
        
        return;
    }
    float New =  UDiaBlueprintFunctionLibrary::SetFloatPrecision(currentCD,1);
    m_TextCooldown->SetText(FText::AsNumber(New));    
    m_MatInst->SetScalarParameterValue("Value",currentCD/maxCd);
}

