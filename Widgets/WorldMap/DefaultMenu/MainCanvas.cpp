#include "MainCanvas.h"

#include "MaterialProgressBar.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Managers/DiabloGameInstance.h"
#include "PlayerHUD/DiaMonsterInfo.h"
#include "PlayerHUD/PlayerStatusBar.h"
#include "Widgets/WorldMap/DefaultMenu/DefaultMenu.h"


UMainCanvas::UMainCanvas(const FObjectInitializer& objInit):Super(objInit)
{
    m_HpFormat=FTextFormat::FromString("{0}/{1}");
}

void UMainCanvas::OpenMainMenu()
{
    m_bIsOpened=true;
    m_MainMenu->OpenMainMenu();
    m_PlayerStatusBar->HidePlayerHUD();
}

void UMainCanvas::CloseMainMenu()
{
    m_bIsOpened=false;
    m_MainMenu->CloseMainMenu();
    m_PlayerStatusBar->ShowPlayerHUD();
}

void UMainCanvas::OpenSkillMenu()
{
    OpenMainMenu();
    m_MainMenu->OpenSkillPanel();
}

void UMainCanvas::Interaction()
{
    m_PlayerPawn->InteractWithTarget();
}

void UMainCanvas::OnAttackPressStart()
{
    m_PlayerPawn->OnAttackPressed();
}

void UMainCanvas::OnAttackPressEnd()
{
    m_PlayerPawn->OnAttackRelease();
}


void UMainCanvas::OpenSetting()
{
}

void UMainCanvas::CloseSetting()
{
    
}

void UMainCanvas::TestOpenDungeon()
{
    UDiabloGameInstance::Get->CreateDungeon();
}


void UMainCanvas::TestSaveAll()
{
    UDiabloGameInstance::Get->SaveAllPlayerInfo();
}

void UMainCanvas::DrinkPotion()
{
    m_PlayerPawn->DrinkPotion();
    //need cd
}

void UMainCanvas::TestGoBackMenu()
{
    m_PlayerCon->BackToSelectMenu();
}

void UMainCanvas::Init(ADiabloPlayerController * playerCon, APlayerDiabloCharacter * playerChar, UEquipmentSystem * equipment, UInventory * inven,TArray<UInventory*>* aryStorage)
{
    m_PlayerCon = playerCon;
    m_PlayerPawn = playerChar;
    m_EquipSys = equipment;
    m_Inven = inven;
    m_Storage=aryStorage;
    
    m_MainMenu->Init(m_PlayerCon,m_PlayerPawn,m_EquipSys,m_Inven,m_Storage);
    
    m_PlayerStatusBar->Init(m_PlayerCon);

    m_PlayerStatusBar->m_InteractButton->OnClicked.AddDynamic(this,&UMainCanvas::Interaction);

    m_PlayerStatusBar->m_PotionButton->OnClicked.AddDynamic(this,&UMainCanvas::DrinkPotion);

    m_PlayerPawn->GetExpGaugeDele().AddUObject(this,&UMainCanvas::UpdateExpGauge);


    m_bIsOpened=false;
    //
    m_EquipSys->m_OnOptionChanged.AddUObject(this,&UMainCanvas::UpdateHpBar);
    //
    m_PlayerStatusBar->m_SkillMenuOpenButton->OnClicked.AddDynamic(this,&UMainCanvas::OpenSkillMenu);
    UPlayerDiabloAbilitySystemComp* PlayerGASComp=Cast<UPlayerDiabloAbilitySystemComp>(m_PlayerPawn->GetAbilitySystemComponent());
    
    PlayerGASComp->m_OnSkillChanged.AddUObject(m_MainMenu->m_SkillPanel,&UDiaSkillPanel::UpdateAllWidget);

    //
    for(auto* LearnBtn:m_MainMenu->m_SkillPanel->GetAllSkillLearnBtn())
    {
        LearnBtn->m_OnClicked.AddUObject(m_MainMenu,&UDefaultMenu::OpenSkillPopup);
        LearnBtn->m_OnDragDetect.AddUObject(m_MainMenu,&UDefaultMenu::CloseSkillPopup);
        LearnBtn->m_OnDragDetect.AddUObject(this,&UMainCanvas::ShowSkillHotkeyPanel);
        LearnBtn->m_OnDragEnd.AddUObject(this,&UMainCanvas::CloseSkillHotkeyPanel);
    }
    
   UpdateExpGauge(m_PlayerPawn->GetExpPercent());
}

