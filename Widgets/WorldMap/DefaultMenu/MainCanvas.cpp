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

void UMainCanvas::Init(ADiabloPlayerController * playerCon, APlayerDiabloCharacter * playerChar, UEquipmentSystem * equipment, UInventory * inven,TArray<UInventory*>* aryStorage)
{
    m_PlayerCon = playerCon;
    m_PlayerPawn = playerChar;
    m_EquipSys = equipment;
    m_Inven = inven;
    m_Storage=aryStorage;

    m_OnWidgetOpenClose.AddUObject(m_PlayerCon,&ADiabloPlayerController::OnWidgetOpenClose);
    
    m_OnWidgetOpenClose.AddUObject(this,&UMainCanvas::OnOpenClosePlayerHUD);

    m_PlayerPawn->m_OnFocusTarget.AddUObject(this,&UMainCanvas::OnMonsterFocused);
    
    m_PlayerPawn->GetExpGaugeDele().AddUObject(this,&UMainCanvas::UpdateExpGauge);
    
    m_MainMenu->Init(m_PlayerCon,m_PlayerPawn,m_EquipSys,m_Inven,m_Storage);
    
    m_PlayerStatusBar->Init(m_PlayerCon,this);

    m_EquipSys->GetItemChangeCallback().AddUObject(this,&UMainCanvas::UpdateHpBar);
    //
    UPlayerDiabloAbilitySystemComp* PlayerGASComp=Cast<UPlayerDiabloAbilitySystemComp>(m_PlayerPawn->GetAbilitySystemComponent());
    
    PlayerGASComp->m_OnSkillChanged.AddUObject(m_MainMenu->m_SkillPanel,&UDiaSkillPanel::UpdateAllWidget);
    //
    for(auto* LearnBtn:m_MainMenu->m_SkillPanel->GetAllSkillLearnBtn())
    {
        LearnBtn->m_OnClicked.AddUObject(m_MainMenu,&UDefaultMenu::OpenSkillPopup);
        LearnBtn->m_OnDragDetect.AddUObject(m_MainMenu,&UDefaultMenu::CloseSkillPopup);
        LearnBtn->m_OnDragDetect.AddUObject(this,&UMainCanvas::OpenSkillHotkeyPanel);
        LearnBtn->m_OnDragEnd.AddUObject(this,&UMainCanvas::CloseSkillHotkeyPanel);
    }
    
    UpdateExpGauge(m_PlayerPawn->GetExpPercent());
    //
    m_MapSelect->Init();

    m_BtnCloseMenu->OnClicked.AddDynamic(this,&UMainCanvas::CloseMainMenu);

    m_BGBlur->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainCanvas::OnOpenClosePlayerHUD(bool isOpen)
{
    if(isOpen)
    {
        m_PlayerStatusBar->HidePlayerHUD();
        m_BGBlur->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        m_PlayerStatusBar->ShowPlayerHUD();
        m_BGBlur->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UMainCanvas::DrinkPotion()
{
    m_PlayerPawn->DrinkPotion();
    //need cd
}

void UMainCanvas::Interaction()
{
    m_PlayerPawn->InteractWithTarget();
}

void UMainCanvas::TestOpenDungeon()
{
    UDiabloGameInstance::Get->CreateDungeon();
}

void UMainCanvas::TestSaveAll()
{
    UDiabloGameInstance::Get->SaveAllPlayerInfo();
}

void UMainCanvas::TestGoBackMenu()
{
    m_PlayerCon->BackToSelectMenu();
}

void UMainCanvas::OpenMainMenu()
{
    m_OnWidgetOpenClose.Broadcast(true);
    m_MainMenu->OpenMainMenu();
}

void UMainCanvas::CloseMainMenu()
{
    m_OnWidgetOpenClose.Broadcast(false);
    m_MainMenu->CloseMainMenu();
    m_MapSelect->CloseDgPanel();
}

void UMainCanvas::OpenSkillMenu()
{
    m_OnWidgetOpenClose.Broadcast(true);
    m_MainMenu->OpenSkillPanel();
}

void UMainCanvas::CloseSkillMenu()
{
    m_OnWidgetOpenClose.Broadcast(false);
    m_MainMenu->CloseSkillPanel();
}

void UMainCanvas::OpenSetting()
{
    m_OnWidgetOpenClose.Broadcast(true);
}

void UMainCanvas::CloseSetting()
{
    m_OnWidgetOpenClose.Broadcast(false);
}

void UMainCanvas::OpenMonsterInfo(AUnitPawn* monInfo)
{
    UpdateMonsterInfo(monInfo);

    m_MonUpdateHandle=monInfo->GetAttributeSet()->m_OnStatChanged.AddUObject(this,&UMainCanvas::UpdateMonsterInfo);
}

void UMainCanvas::UpdateMonsterInfo(AUnitPawn* monInfo)
{
    if(!monInfo->IsAlive())
    {
        CloseMonsterInfo();
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

void UMainCanvas::CloseMonsterInfo()
{
    m_MonUpdateHandle.Reset();
    m_PlayerStatusBar->m_DiaMonInfo->SetVisibility(ESlateVisibility::Hidden);
}

void UMainCanvas::UpdateExpGauge(float v)
{
    m_PlayerStatusBar->m_ExpBar->SetProgressValue(v);
}

void UMainCanvas::UpdateHpBar(int, FItemInstance&)
{
    m_PlayerStatusBar->SetHealthBarProgressV(m_PlayerPawn);
}

void UMainCanvas::UpdateMinimap(UMaterialInterface* mapMat)
{
    m_PlayerStatusBar->UpdateMinimap(mapMat);
}

void UMainCanvas::OpenBasicShopMenu(AShopKeeper* shopKeeper)
{
    m_OnWidgetOpenClose.Broadcast(true);
    
    m_MainMenu->OpenShopMenu(shopKeeper);
}

void UMainCanvas::CloseBasicShopMenu()
{
    m_OnWidgetOpenClose.Broadcast(false);
    
    m_MainMenu->CloseShopMenu();
}

void UMainCanvas::OpenStorageMenu()
{
    m_OnWidgetOpenClose.Broadcast(true);
    
    m_MainMenu->OpenStorage();
}

void UMainCanvas::CloseStorageMenu()
{
    m_OnWidgetOpenClose.Broadcast(false);

    m_MainMenu->CloseStorage();
}

void UMainCanvas::OpenSkillHotkeyPanel()
{
    m_PlayerStatusBar->m_SkillUseCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UMainCanvas::CloseSkillHotkeyPanel()
{
    m_PlayerStatusBar->m_SkillUseCanvas->SetVisibility(ESlateVisibility::Hidden);
}

void UMainCanvas::OpenMapMenu(bool isCleared)
{
    m_OnWidgetOpenClose.Broadcast(true);
    
    m_MapSelect->Open(isCleared);
}

void UMainCanvas::CloseMapMenu()
{
    m_OnWidgetOpenClose.Broadcast(false);
    
    m_MapSelect->CloseDgPanel();
    
    m_MapSelect->SetVisibility(ESlateVisibility::Hidden);
}

void UMainCanvas::OpenMinimap()
{
    m_PlayerStatusBar->ShowMinimap();
}

void UMainCanvas::CloseMinimap()
{
    m_PlayerStatusBar->HideMinimap();
}
 void UMainCanvas::OnMonsterFocused(AUnitPawn* monInfo)
{
    if(!monInfo)
    {
        CloseMonsterInfo();

        return;
    }

    OpenMonsterInfo(monInfo);
    
}