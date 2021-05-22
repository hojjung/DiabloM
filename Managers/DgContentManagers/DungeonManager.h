#pragma once

#include "DiabloM.h"
#include "MonsterSpawnManager.h"
#include "Managers/PlayerUpgradeManager.h"

#include "DungeonManager.generated.h"


class ANavigationData;
class AMonsterPawn;



UCLASS()
class DIABLOM_API UDungeonManager : public UObject//어떤 던전을 갈것인지 여기에 리퀘스트
{
	GENERATED_BODY()
	
public:
	DECLARE_DELEGATE_OneParam(FOnLevelLoad,UWorld*);

	FOnLevelLoad m_OnLevelLoadComplete;
	
protected:
	UPROPERTY()
	UPlayerUpgradeManager* m_PlayerUpgradeManager;
	UPROPERTY()
	UMonsterSpawnManager* m_CurrentSpawnManager;

	FDelegateHandle m_LevelDeleHandle;

public:
	void Init();

	void OpenLevel(UMonsterSpawnManager* currentDgManager);

	AUnitPawn* GetNearEnemy(const FVector& loc);

	UMonsterSpawnManager* GetCurrentSpawnManager() const
	{
		return m_CurrentSpawnManager;
	}

	void Tick(float deltaTime);
	
	bool TryOpenDgKey(int cost);
	
protected:
	void OnLevelLoadComplete(UWorld* world);
	
};

