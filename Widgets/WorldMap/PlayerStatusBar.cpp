#include "PlayerStatusBar.h"


#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Characters/PlayerDiabloCharacter.h"

UPlayerStatusBar::UPlayerStatusBar(const FObjectInitializer& objInit):Super(objInit)
{
    m_SelectedCurAttribute=nullptr;
    m_SelectedMaxAttribute=nullptr;
}

void UPlayerStatusBar::Init(ADiabloPlayerController* diaCon)
{
    m_StaminaBar->SetVisibility(ESlateVisibility::Collapsed);
    m_ManaBar->SetVisibility(ESlateVisibility::Collapsed);
    m_RageBar->SetVisibility(ESlateVisibility::Collapsed);
    
    auto* DiaAttri=Cast<UPlayerDiabloAttribute>( diaCon->GetPlayerPawn()->GetAttributeSet());

    m_SelectedBar=m_RageBar;
    m_SelectedCurAttribute=&DiaAttri->Rage;
    m_SelectedMaxAttribute=&DiaAttri->MaxRage;
    
    if(DiaAttri->GetMaxMana()>0)
    {
        m_SelectedBar=m_ManaBar;
        m_SelectedCurAttribute=&DiaAttri->Mana;
        m_SelectedMaxAttribute=&DiaAttri->MaxMana;
    }
    else if(DiaAttri->GetMaxStamina()>0)
    {
        m_SelectedBar=m_StaminaBar;
        m_SelectedCurAttribute=&DiaAttri->Stamina;
        m_SelectedMaxAttribute=&DiaAttri->MaxStamina;
    }

    m_SelectedBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    DiaAttri->m_OnStatChanged.AddUObject(this,&UPlayerStatusBar::SetHealthBarProgressV);
    DiaAttri->m_OnStatChanged.AddUObject(this,&UPlayerStatusBar::SetResourceBarProgressV);

    m_InvenOpenButton->OnClicked.AddDynamic(diaCon,&ADiabloPlayerController::OpenMainMenu);
}

void UPlayerStatusBar::SetHealthBarProgressV(AUnitPawn* pawn)
{
    m_HpBar->SetProgressValue(pawn->GetHpPercentOne());
}

void UPlayerStatusBar::SetResourceBarProgressV(AUnitPawn* pawn)
{
    float Per=m_SelectedCurAttribute->GetCurrentValue()/m_SelectedMaxAttribute->GetCurrentValue();
    Per=FMath::Clamp(Per,0.f,1.f);
    m_SelectedBar->SetProgressValue(Per);
}
