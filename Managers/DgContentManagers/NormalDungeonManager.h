// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "MonsterSpawnManager.h"
#include "NavigationPath.h"
#include "NavigationData.h"
#include "NavigationSystem.h"
#include "Characters/Pawns/MonsterPawn.h"
#include "Datas/DungeonDataTable.h"


#include "NormalDungeonManager.generated.h"


class APlayerVisual;
class AOtherPlayerPawn;
USTRUCT()
struct FNormalDgDataSpec
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int m_nCurrentStage;
	UPROPERTY(EditAnywhere)
	int m_nMaxStage;
};

UCLASS()
class DIABLOM_API UNormalDungeonManager : public UMonsterSpawnManager
{
	GENERATED_BODY()


public:
	UNormalDungeonManager();

protected:
	static const int MonsterPoolCount = 12;
	UPROPERTY()
	UNavigationSystemV1* m_NavSys;
	UPROPERTY()
	UWorld* m_CurrentWorld;
	UPROPERTY()
	UParticleSystem* m_ParticleCoin;
	UPROPERTY()
	TArray<AMonsterPawn*> m_AryMonsterSpawnedCurrently;
	UPROPERTY()
	int m_nKillCount;
	UPROPERTY()
	AMonsterPawn* m_SpawnedBoss;
	UPROPERTY()
	TSubclassOf<UCameraShake> m_ClassShake;

	FName m_IdEnemy;

	FName m_IdBossEnemy;

	FName m_IdSpecialEnemy;

	bool m_bBossSpawned;

	const FDungeonDataTableRow* m_DgDataTable;

	float m_fSpawnRadius;

	float m_SensingInterval;

	FTimerHandle m_TimerHandle_OnTimer;

	FMonsterEntity* m_GoldGoblinEntity;

	UPROPERTY()
	int m_nGoldGoblinSpawnCount;

	FDelegateHandle m_BossDeleHandle;

	TArray<TSharedPtr<FStreamableHandle>> m_LoadedMonsters;

	TSharedPtr<FStreamableHandle> m_LoadedGoblin;
	UPROPERTY()
	UDataTable* m_MobEntityTable;

public:
	UPROPERTY()
	AOtherPlayerPawn* m_OtherPlayer;
	UPROPERTY()
	APlayerVisual* m_VisualActor;

protected:
	FVector GetRandomPointFromNav(const FVector& loc, const float& radius);

	AMonsterPawn* CreateMob(FVector loc);

	//void MakeBossMonster(AMonsterPawn* mob);
	AMonsterPawn* GetReadyMonster();

	void OnTimer();

	void SetTimer(const float TimeDelay);

	void SetSensingInterval(const float newSensingInterval);

	void SetSpawnMonsterOnTick(const bool bEnabled);

public:
	void Init();

	void Reset();

	FORCEINLINE TArray<AMonsterPawn*>& GetCurrentMonsters()
	{
		return m_AryMonsterSpawnedCurrently;
	}

	virtual AUnitPawn* GetNearestEnemy(const FVector& wantPos) override;

	void StartSpawn(UWorld* world, const FDungeonDataTableRow* dgData);

	void AddKillCount();

	

	void FailBossKill();

	AMonsterPawn* GetBossMob() const
	{
		return m_SpawnedBoss;
	}

	bool IsBossBattleIn()
	{
		return m_bBossSpawned && GetBossMob() && GetBossMob()->IsAlive();
	}

	UFUNCTION()
	AMonsterPawn* SpawnMobToLoc(FVector loc);

	void OnBossDead(AMonsterPawn*);

	virtual void BeginDestroy() override;

protected:
	void SpawnBossMob();
	//

public:
	DECLARE_MULTICAST_DELEGATE(FOnDungeonMaxUpdate);

	FOnDungeonMaxUpdate m_OnDungeonMaxUpdate;


	static UDataTable* DungeonDataTable;

	static UDataTable* DropDataTable;

	static UDataTable* MonsterEntityTable;

	static UDataTable* GoldDungeonDataTable;

	//FItemDropTableRow
	//FMonsterEntity
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDgOpen, int);

	FOnDgOpen m_OnDgOpen;
protected:
	TArray<const FDungeonDataTableRow*> m_AryDgDataTable;


	const FDungeonDataTableRow* m_CurrentDg;


	UPROPERTY()
	int m_nMyMaxStageLevel;
	UPROPERTY()
	int m_nCurrentStageLevel;
	UPROPERTY()
	int m_nSafeMaxStageLevel;
	UPROPERTY()
	int m_nSafeCurrentStageLevel;
	//보스는 10킬이후부터 생성?
	//보스는 1회만 죽여야한다
	//보스는 1회만?
	//그럼 결국 던전이 킬카운트 가지고 있어야한다

protected: //GoldDg
	TArray<const FDungeonDataTableRow*> m_AryGoldDgDataTable;

	const FDungeonDataTableRow* m_CurrentGoldDg;
public:
	void SetDungeonData(const FString& dgJsonStr);

	FORCEINLINE const TArray<const FDungeonDataTableRow*>& GetAryDgData() const
	{
		return m_AryDgDataTable;
	}

	void SelectNormalDungeon(int index);

	void SelectGoldDungeon(int index);

	void LevelUpDungeon(); //Call By Boss

	BigInt GetCurrentDungeonBounty();

	BigInt GetMaxDungeonBounty();

	int GetMaxStage() const;

	int GetCurrentStage() const;

	void SetMaxStageLevel(int stageLv);

	void SetCurrentStageLevel(int stageLv);

	FString GetDgDataStr();

public:
	virtual void OnLevelLoadComplete(UWorld* world) override;

	virtual bool IsBattleStarted() override;

	virtual FString GetOpenLevelAssetName() override;

	void OnMonsterDead(AMonsterPawn* self);

	virtual void StartDungeon() override;

	virtual void EndDungeon(bool b) override;

	void SpawnVisualActor();

	void SpawnOtherPVPActor();

	void OnMenuOpen(bool b);
};
