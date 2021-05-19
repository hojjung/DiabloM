#pragma once
#include "DiabloM.h"
#include "MonsterSpawnManager.h"
#include "Characters/Pawns/MonsterPawn.h"

#include "WeaponScrollDgManager.generated.h"

#define WEAPONDGLIMITTIME 33.f

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FWeaponDgTableRow : public FDungeonDataTableRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nPrizeWeaponStoneMin = 15;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nPrizeWeaponStoneMax = 25;
	

	int GetRandomPrize()
	{
		return FMath::RandRange(m_nPrizeWeaponStoneMin,m_nPrizeWeaponStoneMax);
	}
};

UCLASS()
class DIABLOM_API UWeaponScrollDgManager : public UMonsterSpawnManager
{
	GENERATED_BODY()

public:
	UWeaponScrollDgManager();

	static const int MonsterPoolCount = 12;

	DECLARE_DELEGATE_OneParam(FOnObtainBounty,int);

	DECLARE_DELEGATE_TwoParams(FOnMobDead,int,int);

	FOnObtainBounty m_OnObtainBounty;

	FOnTick m_OnTimerTick;

	FOnMobDead m_OnMobDead;

protected:
	UPROPERTY()
	float m_SensingInterval;
	UPROPERTY()
	float m_fTimer;
	UPROPERTY()
	UDataTable* m_WeaponTable;
	UPROPERTY()
	int m_nMobCount;
	
	TArray<FWeaponDgTableRow*> m_DgDataRow;

	FWeaponDgTableRow* m_CurrentDgData;

	TArray<TSharedPtr<FStreamableHandle>> m_LoadedMonsters;

	FTimerHandle m_TimerHandle_OnTimer;

	UPROPERTY()
	UNavigationSystemV1* m_NavSys;
	UPROPERTY()
	UWorld* m_CurrentWorld;
	UPROPERTY()
	TArray<AMonsterPawn*> m_AryMonsterSpawnedCurrently;

	FSafeInt m_ObtainStoneFromHere;

public:
	void Init();
	
	void RequestMoveWeaponDg(int dgLevel);
	
	virtual void OnLevelLoadComplete(UWorld* world) override;

	virtual void StartDungeon() override;

	virtual void EndDungeon(bool b) override;

	virtual void Tick(float deltaTime) override;

	virtual FString GetOpenLevelAssetName() override;

	void OnMonsterDead(AMonsterPawn* self);

	virtual AUnitPawn* GetNearestEnemy(const FVector& wantPos) override;

	virtual void BeginDestroy() override;
	
	float GetTimePercent();
	//
protected:
	FVector GetRandomPointFromNav(const FVector& loc, const float& radius);

	AMonsterPawn* CreateMob(FVector loc);

	AMonsterPawn* GetReadyMonster();

	void OnTimer();

	void SetTimer(const float TimeDelay);

	void SetSpawnMonsterOnTick(const bool bEnabled);

	void Reset();

	UFUNCTION()
	AMonsterPawn* SpawnMobToLoc(FVector loc);

	void StartSpawn(UWorld* world);

public:
	int GetObtainedStone();

};


