#include "DiabloGameInstance.h"
#include "BigInt.h"
#include "NavigationSystem.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/Pawns/PlayerDiabloCharacter.h"
#include "DgContentManagers/DungeonManager.h"
#include "DgContentManagers/MagicStoneDgManager.h"
#include "DgContentManagers/NormalDungeonManager.h"
#include "DgContentManagers/PVPManager.h"
#include "GameFramework/GameUserSettings.h"
#include "GameMode/DiabloGameMode.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "SystemManagers/AdvertiseManager.h"
#include "SystemManagers/ChatManager.h"
#include "SystemManagers/DailyPrizeManager.h"
#include "SystemManagers/InboxManager.h"
#include "SystemManagers/PlayfabManager.h"
#include "Widgets/HUD/MyHUD.h"


UDiabloGameInstance* UDiabloGameInstance::Get = nullptr;

UDiabloGameInstance::UDiabloGameInstance()
{
    UDiabloGameInstance::Get = this;

    m_NormalDgManager = nullptr;

    m_DungeonManager = nullptr;
}

void UDiabloGameInstance::Init()
{
    Super::Init();

    m_NormalDgManager = NewObject<UNormalDungeonManager>();
	m_NormalDgManager->Init();
    m_PlayerUpgradeManager = NewObject<UPlayerUpgradeManager>();
    m_DungeonManager = NewObject<UDungeonManager>();
    m_DungeonManager->Init();
    m_GoldManager = NewObject<UGoldManager>();
    m_EquipManager = NewObject<UEquipManager>();
	m_GachaManager = NewObject<UGachaManager>();
	m_QuestManager= NewObject<UQuestManager>();
	m_ShopManager= NewObject<UShopManager>();
	m_DailyManager= NewObject<UDailyPrizeManager>();
	m_InboxManager= NewObject<UInboxManager>();
	m_AdverManager= NewObject<UAdvertiseManager>();
	m_AdverManager->Init();
	m_PVPManager=NewObject<UPVPManager>();
	m_MagicDgManager=NewObject<UMagicStoneDgManager>();
	m_MagicDgManager->Init();
	m_PetDgManager=NewObject<UPetDgManager>();
	m_PetDgManager->Init();
	m_WeaponDgManager=NewObject<UWeaponScrollDgManager>();
	m_WeaponDgManager->Init();
	
    m_PlayfabManager = NewObject<UPlayfabManager>();
    m_PlayfabManager->Init();
	m_ChatManager = NewObject<UChatManager>();
    //
	
}

void UDiabloGameInstance::LoadOptionSaveData()
{
	m_LoadedOptionSave = Cast<UOptionSave>(UGameplayStatics::LoadGameFromSlot(TEXT("OptionSave"),0));

	if(!m_LoadedOptionSave)
	{
		m_LoadedOptionSave = Cast<UOptionSave>(UGameplayStatics::CreateSaveGameObject(UOptionSave::StaticClass()));
		UGameplayStatics::SaveGameToSlot(m_LoadedOptionSave,TEXT("OptionSave"),0);	
	}
}

void UDiabloGameInstance::UnloadSaveOptionSaveData()
{
	if(m_LoadedOptionSave)
	{
		UGameplayStatics::SaveGameToSlot(m_LoadedOptionSave,TEXT("OptionSave"),0);	
	}
	m_LoadedOptionSave = nullptr;
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

AGameLevelHUD* UDiabloGameInstance::GetHud()
{
	if(!UGameplayStatics::GetPlayerController(GetWorld(),0))
	{
		return nullptr;
	}

	auto* MyHud =UGameplayStatics::GetPlayerController(GetWorld(),0)->GetHUD();

	return Cast<AGameLevelHUD>( MyHud);
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
