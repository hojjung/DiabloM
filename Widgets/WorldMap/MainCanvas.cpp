#include "MainCanvas.h"


#include "DiaMonsterInfo.h"
#include "PlayerStatusBar.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "DefaultMenu/MaterialProgressBar.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
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
}

void UMainCanvas::Interaction()
{
    m_PlayerPawn->InteractWithTarget();
}

void UMainCanvas::Attack()
{
    m_PlayerPawn->AttackInput(1.f);
}

void UMainCanvas::OpenSetting()
{
}

void UMainCanvas::CloseSetting()
{
    
}

void UMainCanvas::Init(ADiabloPlayerController * playerCon, APlayerDiabloCharacter * playerChar, UEquipmentSystem * equipment, UInventory * inven)
{
    m_PlayerCon = playerCon;
    m_PlayerPawn = playerChar;
    m_EquipSys = equipment;
    m_Inven = inven;
    m_MainMenu->Init(m_PlayerCon,m_PlayerPawn,m_EquipSys,m_Inven);

    m_SettingButton->OnClicked.AddDynamic(this,&UMainCanvas::OpenSetting);

    m_InteractButton->OnClicked.AddDynamic(this,&UMainCanvas::Interaction);

    m_AttackButton->OnPressed.AddDynamic(this,&UMainCanvas::Attack);

    m_PlayerPawn->GetExpGaugeDele().AddUObject(this,&UMainCanvas::UpdateExpGauge);

    m_PlayerStatusBar->Init(m_PlayerCon);

    m_bIsOpened=false;
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
    m_DiaMonInfo->SetCharacterLevel(monInfo->GetLevel());
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
