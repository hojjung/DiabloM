#pragma once

#include "DiabloM.h"
#include "PlayerClassManager.h"
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
	UPlayerClassManager* m_PlayerClassManager;
	UPROPERTY()
	UPlayerUpgradeManager* m_PlayerUpgradeManager;
	
protected:
	virtual void Init() override;

public:
	void SaveAllPlayerInfo();
};
