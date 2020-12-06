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
    
    m_PlayerStatusBar->SetVisibility(ESlateVisibility::Hidden);
    m_SettingButton->SetVisibility(ESlateVisibility::Hidden);
    m_AttackButton->SetVisibility(ESlateVisibility::Hidden);
    m_InteractButton->SetVisibility(ESlateVisibility::Hidden);
    UGameplayStatics::SetGamePaused(m_PlayerCon->GetWorld(),true);
    m_PlayerCon->SetVirtualJoystickVisibility(false);
}

void UMainCanvas::CloseMainMenu()
{
    m_bIsOpened=false;
    m_MainMenu->CloseMainMenu();

    m_PlayerStatusBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_SettingButton->SetVisibility(ESlateVisibility::Visible);
    m_AttackButton->SetVisibility(ESlateVisibility::Visible);
    m_InteractButton->SetVisibility(ESlateVisibility::Visible);

    UGameplayStatics::SetGamePaused(m_PlayerCon->GetWorld(),false);
    m_PlayerCon->SetVirtualJoystickVisibility(true);
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

    m_SettingButton->OnClicked.AddDynamic(this,&UMainCanvas::OpenSetting);

    m_InteractButton->OnClicked.AddDynamic(this,&UMainCanvas::Interaction);

    m_PotionButton->OnClicked.AddDynamic(this,&UMainCanvas::DrinkPotion);

    m_AttackButton->OnPressed.AddDynamic(this,&UMainCanvas::OnAttackPressStart);
    m_AttackButton->OnReleased.AddDynamic(this,&UMainCanvas::OnAttackPressEnd);

    m_PlayerPawn->GetExpGaugeDele().AddUObject(this,&UMainCanvas::UpdateExpGauge);

    m_PlayerStatusBar->Init(m_PlayerCon);

    m_bIsOpened=false;
    //
    m_EquipSys->m_OnOptionChanged.AddUObject(this,&UMainCanvas::UpdateHpBar);
    //
    m_SkillMenuButton->OnClicked.AddDynamic(this,&UMainCanvas::OpenSkillMenu);
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
    m_DiaMonInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_DiaMonInfo->SetCharacterLevel(monInfo->GetCharacterLevel());
    m_DiaMonInfo->SetCharacterName(monInfo->GetShowNameText());
    m_DiaMonInfo->SetHealthPercentage(monInfo->GetHpPercentOne());

    
    
    FFormatOrderedArguments Args;
    float CH=monInfo->GetHealth();
    float MH=monInfo->GetMaxHealth();
    Args.Add(UDiaBlueprintFunctionLibrary::GetAlphabetText(CH));
    Args.Add(UDiaBlueprintFunctionLibrary::GetAlphabetText(MH));
    m_DiaMonInfo->SetHealthFormat(FText::Format(m_HpFormat,Args));
}

void UMainCanvas::HideMonsterInfo()
{
    m_MonUpdateHandle.Reset();
    m_DiaMonInfo->SetVisibility(ESlateVisibility::Hidden);
}

void UMainCanvas::UpdateExpGauge(float v)
{
    m_ExpBar->SetProgressValue(v);
}

void UMainCanvas::UpdateHpBar()
{
    m_PlayerStatusBar->SetHealthBarProgressV(m_PlayerPawn);
}

void UMainCanvas::UpdateMinimap(UMaterialInterface* mapMat)
{
    m_PlayerStatusBar->UpdateMinimap(mapMat);
}

void UMainCanvas::ShowWorldMap()
{
    PRINTF("WorldMap");
}

void UMainCanvas::HideWorldMap()
{
    
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

UDiaShopPanel* UMainCanvas::GetShopPanelWidget()
{
    return m_MainMenu->GetShopPanelWidget();
}

void UMainCanvas::ShowMinimap()
{
    m_PlayerStatusBar->ShowMinimap();
}
