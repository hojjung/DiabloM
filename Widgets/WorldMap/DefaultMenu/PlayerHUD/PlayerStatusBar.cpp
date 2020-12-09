#include "PlayerStatusBar.h"

#include "DiaMonsterInfo.h"
#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Widgets/WorldMap/DefaultMenu/MaterialProgressBar.h"

UPlayerStatusBar::UPlayerStatusBar(const FObjectInitializer& objInit): Super(objInit)
{
    m_SelectedCurAttribute = nullptr;
    m_SelectedMaxAttribute = nullptr;

    m_HpFormat = FTextFormat::FromString("{0}/{1}");
}

void UPlayerStatusBar::Init(ADiabloPlayerController* diaCon)
{
    m_StaminaBar->SetVisibility(ESlateVisibility::Collapsed);
    m_ManaBar->SetVisibility(ESlateVisibility::Collapsed);
    m_RageBar->SetVisibility(ESlateVisibility::Collapsed);

    UPlayerDiabloAttribute* DiaAttri = Cast<UPlayerDiabloAttribute>(diaCon->GetPlayerPawn()->GetAttributeSet());
    m_PlayerComp = Cast<UPlayerDiabloAbilitySystemComp>(diaCon->GetPlayerPawn()->GetAbilitySystemComponent());

    m_SelectedBar = m_RageBar;
    m_SelectedCurAttribute = &DiaAttri->Rage;
    m_SelectedMaxAttribute = &DiaAttri->MaxRage;
    PRINTF("RageSet");
    if (DiaAttri->GetMaxMana() > 0)
    {
        m_SelectedBar = m_ManaBar;
        m_SelectedCurAttribute = &DiaAttri->Mana;
        m_SelectedMaxAttribute = &DiaAttri->MaxMana;
        PRINTF("Nope Mana Set");
    }
    else if (DiaAttri->GetMaxStamina() > 0)
    {
        m_SelectedBar = m_StaminaBar;
        m_SelectedCurAttribute = &DiaAttri->Stamina;
        m_SelectedMaxAttribute = &DiaAttri->MaxStamina;
        PRINTF("Nope Stamina Set");
    }

    m_SelectedBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    DiaAttri->m_OnStatChanged.AddUObject(this, &UPlayerStatusBar::SetHealthBarProgressV);
    DiaAttri->m_OnStatChanged.AddUObject(this, &UPlayerStatusBar::SetResourceBarProgressV);

    m_InvenOpenButton->OnClicked.AddDynamic(diaCon, &ADiabloPlayerController::OpenMainMenu);

    SetHealthBarProgressV(diaCon->GetPlayerPawn());
    SetResourceBarProgressV(diaCon->GetPlayerPawn());
    //
    m_PlayerComp->m_OnSkillLevelChanged.AddUObject(this,&UPlayerStatusBar::UpdateSkill);
    m_PlayerComp->m_OnSkillChanged.AddUObject(this,&UPlayerStatusBar::UpdateSkillBtn);
    

    for(int i=0; i<m_SkillUseCanvas->m_AryButtons.Num();i++)
    {
        m_SkillUseCanvas->m_AryButtons[i]->Init(m_PlayerComp.Get(),i);
    }

    m_Minimap->Init();
}

void UPlayerStatusBar::SetHealthBarProgressV(AUnitPawn* pawn)
{
    m_HpBar->SetProgressValue(pawn->GetHpPercentOne());

    FFormatOrderedArguments Args;
    float CH = pawn->GetHealth();
    float MH = pawn->GetMaxHealth();
    Args.Add(UDiaBlueprintFunctionLibrary::GetAlphabetText(CH));
    Args.Add(UDiaBlueprintFunctionLibrary::GetAlphabetText(MH));

    m_TextHp->SetText(FText::Format(m_HpFormat,Args));
}

void UPlayerStatusBar::SetResourceBarProgressV(AUnitPawn* pawn)
{
    float Per = m_SelectedCurAttribute->GetCurrentValue() / m_SelectedMaxAttribute->GetCurrentValue();
    Per = FMath::Clamp(Per, 0.f, 1.f);
    m_SelectedBar->SetProgressValue(Per);
}

void UPlayerStatusBar::UpdateMinimap(UMaterialInterface* material_interface)
{
    m_Minimap->SetMinimapMat(material_interface);
}

void UPlayerStatusBar::HideMinimap()
{
    m_Minimap->HideMinimap();
}

void UPlayerStatusBar::ShowMinimap()
{
    m_Minimap->ShowMinimap();
}

void UPlayerStatusBar::ShowPlayerHUD()
{
     m_InvenOpenButton->SetVisibility(ESlateVisibility::Visible);
     m_TextHp->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
     m_HpBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
     m_SelectedBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
     m_Minimap->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
     m_InteractButton->SetVisibility(ESlateVisibility::Visible);
     m_PotionButton->SetVisibility(ESlateVisibility::Visible);
     m_SkillMenuOpenButton->SetVisibility(ESlateVisibility::Visible);
     m_SkillUseCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
     //m_DiaMonInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
     m_ExpBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UPlayerStatusBar::HidePlayerHUD()
{
    m_InvenOpenButton->SetVisibility(ESlateVisibility::Hidden);
    m_TextHp->SetVisibility(ESlateVisibility::Hidden);
    m_HpBar->SetVisibility(ESlateVisibility::Hidden);
    m_SelectedBar->SetVisibility(ESlateVisibility::Hidden);
    m_Minimap->SetVisibility(ESlateVisibility::Hidden);
    m_InteractButton->SetVisibility(ESlateVisibility::Hidden);
    m_PotionButton->SetVisibility(ESlateVisibility::Hidden);
    m_SkillMenuOpenButton->SetVisibility(ESlateVisibility::Hidden);
    m_SkillUseCanvas->SetVisibility(ESlateVisibility::Hidden);
   // m_DiaMonInfo->SetVisibility(ESlateVisibility::Hidden);
    m_ExpBar->SetVisibility(ESlateVisibility::Hidden);
}


void UPlayerStatusBar::UpdateSkill(FSkillDataSpec* spec)
{
    if(spec->m_nEquipIndex<0)
    {
        return;
    }
    int Index=spec->m_nEquipIndex;
    m_PlayerComp.Get()->UnequipSkill(spec);
    spec->m_nEquipIndex=Index;
    m_PlayerComp.Get()->EquipSkill(spec);
}

void UPlayerStatusBar::UpdateSkillBtn(FSkillDataSpec* spec, int index)
{
    m_SkillUseCanvas->m_AryButtons[index]->ClearSkillSpec();
}
