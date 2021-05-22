// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "MonsterSpawnManager.h"
#include "Characters/Pawns/MonsterPawn.h"
#include "Datas/DungeonDataTable.h"
#include "UObject/NoExportTypes.h"
#include "PetDgManager.generated.h"

#define PETDGTIME 40.f

class UNavigationSystemV1;
USTRUCT(BlueprintType) //���̵�,Ƽ��
struct FPetDgTableRow : public FDungeonDataTableRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nRewardTicket = 2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nSpawnMobCount = 17;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nTicketCost = 1;
};

UCLASS()
class DIABLOM_API UPetDgManager : public UMonsterSpawnManager
{
	GENERATED_BODY()
public:
	UPetDgManager();

	DECLARE_DELEGATE(FOnMonsterKilled);

	FOnMonsterKilled m_OnMonsterKilled;

	FOnTick m_OnTickTimer;

protected:
	UPROPERTY()
	UWorld* m_CurrentWorld;
	UPROPERTY()
	UDataTable* m_PetTable;
	UPROPERTY()
	UNavigationSystemV1* m_NavSys;
	UPROPERTY()
	TArray<AMonsterPawn*> m_AryMonsterSpawnedCurrently;
	UPROPERTY()
	float m_fTimer;
	UPROPERTY()
	int m_nCurrentKillCount;
	UPROPERTY()
	int m_nMaxKillCount;

	FPetDgTableRow* m_CurrentTable;

	TArray<FPetDgTableRow*> m_AryTables;

	TArray<TSharedPtr<FStreamableHandle>> m_LoadedMonsters;

public:
	void Init();

	virtual void BeginDestroy() override;

	void SpawnPetDgMonsters(int count);

	void RequestMovePetDg(int dgLevel);

	virtual void OnLevelLoadComplete(UWorld* world) override;

	virtual void StartDungeon() override;

	virtual void EndDungeon(bool b) override;

	virtual void Tick(float deltaTime) override;

	virtual FString GetOpenLevelAssetName() override;

	FVector GetRandomPointFromNav(const FVector& loc, const float& radius);

	void AddKillCount(AMonsterPawn*);

	AMonsterPawn* SpawnMobToLoc(FVector loc);

	AMonsterPawn* CreateMob(FVector loc);

	virtual AUnitPawn* GetNearestEnemy(const FVector& wantPos) override;

	int GetCurrentReward();

	float GetMobCountPercent();

	FString GetMobRemainCountStr();

	float GetRemainTime()
	{
		return PETDGTIME - m_fTimer;
	}

	FORCEINLINE const TArray<FPetDgTableRow*>& GetPetDgTables()
	{
		return m_AryTables;
	}

	
	FORCEINLINE const FPetDgTableRow* GetCurrentDg ()
	{
		return m_CurrentTable;
	}
};
