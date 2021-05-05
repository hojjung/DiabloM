#pragma once

#include "DiabloM.h"
#include "DungeonManager.h"
#include "Datas/DungeonDataTable.h"
#include "UObject/NoExportTypes.h"
#include "MagicStoneDgManager.generated.h"

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
class DIABLOM_API UMagicStoneDgManager : public UObject
{
	GENERATED_BODY()
	//드래곤 한마리 소환

public:
	UMagicStoneDgManager();

	DECLARE_DELEGATE_OneParam(FOnTick,float);

	FOnTick m_OnTick;

protected:
	UPROPERTY()
	TSubclassOf<UCameraShake> m_ClassShake;
	UPROPERTY()
	UDataTable* m_MagicDgTable;
	UPROPERTY()
	AMonsterPawn* m_SpawnedMagicDragon;
	UPROPERTY()
	int m_nDgLevel;

	TArray<FMagicDgTableRow*> m_DgDataRow;

	FMagicDgTableRow* m_CurrentDgData;

	FDelegateHandle m_LevelLoadHandle;

	bool m_bIsMatchStarted;
	
	float m_fTimer;

public:
	void RequestMoveMagicStoneDg(int dgLevel);

	void OnLevelLoaded(UWorld*);

	void Init();
	
	void Tick(float delta_seconds);
	
	void AddMagicStones(int magicStones);

	void StartMagicDg();

	void EndMagicDgSuccess(AMonsterPawn*);

	void EndMagicDgFail();
};
