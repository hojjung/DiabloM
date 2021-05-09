// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "MonsterSpawnManager.h"
#include "Datas/DungeonDataTable.h"
#include "UObject/NoExportTypes.h"
#include "PetDgManager.generated.h"



USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FPetDgTableRow : public FDungeonDataTableRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nPrizeMagicStoneMin = 15;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nPrizeMagicStoneMax = 25;
};

UCLASS()
class DIABLOM_API UPetDgManager : public UMonsterSpawnManager
{
	GENERATED_BODY()
public:
	UPetDgManager();

	UPROPERTY()
	UDataTable* m_PetTable;

	FPetDgTableRow* m_CurrentTable;

	TArray<FPetDgTableRow*> m_AryTables;

	float m_fTimer;
public:
	void Init();

	void RequestMovePetDg(int dgLevel);
	
	virtual void OnLevelLoadComplete(UWorld* world) override;

	virtual void StartDungeon() override;

	virtual void EndDungeon(bool b) override;

	virtual void Tick(float deltaTime) override;

	virtual FString GetOpenLevelAssetName() override;
};
