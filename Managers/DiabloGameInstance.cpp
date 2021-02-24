#include "DiabloGameInstance.h"
#include "BigInt.h"
#include "NavigationSystem.h"
#include "MonsterSpawnManager.h"
#include "DungeonManager.h"
#include "PlayfabManager.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "GameFramework/GameUserSettings.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"


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
    m_PlayerClassManager = NewObject<UPlayerClassManager>();
    m_GoldManager = NewObject<UGoldManager>();
    //
    m_PlayfabManager = NewObject<UPlayfabManager>();
    m_PlayfabManager->Init();
    //
    //
    UGameUserSettings::GetGameUserSettings()->SetFrameRateLimit(40.f);
    
    UGameUserSettings::GetGameUserSettings()->ApplySettings(true);
    
    UKismetSystemLibrary::ControlScreensaver(false);

    //
}

void UDiabloGameInstance::SaveAllPlayerInfo()
{
}


