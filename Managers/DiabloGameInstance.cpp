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
    m_DungeonManager = NewObject<UDungeonManager>();
    m_DungeonManager->Init();
    m_PlayerUpgradeManager = NewObject<UPlayerUpgradeManager>();
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
    m_PlayfabManager = NewObject<UPlayfabManager>();
    m_PlayfabManager->Init();
	m_ChatManager = NewObject<UChatManager>();
    //
	
	//
    UGameUserSettings::GetGameUserSettings()->SetFrameRateLimit(33.f);
    
    UGameUserSettings::GetGameUserSettings()->ApplySettings(true);

#if PLATFORM_WINDOWS
	UGameUserSettings::GetGameUserSettings()->SetScreenResolution(FIntPoint(360,640));

	UGameUserSettings::GetGameUserSettings()->ApplyResolutionSettings(true);

#endif
    
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

