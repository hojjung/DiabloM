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

void UPlayerStatusBar::EquipFromSaveData()
{
    for(FSkillDataSpec& Skill : m_PlayerComp->m_AryBaseSkill)
    {
        if(Skill.m_nEquipIndex>-1)
        {
            m_SkillUseCanvas->m_ArySkillButtons[Skill.m_nEquipIndex]->SetSkillSpec(&Skill);
        }
    }

    for(FSkillDataSpec& Skill : m_PlayerComp->m_AryPowerSkill)
    {
        if(Skill.m_nEquipIndex>-1)
        {
            m_SkillUseCanvas->m_ArySkillButtons[Skill.m_nEquipIndex]->SetSkillSpec(&Skill);
        }
    }

    
    for(FSkillDataSpec& Skill : m_PlayerComp->m_AryDefensvieSkill)
    {
        if(Skill.m_nEquipIndex>-1)
        {
            m_SkillUseCanvas->m_ArySkillButtons[Skill.m_nEquipIndex]->SetSkillSpec(&Skill);
        }
    }

    for(FSkillDataSpec& Skill : m_PlayerComp->m_AryDefensvieSkill)
    {
        if(Skill.m_nEquipIndex>-1)
        {
            m_SkillUseCanvas->m_ArySkillButtons[Skill.m_nEquipIndex]->SetSkillSpec(&Skill);
        }
    }

    
    for(FSkillDataSpec& Skill : m_PlayerComp->m_ArySpecialSkill)
    {
        if(Skill.m_nEquipIndex>-1)
        {
            m_SkillUseCanvas->m_ArySkillButtons[Skill.m_nEquipIndex]->SetSkillSpec(&Skill);
        }
    }

    for(FSkillDataSpec& Skill : m_PlayerComp->m_AryUltimateSkill)
    {
        if(Skill.m_nEquipIndex>-1)
        {
            m_SkillUseCanvas->m_ArySkillButtons[Skill.m_nEquipIndex]->SetSkillSpec(&Skill);
        }
    }
}

void UPlayerStatusBar::StopAutoPlay()
{
    m_AutoPlayButton->SetCheckedState(ECheckBoxState::Unchecked);
}

