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
	m_GachaManager = NewObject<UGachaManager>();
	m_QuestManager= NewObject<UQuestManager>();
	m_ShopManager= NewObject<UShopManager>();
    //
    m_PlayfabManager = NewObject<UPlayfabManager>();
    m_PlayfabManager->Init();
    //
	
	//
    UGameUserSettings::GetGameUserSettings()->SetFrameRateLimit(33.f);
    
    UGameUserSettings::GetGameUserSettings()->ApplySettings(true);
    
    UKismetSystemLibrary::ControlScreensaver(false);

    //
	m_ChatManager = NewObject<UChatManager>();
}

void UDiabloGameInstance::Shutdown()
{
	Super::Shutdown();
	UDiabloGameInstance::Get->m_PlayfabManager->SetOfflineStatus();
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
	auto* PlCon = UGameplayStatics::GetPlayerController(GetWorld(),0);
	
	if(!PlCon)
	{
		return;
	}

	Cast<AMyHUD>( PlCon->GetHUD())->ReqeustPopupText(txt);
}

void UDiabloGameInstance::SavePlayerSetting()
{
	
}

void UDiabloGameInstance::LoadPlayerSetting()
{
	
}

