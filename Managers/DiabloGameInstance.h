#pragma once

#include "DiabloM.h"
#include "EquipManager.h"
#include "GoldManager.h"
#include "NavigationSystem.h"
#include "PlayerUpgradeManager.h"

#include "DiabloGameInstance.generated.h"


class UPlayfabManager;
class APlayerVisual;
class UStartMenuCanvas;
class UPlayerCreateManager;
class UMonsterSpawnManager;
class UDungeonManager;
struct FItemTier;

UCLASS()
class DIABLOM_API UDiabloGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UDiabloGameInstance();//
	static UDiabloGameInstance* Get;
	
public:
	UPROPERTY()
	UPlayfabManager* m_PlayfabManager;
	UPROPERTY()
	UMonsterSpawnManager* m_MonsterSpawn;
	UPROPERTY()
	UDungeonManager* m_DungeonManager;
	UPROPERTY()
	UPlayerUpgradeManager* m_PlayerUpgradeManager;
	UPROPERTY()
	UEquipManager* m_EquipManager;
	UPROPERTY()
	UGoldManager* m_GoldManager;
	
protected:
	virtual void Init() override;

public:
	ADiabloPlayerController* GetPlCon();
	
	APlayerDiabloCharacter* GetPlChar();

	UNavigationSystemV1* GetNavSys();

	

public:
	void SavePlayerSetting();

	void LoadPlayerSetting();
};
