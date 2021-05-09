#pragma once

#include "DiabloM.h"
#include "MonsterSpawnManager.h"
#include "Characters/Pawns/MonsterPawn.h"
#include "Datas/DungeonDataTable.h"
#include "UObject/NoExportTypes.h"
#include "MagicStoneDgManager.generated.h"

#define MAGICDGTIME 50

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FMagicDgTableRow : public FDungeonDataTableRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nPrizeMagicStoneMin = 15;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nPrizeMagicStoneMax = 25;

	int GetRandomPrize()
	{
		return FMath::RandRange(m_nPrizeMagicStoneMin,m_nPrizeMagicStoneMax);
	}
};


UCLASS()
class DIABLOM_API UMagicStoneDgManager : public UMonsterSpawnManager
{
	GENERATED_BODY()
	//드래곤 한마리 소환

public:
	UMagicStoneDgManager();

	DECLARE_DELEGATE_OneParam(FOnDragonSpawned,AMonsterPawn*);

	FOnDragonSpawned m_OnDragonSpawned;

	UPROPERTY()
	AMonsterPawn* m_SpawnedMagicDragon;
protected:
	UPROPERTY()
	UDataTable* m_MagicDgTable;
	

	TArray<FMagicDgTableRow*> m_DgDataRow;

	FMagicDgTableRow* m_CurrentDgData;

	float m_fTimer;

	FTimerHandle m_TimerHandle_OnTimer;
	UPROPERTY()
	int m_nSuccessBounty;
	UPROPERTY()
	int m_nFailBounty;
	UPROPERTY()
	int m_nCurrentMagicStone;

public:
	void Init();
	
	void RequestMoveMagicStoneDg(int dgLevel);
	
	virtual void OnLevelLoadComplete(UWorld* world) override;

	virtual void StartDungeon() override;

	virtual void EndDungeon(bool b) override;

	virtual FString GetOpenLevelAssetName() override;

	virtual void Tick(float delta_seconds) override;

	virtual bool IsBattleStarted() override;
	
	void AddMagicStones(int magicStones);

	void EndMagicDgSuccess(AMonsterPawn*);

	void EndMagicDgFail();

	int GetResultBounty();

	int GetFailBounty();
};
