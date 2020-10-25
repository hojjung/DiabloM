#pragma once

#include "DiabloM.h"
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
	UPROPERTY(EditAnywhere)
	TSubclassOf<ADroppedItem> m_DropItemClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ADroppedGold> m_DropGoldClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AHealthSphere> m_DropHpSphereClass;
	
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

	ACollisionInteract* SpawnDropCollInteract(TSubclassOf<ACollisionInteract> classWant,FVector posWant);
public:
	FItemInstance CreateItem(FName id);

	ADroppedItem* DropItemActor(APawn* dropCenterActor,float dropRadius,FItemInstance& myItem);

	ADroppedItem* DropItemActor(FVector dropCenterPos,UNavigationSystemV1* nav,float dropRadius,FItemInstance& myItem);

	ADroppedGold* DropGoldActor(APawn* dropCenterActor,float dropRadius);

	AHealthSphere* DropHpSphereActor(APawn* dropCenterActor,float dropRadius);
	

	
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
	void CreateDungeon();
	UFUNCTION(BlueprintCallable)
    void BackToDungeon();
	UFUNCTION(BlueprintCallable)
	void SaveAllPlayerInfo();
	UFUNCTION(BlueprintCallable)
	bool IsDungeonOpened();
};
