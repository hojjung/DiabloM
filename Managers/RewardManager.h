// My First Hack n Slash

#pragma once

#include "DiabloM.h"

#include "CActionBaseInterface.h"
#include "NavigationSystem.h"
#include "Characters/MonsterPawn.h"
#include "Item/DroppedGold.h"
#include "Item/DroppedItem.h"
#include "Objs/Actor/HealthSphere.h"
#include "UObject/NoExportTypes.h"
#include "RewardManager.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API URewardManager : public UObject
{
	GENERATED_BODY()
	//아이템 액터 풀링 필요함 렉걸림
public:
	URewardManager();

protected:
	TQueue<ADroppedItem*> m_PoolItem;
	
	TQueue<ADroppedGold*> m_PoolGold;
	
	TQueue<AHealthSphere*> m_PoolHp;

	UPROPERTY()
	TArray<ADroppedItem*> m_AryAllItemActors;
	UPROPERTY()
	TArray<ADroppedGold*> m_AryAllGoldActors;
	UPROPERTY()
	TArray<AHealthSphere*> m_AryAllHpActors;

	FVector m_HidingPoint;
	UPROPERTY()
	int m_nItemIndex;
	UPROPERTY()
	int m_nGoldIndex;
	UPROPERTY()
	int m_nHpIndex;
	//
	UPROPERTY()
	TArray<int> m_AryGoldCount;
	
public:
	void CreateActorPool();
	
	void RequestGoldRop(FVector& dropCenter, float GoldAmount);

	void RequestMonsterDropItem(AMonsterPawn* dropActor, int level);

	ADroppedItem* DropItemActor(FVector& dropCenter,float dropRadius,FItemInstance& myItem);

	ADroppedGold* DropGoldActor(FVector& dropCenter,float dropRadius,float goldAmount);

	AHealthSphere* DropHpSphereActor(FVector& dropCenter ,float dropRadius);

	void EnqueAllActors(bool dgOpen);
	
	ACollisionInteract* GetNearestCollActor(APawn* wantPawn,ACollisionInteract* wantIgnore = nullptr);

	void GetNearestItem(const FVector& PawnPos, ADroppedItem*& outNearItem, float& outNearItemDist,ACollisionInteract* wantIgnore = nullptr);

	void GetNearestGold(const FVector& PawnPos, ADroppedGold*& outNearGold, float& outNearGoldDist,ACollisionInteract* wantIgnore = nullptr);

	void GetNearestHp(const FVector& PawnPos, AHealthSphere*& outNearHpSph, float& outNearHpDist,ACollisionInteract* wantIgnore = nullptr);

protected:
	void CreateAllItemPool(int itemPoolCount,int goldPoolCount,int hpPoolCount);

	ACollisionInteract* SpawnInteractActor(TSubclassOf<ACollisionInteract> classWant);
	
	ACollisionInteract* DropRandomPoint(FVector& dropCenter,float dur,float dropRadius,ACollisionInteract* targetActorToDrop,float height,FOnEnd* endCallback=nullptr);
	
	void BezierCurveMove(AActor* target,float height,FVector destination,float dur,FOnEnd* endCallback=nullptr);

	FVector GetQuadControlPoint(FVector start,FVector end,float height);

	

protected:
	ADroppedItem* CreateDropItemActor();

	ADroppedGold* CreateDropGoldActor();

	AHealthSphere* CreateHealthActor();

protected:
	ADroppedItem* GetDropItemActor();

	ADroppedGold* GetDropGoldActor();

	AHealthSphere* GetDropHealthActor();


protected:
	void EnqueItemActor(ACollisionInteract* collActor);
	
	void EnqueGoldActor(ACollisionInteract* collActor);
	
	void EnqueHpSphereActor(ACollisionInteract* collActor);


};


