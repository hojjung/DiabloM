#pragma once
#include "DiabloM.h"
#include "MonsterSpawnManager.h"
#include "Characters/Pawns/MonsterPawn.h"

#include "WeaponScrollDgManager.generated.h"


USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FWeaponDgTableRow : public FDungeonDataTableRow
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
class DIABLOM_API UWeaponScrollDgManager : public UMonsterSpawnManager
{
	GENERATED_BODY()

public:
	UWeaponScrollDgManager();

	DECLARE_DELEGATE_OneParam(FOnObtainBounty,int);

	FOnObtainBounty m_OnObtainBounty;

protected:
	FSafeInt m_WeaponStones;
	UPROPERTY()
	float m_fTimer;
	UPROPERTY()
	UDataTable* m_WeaponTable;
	
	TArray<FWeaponDgTableRow*> m_DgDataRow;

	FWeaponDgTableRow* m_CurrentDgData;

public:
	void Init();
	
	void RequestMoveWeaponDg(int dgLevel);
	
	virtual void OnLevelLoadComplete(UWorld* world) override;

	virtual void StartDungeon() override;

	virtual void EndDungeon(bool b) override;

	virtual void Tick(float deltaTime) override;

	virtual FString GetOpenLevelAssetName() override;

	void OnMonsterDead(AMonsterPawn* self);

	
};