void UPlayerStatusBar::Init(ADiabloPlayerController* diaCon,UMainCanvas* mainCanvas)
{
    m_StaminaOverlay->SetVisibility(ESlateVisibility::Collapsed);
    m_ManaOverlay->SetVisibility(ESlateVisibility::Collapsed);
    m_RageOverlay->SetVisibility(ESlateVisibility::Collapsed);

    UPlayerDiabloAttribute* DiaAttri = Cast<UPlayerDiabloAttribute>(diaCon->GetPlayerPawn()->GetAttributeSet());
    m_PlayerComp = Cast<UPlayerDiabloAbilitySystemComp>(diaCon->GetPlayerPawn()->GetAbilitySystemComponent());

    m_SelectedOverlay = m_RageOverlay;
    m_SelectedResourceBar = m_RageBar;
    m_SelectedResourceText = m_RageText;
    m_SelectedCurAttribute = &DiaAttri->Rage;
    m_SelectedMaxAttribute = &DiaAttri->MaxRage;
    PRINTF("RageSet");
    if (DiaAttri->GetMaxMana() > 0)
    {
        m_SelectedOverlay = m_ManaOverlay;
        m_SelectedResourceBar = m_ManaBar;
        m_SelectedResourceText = m_ManaText;
        m_SelectedCurAttribute = &DiaAttri->Mana;
        m_SelectedMaxAttribute = &DiaAttri->MaxMana;
        PRINTF("Nope Mana Set");
    }
    else if (DiaAttri->GetMaxStamina() > 0)
    {
        m_SelectedOverlay = m_StaminaOverlay;
        m_SelectedResourceBar = m_StaminaBar;
        m_SelectedResourceText = m_StaminaText;
        m_SelectedCurAttribute = &DiaAttri->Stamina;
        m_SelectedMaxAttribute = &DiaAttri->MaxStamina;
        PRINTF("Nope Stamina Set");
    }

    m_SelectedOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    DiaAttri->m_OnStatChanged.AddUObject(this, &UPlayerStatusBar::SetHealthBarProgressV);
    DiaAttri->m_OnStatChanged.AddUObject(this, &UPlayerStatusBar::SetResourceBarProgressV);

    m_InvenOpenButton->OnClicked.AddDynamic(diaCon->GetMainCanvas(), &UMainCanvas::OpenMainMenu);

    SetHealthBarProgressV(diaCon->GetPlayerPawn());
    SetResourceBarProgressV(diaCon->GetPlayerPawn());
    //
    m_PlayerComp->m_OnSkillLevelChanged.AddUObject(this,&UPlayerStatusBar::UpdateSkill);
    m_PlayerComp->m_OnSkillChanged.AddUObject(this,&UPlayerStatusBar::UpdateSkillBtn);
    

    for(int i=0; i<m_SkillUseCanvas->m_ArySkillButtons.Num();i++)
    {
        m_SkillUseCanvas->m_ArySkillButtons[i]->Init(diaCon->GetPlayerPawn(),m_PlayerComp.Get(),i);
    }
    m_SkillUseCanvas->m_PotionButton->Init(diaCon->GetPlayerPawn(),m_PlayerComp.Get());

    m_Minimap->Init();
    //
    EquipFromSaveData();

    m_AutoPlayButton->OnCheckStateChanged.AddDynamic(this,&UPlayerStatusBar::SetAutoPlay);
    StopAutoPlay();

    diaCon->GetPlayerPawn()->m_OnMove.AddUObject(this,&UPlayerStatusBar::StopAutoPlay);

    m_SkillUseCanvas->m_InteractButton->OnClicked.AddDynamic(mainCanvas,&UMainCanvas::Interaction);

   // m_SkillUseCanvas->m_PotionButton->OnClicked.AddDynamic(mainCanvas,&UMainCanvas::DrinkPotion);

    m_SkillMenuOpenButton->OnClicked.AddDynamic(mainCanvas,&UMainCanvas::OpenSkillMenu);
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
    float CR =m_SelectedCurAttribute->GetCurrentValue();
    
    float MR = m_SelectedMaxAttribute->GetCurrentValue();
    
    float Per = CR / MR;
    
    Per = FMath::Clamp(Per, 0.f, 1.f);
    
    m_SelectedResourceBar->SetProgressValue(Per);

    FFormatOrderedArguments Args;

    Args.Add(UDiaBlueprintFunctionLibrary::GetAlphabetText(CR));
    
    Args.Add(UDiaBlueprintFunctionLibrary::GetAlphabetText(MR));

    m_SelectedResourceText->SetText(FText::Format(m_HpFormat,Args));
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
     m_SelectedOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
     m_Minimap->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
     m_SkillUseCanvas->m_InteractButton->SetVisibility(ESlateVisibility::Visible);
     m_SkillUseCanvas->m_PotionButton->SetVisibility(ESlateVisibility::Visible);
     m_SkillMenuOpenButton->SetVisibility(ESlateVisibility::Visible);
     m_SkillUseCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
     if(Cast<APlayerDiabloCharacter>( m_PlayerComp->GetOwner())->GetFocusedTargetWeakPtr().Get())
     {
        m_DiaMonInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible); //이것은 타겟팅용이기때문에 무작정켜지면 안된다.
     }
     m_ExpBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
     m_AutoPlayButton->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerStatusBar::HidePlayerHUD()
{
    m_InvenOpenButton->SetVisibility(ESlateVisibility::Hidden);
    m_TextHp->SetVisibility(ESlateVisibility::Hidden);
    m_HpBar->SetVisibility(ESlateVisibility::Hidden);
    m_SelectedOverlay->SetVisibility(ESlateVisibility::Hidden);
    m_Minimap->SetVisibility(ESlateVisibility::Hidden);
    m_SkillUseCanvas->m_InteractButton->SetVisibility(ESlateVisibility::Hidden);
    m_SkillUseCanvas->m_PotionButton->SetVisibility(ESlateVisibility::Hidden);
    m_SkillMenuOpenButton->SetVisibility(ESlateVisibility::Hidden);
    m_SkillUseCanvas->SetVisibility(ESlateVisibility::Hidden);
    m_DiaMonInfo->SetVisibility(ESlateVisibility::Hidden);
    m_ExpBar->SetVisibility(ESlateVisibility::Hidden);
    m_AutoPlayButton->SetVisibility(ESlateVisibility::Hidden);
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
    m_SkillUseCanvas->m_ArySkillButtons[index]->ClearSkillSpec();
}

void UPlayerStatusBar::SetAutoPlay(bool isAuto)
{
    ADiabloPlayerController::Get->GetPlayerPawn()->SetAutoPlay(isAuto);
}
