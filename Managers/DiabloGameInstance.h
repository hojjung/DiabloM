#pragma once

#include "DiabloM.h"
#include "Datas/ItemDataTable.h"
#include "Item/ItemManager.h"
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
	UPROPERTY(EditAnywhere)
	TSubclassOf<ADroppedItem> m_DropItemClass;
	
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
	
protected:
	virtual void Init() override;
	
	virtual void Shutdown()override;

public:
	FItemInstance CreateItem(FName id);

	ADroppedItem* DropItemActor(FItemInstance& myItem);

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
	//TEST 1017
	bool m_bTestIsDungeonOpen=false;
	UFUNCTION(BlueprintCallable)
	void TestCreateDungeon();
	UFUNCTION(BlueprintCallable)
    void TestBackToDungeon();
	UFUNCTION(BlueprintCallable)
	void TestSaveAll();
};
