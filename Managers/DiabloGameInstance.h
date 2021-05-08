#pragma once

#include "DiabloM.h"
#include "EquipManager.h"
#include "GachaManager.h"
#include "GoldManager.h"
#include "NavigationSystem.h"
#include "PlayerUpgradeManager.h"
#include "QuestManager.h"
#include "ShopManager.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/Pawns/PlayerDiabloCharacter.h"
#include "DgContentManagers/MagicStoneDgManager.h"
#include "DgContentManagers/NormalDungeonManager.h"
#include "DgContentManagers/PVPManager.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "SystemManagers/AdvertiseManager.h"
#include "SystemManagers/ChatManager.h"
#include "SystemManagers/DailyPrizeManager.h"
#include "Widgets/HUD/GameLevelHUD.h"
#include "SCUE4.h"


#include "DiabloGameInstance.generated.h"


class UPlayfabManager;
class APlayerVisual;
class UStartMenuCanvas;
class UPlayerCreateManager;
class UMonsterSpawnManager;
class UDungeonManager;
struct FItemTier;



UCLASS()
class DIABLOM_API UDiabloGameInstance : public USafeGameInstance
{
	GENERATED_BODY()
	
public:
	UDiabloGameInstance();//
	static UDiabloGameInstance* Get;

	
public:
	UPROPERTY()
	UPlayfabManager* m_PlayfabManager;
	UPROPERTY()
	UNormalDungeonManager* m_NormalDgManager;
	UPROPERTY()
	UDungeonManager* m_DungeonManager;
	UPROPERTY()
	UPlayerUpgradeManager* m_PlayerUpgradeManager;
	UPROPERTY()
	UEquipManager* m_EquipManager;
	UPROPERTY()
	UGoldManager* m_GoldManager;
	UPROPERTY()
	UGachaManager* m_GachaManager;
	UPROPERTY()
	UChatManager* m_ChatManager;
	UPROPERTY()
	UQuestManager* m_QuestManager;
	UPROPERTY()
	UShopManager* m_ShopManager;
	UPROPERTY()
	UDailyPrizeManager* m_DailyManager;
	UPROPERTY()
	UInboxManager* m_InboxManager;
	UPROPERTY()
	UAdvertiseManager* m_AdverManager;
	UPROPERTY()
	UPVPManager* m_PVPManager;
	UPROPERTY()
	UMagicStoneDgManager* m_MagicDgManager;
	
protected:
	virtual void Init() override;
	
public:
	ADiabloPlayerController* GetPlCon();
	
	APlayerDiabloCharacter* GetPlChar();

	UNavigationSystemV1* GetNavSys();

	AGameLevelHUD* GetHud();

	void RequestPopupText(FString txt);
	
	void RequestPopupText(FText txt);

public:
	void SavePlayerSetting();

	void LoadPlayerSetting();
};
