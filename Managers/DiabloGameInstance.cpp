#include "DiabloGameInstance.h"
#include "BigInt.h"
#include "NavigationSystem.h"
#include "MonsterSpawnManager.h"
#include "DungeonManager.h"
#include "PlayfabManager.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "GameFramework/GameUserSettings.h"
#include "GameMode/DiabloGameMode.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Widgets/MainCanvas.h"
#include "Widgets/MyHUD.h"


UDiabloGameInstance* UDiabloGameInstance::Get = nullptr;

UDiabloGameInstance::UDiabloGameInstance()
{
    UDiabloGameInstance::Get = this;

    m_MonsterSpawn = nullptr;

    m_DungeonManager = nullptr;
}

void UDiabloGameInstance::Init()
{
    Super::Init();
    m_MonsterSpawn = NewObject<UMonsterSpawnManager>();
    m_DungeonManager = NewObject<UDungeonManager>();
    m_DungeonManager->Init(m_MonsterSpawn);
    m_PlayerUpgradeManager = NewObject<UPlayerUpgradeManager>();
    m_GoldManager = NewObject<UGoldManager>();
    m_EquipManager = NewObject<UEquipManager>();
    //
    m_PlayfabManager = NewObject<UPlayfabManager>();
    m_PlayfabManager->Init();
    //
    //
    UGameUserSettings::GetGameUserSettings()->SetFrameRateLimit(33.f);
    
    UGameUserSettings::GetGameUserSettings()->ApplySettings(true);
    
    UKismetSystemLibrary::ControlScreensaver(false);

    //
}

ADiabloPlayerController* UDiabloGameInstance::GetPlCon()
{
	return Cast<ADiabloPlayerController>( UGameplayStatics::GetPlayerController(GetWorld(),0));
}

APlayerDiabloCharacter* UDiabloGameInstance::GetPlChar()
{
     return Cast<APlayerDiabloCharacter>( UGameplayStatics::GetPlayerPawn(GetWorld(),0));
}

UNavigationSystemV1* UDiabloGameInstance::GetNavSys()
{
	return Cast<UNavigationSystemV1>( GetWorld()->GetNavigationSystem());
}

void UDiabloGameInstance::RequestPopupText(FString txt)
{
	RequestPopupText(FText::FromString(txt));
}

void UDiabloGameInstance::RequestPopupText(FText txt)
{
	if(!GetPlCon())
	{
		return;
	}

	Cast<AMyHUD>( GetPlCon()->GetHUD())->m_Canvas->ReqeustText(txt);
}

void UDiabloGameInstance::SavePlayerSetting()
{
	
}

void UDiabloGameInstance::LoadPlayerSetting()
{
	
}

