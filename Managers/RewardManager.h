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

protected:
	TQueue<ADroppedItem*> m_PoolItem;
	TQueue<ADroppedGold*> m_PoolGold;
	TQueue<AHealthSphere*> m_PoolHp;

	FVector m_HidingPoint;
public:
	void Init();
	
	void RequestMonsterDropItem(AMonsterPawn* dropActor,const FMonsterItemDropRow& dropData, int level);

	ADroppedItem* DropItemActor(APawn* dropCenterActor,float dropRadius,FItemInstance& myItem);

	ADroppedGold* DropGoldActor(APawn* dropCenterActor,float dropRadius,float goldAmount);

	AHealthSphere* DropHpSphereActor(APawn* dropCenterActor,float dropRadius);

protected:
	void CreateAllItemPool(int itemPoolCount,int goldPoolCount,int hpPoolCount);

	ACollisionInteract* SpawnInteractActor(TSubclassOf<ACollisionInteract> classWant);
	
	ACollisionInteract* DropRandomPoint(APawn* dropCenterActor,float dropRadius,ACollisionInteract* targetActorToDrop,float height,FOnEnd* endCallback=nullptr);
	
	void BezierCurveMove(AActor* target,float height,FVector destination,FOnEnd* endCallback=nullptr);

	FVector GetQuadControlPoint(FVector start,FVector end,float height);

	void EnqueItemActor(ACollisionInteract* collActor);
	void EnqueGoldActor(ACollisionInteract* collActor);
	void EnqueHpSphereActor(ACollisionInteract* collActor);
};


