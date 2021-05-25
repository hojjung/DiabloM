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

	static const int MonsterPoolCount = 12;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnBossTimer, float, float);
	DECLARE_MULTICAST_DELEGATE(FOnDungeonMaxUpdate);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDgOpen, int);

	FOnBossTimer m_OnBossBattleTick;

	static UDataTable* DungeonDataTable;

	static UDataTable* DropDataTable;

	static UDataTable* MonsterEntityTable;
protected:
	UPROPERTY()
	float m_fBossTimer;
	UPROPERTY()
	bool m_bTouched;
	UPROPERTY()
	FRotator m_InitVisualRot;
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
	UPROPERTY()
	float m_fSpawnRadius;
	UPROPERTY()
	float m_SensingInterval;

	FName m_IdEnemy;

	FName m_IdBossEnemy;

	FName m_IdSpecialEnemy;

	const FDungeonDataTableRow* m_DgDataTable;

	FTimerHandle m_TimerHandle_OnTimer;

	FMonsterEntity* m_GoldGoblinEntity;

	UPROPERTY()
	int m_nGoldGoblinSpawnCount;

	UPROPERTY()
	UDataTable* m_MobEntityTable;

public:
	UPROPERTY()
	AOtherPlayerPawn* m_OtherPlayer;
	UPROPERTY()
	APlayerVisual* m_VisualActor;

	FOnDungeonMaxUpdate m_OnDungeonMaxUpdate;

	FOnDgOpen m_OnDgOpen;
	
	FSafeInt m_nMyMaxStageLevel;
	
	FSafeInt m_nCurrentStageLevel;
	
protected:
	TArray<const FDungeonDataTableRow*> m_AryDgDataTable;

	const FDungeonDataTableRow* m_CurrentDg;



	FVector GetRandomPointFromNav(const FVector& loc, const float& radius);

	AMonsterPawn* CreateMob(FVector loc);

	AMonsterPawn* GetReadyMonster();

	void OnTimer();

	void SetTimer(const float TimeDelay);

	void SetSpawnMonsterOnTick(const bool bEnabled);

	void Reset();

	void SpawnBossMob();

public:
	void Init();

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

	UFUNCTION()
	AMonsterPawn* SpawnMobToLoc(FVector loc);

	void OnBossDeadBeforeAnim(AMonsterPawn*);

	void OnBossDead(AMonsterPawn*);

	virtual void BeginDestroy() override;

	void SetDungeonData(const FString& dgJsonStr);

	FORCEINLINE const TArray<const FDungeonDataTableRow*>& GetAryDgData() const
	{
		return m_AryDgDataTable;
	}

	void SelectNormalDungeon(int index);

	void LevelUpDungeon(); //Call By Boss

	BigInt GetCurrentDungeonBounty();

	BigInt GetMaxDungeonBounty();

	FString GetDgDataStr();

	virtual void OnLevelLoadComplete(UWorld* world) override;

	virtual bool IsBattleStarted() override;

	virtual FString GetOpenLevelAssetName() override;

	void OnMonsterDead(AMonsterPawn* self);

	virtual void StartDungeon() override;

	virtual void EndDungeon(bool b) override;

	void SpawnVisualActor(UWorld* world);

	void SpawnOtherPVPActor(UWorld* world);

	void OnMenuOpen(bool b);

	void RotatePawn(float x);

	void OnTouchStart();

	void OnTouchEnd();

	virtual void Tick(float delta) override;

	void CalculateVisualActorRot(float delta);

};
