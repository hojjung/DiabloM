#pragma once

#include "DiabloM.h"
#include "DungeonManager.h"
#include "MonsterSpawnManager.h"
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
};


UCLASS()
class DIABLOM_API UMagicStoneDgManager : public UMonsterSpawnManager
{
	GENERATED_BODY()
	//드래곤 한마리 소환

public:
	UMagicStoneDgManager();

	DECLARE_DELEGATE_OneParam(FOnTick,float);

	DECLARE_DELEGATE_OneParam(FOnDragonSpawned,AMonsterPawn*);

	FOnTick m_OnTick;
	
	FOnDragonSpawned m_OnDragonSpawned;

	UPROPERTY()
	AMonsterPawn* m_SpawnedMagicDragon;
protected:
	UPROPERTY()
	TSubclassOf<UCameraShake> m_ClassShake;
	UPROPERTY()
	UDataTable* m_MagicDgTable;
	
	UPROPERTY()
	int m_nDgLevel;

	TArray<FMagicDgTableRow*> m_DgDataRow;

	FMagicDgTableRow* m_CurrentDgData;

	FDelegateHandle m_LevelLoadHandle;
	
	
	float m_fTimer;

	FTimerHandle m_TimerHandle_OnTimer;
	UPROPERTY()
	int m_nSuccessBounty;
	UPROPERTY()
	int m_nFailBounty;
	UPROPERTY()
	int m_nCurrentMagicStone;

public:
	void RequestMoveMagicStoneDg(int dgLevel);

	void Init();
	
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

	void MoveToNormalDungeon();
};
