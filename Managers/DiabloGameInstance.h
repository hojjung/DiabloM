#pragma once

#include "DiabloM.h"
#include "RewardManager.h"
#include "Datas/ItemDataTable.h"
#include "Item/DroppedGold.h"
#include "Item/ItemManager.h"
#include "Objs/Actor/HealthSphere.h"
#include "SaveLoad/SaveLoadManager.h"
#include "DiabloGameInstance.generated.h"


class APlayerVisual;
class UStartMenuCanvas;
class UPlayerCreateManager;
class UMonsterSpawnManager;
class UDungeonManager;
struct FItemInstance;
struct FItemTier;
struct FItemTier;

UCLASS()
class DIABLOM_API UDiabloGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UDiabloGameInstance();//
	static UDiabloGameInstance* Get;
	
protected:
	UPROPERTY()
	UItemManager* m_ItemManager;
	UPROPERTY()
	USaveLoadManager* m_SaveLoadManager;
	UPROPERTY()
	UPlayerCreateManager* m_PlCreateManager;
	UPROPERTY()
	UMonsterSpawnManager* m_MonsterSpawn;
	UPROPERTY()
	UDungeonManager* m_DungeonManager;
	UPROPERTY()
	URewardManager* m_RewardManager;
	
protected:
	virtual void Init() override;
	
	virtual void Shutdown()override;

public:
	FItemInstance CreateItem(FName id,float magicItemBonus=0.f,float rareItemBonus=0.f,float epicItemBonus=0.f,int itemLevel=1.f);

	FItemInstance CreateUniqueItem(const FUniqueEquipData* uniqueItem,int itemLevel=1);

	ADroppedItem* DropItemActor(APawn* dropCenterActor,float dropRadius,FItemInstance& myItem);

	FORCEINLINE UItemManager* GetItemManager()
	{
		return m_ItemManager;
	}
	FORCEINLINE USaveLoadManager* GetSaveLoadManager()
	{
		return m_SaveLoadManager;
	}
	FORCEINLINE UPlayerCreateManager* GetPlCreateManager()
	{
		return    m_PlCreateManager;
	}
	FORCEINLINE UMonsterSpawnManager* GetMonsterSpawn()
	{
		return   m_MonsterSpawn;
	}
	FORCEINLINE UDungeonManager* GetDungeonManager()
	{
		return   m_DungeonManager;
	}
	FORCEINLINE URewardManager* GetRewardManager()
	{
		return   m_RewardManager;
	}
	//TEST 1017
	bool m_bTestIsDungeonOpen=false;
	UFUNCTION(BlueprintCallable)
	void CreateDungeon();
	UFUNCTION(BlueprintCallable)
    void BackToDungeon();
	UFUNCTION(BlueprintCallable)
	void SaveAllPlayerInfo();
	UFUNCTION(BlueprintCallable)
	bool IsDungeonOpened();
};