void UMainCanvas::ShowMonsterInfo(AUnitPawn* monInfo)
{
    UpdateMonsterInfo(monInfo);

    m_MonUpdateHandle=monInfo->GetAttributeSet()->m_OnStatChanged.AddUObject(this,&UMainCanvas::UpdateMonsterInfo);
}

void UMainCanvas::UpdateMonsterInfo(AUnitPawn* monInfo)
{
    if(!monInfo->IsAlive())
    {
        HideMonsterInfo();
        return;
    }
    m_PlayerStatusBar->m_DiaMonInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_PlayerStatusBar->m_DiaMonInfo->SetCharacterLevel(monInfo->GetCharacterLevel());
    m_PlayerStatusBar->m_DiaMonInfo->SetCharacterName(monInfo->GetShowNameText());
    m_PlayerStatusBar->m_DiaMonInfo->SetHealthPercentage(monInfo->GetHpPercentOne());

    
    
    FFormatOrderedArguments Args;
    float CH=monInfo->GetHealth();
    float MH=monInfo->GetMaxHealth();
    Args.Add(UDiaBlueprintFunctionLibrary::GetAlphabetText(CH));
    Args.Add(UDiaBlueprintFunctionLibrary::GetAlphabetText(MH));
    m_PlayerStatusBar->m_DiaMonInfo->SetHealthFormat(FText::Format(m_HpFormat,Args));
}

void UMainCanvas::HideMonsterInfo()
{
    m_MonUpdateHandle.Reset();
    m_PlayerStatusBar->m_DiaMonInfo->SetVisibility(ESlateVisibility::Hidden);
}

void UMainCanvas::UpdateExpGauge(float v)
{
    m_PlayerStatusBar->m_ExpBar->SetProgressValue(v);
}

void UMainCanvas::UpdateHpBar()
{
    m_PlayerStatusBar->SetHealthBarProgressV(m_PlayerPawn);
}

void UMainCanvas::UpdateMinimap(UMaterialInterface* mapMat)
{
    m_PlayerStatusBar->UpdateMinimap(mapMat);
}


void UMainCanvas::ShowBasicShopMenu(AShopKeeper* shopKeeper)
{
    PRINTF("ShopMenu");
    m_MainMenu->OpenShopMenu(shopKeeper);

    
}

void UMainCanvas::ShowStorageMenu()
{
    m_MainMenu->OpenStorage();
}

void UMainCanvas::HideMinimap()
{
    m_PlayerStatusBar->HideMinimap();
}

void UMainCanvas::ShowSkillHotkeyPanel()
{
    m_PlayerStatusBar->m_SkillUseCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UMainCanvas::CloseSkillHotkeyPanel()
{
    m_PlayerStatusBar->m_SkillUseCanvas->SetVisibility(ESlateVisibility::Hidden);
}

UDiaShopPanel* UMainCanvas::GetShopPanelWidget()
{
    return m_MainMenu->GetShopPanelWidget();
}

void UMainCanvas::OpenMapMenu()
{
    m_MapSelect->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    m_PlayerStatusBar->SetVisibility(ESlateVisibility::Hidden);
}

void UMainCanvas::CloseMapMenu()
{
    m_MapSelect->SetVisibility(ESlateVisibility::Hidden);

    m_PlayerStatusBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}


void UMainCanvas::ShowMinimap()
{
    m_PlayerStatusBar->ShowMinimap();
}
