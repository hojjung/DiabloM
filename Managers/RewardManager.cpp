#include "RewardManager.h"
#include "CBezierCurve.h"
#include "DiabloGameInstance.h"
#include "DiabloGameMode.h"
#include "DungeonManager.h"


URewardManager::URewardManager()
{
	
}

void URewardManager::CreateActorPool()
{
	m_HidingPoint = FVector(100000, 100000, 100000);

	m_nItemIndex = 0;
	m_nGoldIndex = 0;
	m_nHpIndex = 0;

	m_AryGoldCount.Reset();
	m_AryGoldCount.Add(0);
	m_AryGoldCount.Add(0);
	m_AryGoldCount.Add(0);
	m_AryGoldCount.Add(0);
	m_AryGoldCount.Add(1);
	m_AryGoldCount.Add(1);
	m_AryGoldCount.Add(1);
	m_AryGoldCount.Add(2);
	m_AryGoldCount.Add(2);
	m_AryGoldCount.Add(3);

	CreateAllItemPool(35, 15, 7); //75/40/4
}

void URewardManager::RequestMonsterDropItem(AMonsterPawn* dropActor, int level)
{
	int MinItemLevel = level - 2;

	MinItemLevel = FMath::Max(MinItemLevel, 1);

	int MaxItemLevel = level + 2;

	MaxItemLevel = FMath::Min(MaxItemLevel,MAXLEVEL);
	//
	//Spawn Gold
	
	float GoldAmount = dropActor->GetAttributeSet()->GetGoldBounty();

	if (GoldAmount > 0.f)
	{
		int CountGoldActor = m_AryGoldCount.GetRandom();

		int IterGold = 0;

		while (IterGold++ < CountGoldActor)
		{
			float NewAmount = FMath::RandRange(0.85f,1.15f)*GoldAmount;
			DropGoldActor(dropActor, 400.f, NewAmount);
		}
	}
	//
	//Spawn Normal Item
	const TArray<FDungeonDropTableHandle>& DropDataAry = UDiabloGameInstance::Get->GetDungeonManager()->GetCurrentDgStageData()->m_AryDgDroptableHandle;

	int DropTableIndex = dropActor->GetMonsterDataTable().m_nDropTableIndex;

	if(DropTableIndex<0 || DropTableIndex>=DropDataAry.Num())
	{
		return;
	}

	const FDungeonDropTable* CurrentDroptable = DropDataAry[DropTableIndex].GetRow<FDungeonDropTable>("RewardManager-DungeonDropHandleNull");

	if(!CurrentDroptable)
	{
		return;
	}

	int Iter = dropActor->GetMonsterDataTable().m_nDroptableRollCount;

	if(Iter<=0)
	{
		return;
	}

	float NormalDropRate=dropActor->GetMonsterDataTable().m_fTierDropBonusNormal;

	float MagicDropRate=dropActor->GetMonsterDataTable().m_fTierDropBonusMagic;

	float RareDropRate=dropActor->GetMonsterDataTable().m_fTierDropBonusRare;

	float LegendDropRate=dropActor->GetMonsterDataTable().m_fTierDropBonusLegend;

	for(int i=0; i<Iter;i++)
	{
		for(const FItemDataHandle& ItemDataHandle :CurrentDroptable->m_AryDropItems)
		{
			FItemData* CurrentItemData = ItemDataHandle.GetRow<FItemData>("");
		
			float ItemPercent = 1.f - (1.f - 1.f / CurrentItemData->m_nDropRateCount);

			FItemTier* Tier = CurrentItemData->m_ItemTierHandle.GetRow<FItemTier>("");

			if(Tier->m_TierID == "Normal")
			{
				ItemPercent+=NormalDropRate;
			}
			else if(Tier->m_TierID == "Magic")
			{
				ItemPercent+=MagicDropRate;
			}
			else if(Tier->m_TierID == "Rare")
			{
				ItemPercent+=RareDropRate;
			}
			else if(Tier->m_TierID == "Legend")
			{
				ItemPercent+=LegendDropRate;
			}
		
			float Rand01 = FMath::RandRange(0.f,1.0f);

			if(ItemPercent < Rand01)
			{
				continue;
			}
			
			int RandItemLevel = FMath::RandRange(MinItemLevel, MaxItemLevel);

			FItemInstance CreatedItem = UDiabloGameInstance::Get->CreateItem(CurrentItemData->m_ItemID,RandItemLevel);

			DropItemActor(dropActor, 400.f, CreatedItem);
		}
	}
	//
}

ADroppedItem* URewardManager::DropItemActor(APawn* dropCenterActor, float dropRadius, FItemInstance& myItem)
{
	ADroppedItem* DropItem = GetDropItemActor();

	myItem.m_Holder = UDiabloGameInstance::Get->GetItemManager();

	DropItem->SetItemInstance(myItem);

	FOnEnd OnDropEnd;
	OnDropEnd.AddUObject(DropItem, &ADroppedItem::DropEnd);

	DropRandomPoint(dropCenterActor, 0.8f, dropRadius, DropItem, 700.f, &OnDropEnd);

	return DropItem;
}

ADroppedGold* URewardManager::DropGoldActor(APawn* dropCenterActor, float dropRadius, float goldAmount)
{
	ADroppedGold* DropGold = GetDropGoldActor();

	DropGold->SetGoldAmount(goldAmount);

	FOnEnd OnDropEnd;
	OnDropEnd.AddUObject(DropGold, &ADroppedGold::DropEnd);

	DropRandomPoint(dropCenterActor, 0.55f, dropRadius, DropGold, 450.f, &OnDropEnd);

	return DropGold;
}

AHealthSphere* URewardManager::DropHpSphereActor(APawn* dropCenterActor, float dropRadius)
{
	AHealthSphere* DropHp = GetDropHealthActor();

	FOnEnd OnDropEnd;
	OnDropEnd.AddUObject(DropHp, &AHealthSphere::DropEnd);

	DropRandomPoint(dropCenterActor, 0.35f, dropRadius, DropHp, 550.f, &OnDropEnd);

	DropHp->SetActorHiddenInGame(false); //이게 문제 아닐까//Set이 없기때문에해줌

	return DropHp;
}

void URewardManager::EnqueAllActors(bool dgOpen) //애초에 이게 문제 될수있는게 풀 밖에 있는지를 히든으로 따질수 없다,
{
	if (dgOpen) //이거때문에 느린듯
	{
		return;
	}

	ADiabloGameMode::Get->GetItemDropActionManager()->CompleteAll();

	m_PoolItem.Empty();

	m_PoolGold.Empty();

	m_PoolHp.Empty();

	for (ADroppedItem* CollActors : m_AryAllItemActors)
	{
		EnqueItemActor(CollActors);
	}

	for (ADroppedGold* CollActors : m_AryAllGoldActors)
	{
		if (!CollActors->IsHidden())
		{
			EnqueGoldActor(CollActors);
		}
	}

	for (AHealthSphere* CollActors : m_AryAllHpActors)
	{
		if (!CollActors->IsHidden())
		{
			EnqueHpSphereActor(CollActors);
		}
	}
}

ACollisionInteract* URewardManager::GetNearestCollActor(APawn* wantPawn,ACollisionInteract* wantIgnore)
{
	FVector PawnPos = wantPawn->GetActorLocation();
	//
	ADroppedItem* NearItem = nullptr;

	float NearItemDist = FLT_MAX;

	GetNearestItem(PawnPos, NearItem, NearItemDist,wantIgnore);
	//
	ADroppedGold* NearGold = nullptr;

	float NearGoldDist = FLT_MAX;

	GetNearestGold(PawnPos, NearGold, NearGoldDist,wantIgnore);
	//
	AHealthSphere* NearHp = nullptr;

	float NearHpDist = FLT_MAX;

	GetNearestHp(PawnPos, NearHp, NearHpDist,wantIgnore);
	//
	ACollisionInteract* FinalNearestActor;
	
	if(NearHpDist <= NearItemDist)
	{		
		FinalNearestActor = NearHp;

		if(NearHpDist > NearGoldDist)
		{
			FinalNearestActor = NearGold;
		}
	}
	else
	{
		FinalNearestActor = NearItem;

		if(NearItemDist >= NearGoldDist)
		{
			FinalNearestActor = NearGold;
		}
	}
	
	return FinalNearestActor;
}

void URewardManager::GetNearestItem(const FVector& PawnPos, ADroppedItem*& outNearItem, float& outNearItemDist,ACollisionInteract* wantIgnore)
{
	if(ADiabloPlayerController::Get->GetInven()->GetEmptyIndex() == -1)//인벤칸없음
	{
		return;
	}
	
	float CurrentNearDist = FLT_MAX;
	
	ADroppedItem* Current = nullptr;

	for (ADroppedItem* DropItem : m_AryAllItemActors)
	{
		if (!DropItem || DropItem->IsHidden() || !DropItem->IsValidLowLevel() || DropItem == wantIgnore ||!DropItem->m_bIsActingForGame)
		{
			continue;
		}

		float NewDist = FVector::DistSquared2D(DropItem->GetActorLocation(), PawnPos);

		if (CurrentNearDist < NewDist) //새로 찾은게 멀어서 무의미
		{
			continue;
		}

		Current = DropItem;

		CurrentNearDist = NewDist;
	}

	outNearItem = Current;
	
	outNearItemDist = CurrentNearDist;
}

void URewardManager::GetNearestGold(const FVector& PawnPos, ADroppedGold*& outNearGold, float& outNearGoldDist,ACollisionInteract* wantIgnore)
{
	float CurrentNearDist = FLT_MAX;
	
	ADroppedGold* Current = nullptr;

	for (ADroppedGold* DropGold : m_AryAllGoldActors)
	{
		if (!DropGold || DropGold->IsHidden() || !DropGold->IsValidLowLevel() || DropGold == wantIgnore||!DropGold->m_bIsActingForGame)
		{
			continue;
		}

		float NewDist = FVector::DistSquared2D(DropGold->GetActorLocation(), PawnPos);

		if (CurrentNearDist < NewDist) //새로 찾은게 멀어서 무의미
		{
			continue;
		}

		Current = DropGold;

		CurrentNearDist = NewDist;
	}

	outNearGold = Current;
	
	outNearGoldDist = CurrentNearDist;
}

void URewardManager::GetNearestHp(const FVector& PawnPos, AHealthSphere*& outNearHpSph, float& outNearHpDist,ACollisionInteract* wantIgnore)
{
	float CurrentNearDist = FLT_MAX;
	
	AHealthSphere* Current = nullptr;

	for (AHealthSphere* DropHp : m_AryAllHpActors)
	{
		if (!DropHp || DropHp->IsHidden() || !DropHp->IsValidLowLevel() || DropHp == wantIgnore||!DropHp->m_bIsActingForGame)
		{
			continue;
		}

		float NewDist = FVector::DistSquared2D(DropHp->GetActorLocation(), PawnPos);

		if (CurrentNearDist < NewDist) //새로 찾은게 멀어서 무의미
		{
			continue;
		}

		Current = DropHp;

		CurrentNearDist = NewDist;
	}

	outNearHpSph = Current;
	
	outNearHpDist = CurrentNearDist;
}


void URewardManager::CreateAllItemPool(int itemPoolCount, int goldPoolCount, int hpPoolCount)
{
	m_PoolItem.Empty();
	m_PoolGold.Empty();
	m_PoolHp.Empty();

	m_AryAllItemActors.Reset(100);
	m_AryAllGoldActors.Reset(100);
	m_AryAllHpActors.Reset(100);

	int i = 0;

	while (i++ < itemPoolCount)
	{
		EnqueItemActor(CreateDropItemActor());
	}

	i = 0;
	while (i++ < goldPoolCount)
	{
		EnqueGoldActor(CreateDropGoldActor());
	}

	i = 0;
	while (i++ < hpPoolCount)
	{
		EnqueHpSphereActor(CreateHealthActor());
	}
}

ACollisionInteract* URewardManager::DropRandomPoint(APawn* dropCenterActor, float dur, float dropRadius,
                                                    ACollisionInteract* targetActorToDrop, float height,
                                                    FOnEnd* endCallback)
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(dropCenterActor->GetWorld());

	if (!NavSys)
	{
		PRINTF("RewardManager-Fail-NoNav");

		return nullptr;
	}

	FVector PosWant = dropCenterActor->GetMovementComponent()->GetActorFeetLocation();//for path find

	targetActorToDrop->SetActorLocation(PosWant);

	FVector NewPos = PosWant;

	FNavLocation OutLoc;

	if (NavSys->GetRandomPointInNavigableRadius(PosWant, dropRadius, OutLoc))
	{
		NewPos = OutLoc.Location;
	}

	BezierCurveMove(targetActorToDrop, height, NewPos, dur, endCallback);

	targetActorToDrop->m_bIsActingForGame=true;

	return targetActorToDrop;
}

void URewardManager::BezierCurveMove(AActor* target, float height, FVector destination, float dur, FOnEnd* endCallback)
{
	FVector StartPoint = target->GetActorLocation();
	UCBezierCurve* Curve = UCActionFactory::MakeCurve();
	FVector ControlPoint = GetQuadControlPoint(StartPoint, destination, height);
	Curve->InitializeQuad(StartPoint, ControlPoint, destination);

	UCFollowBezierCurvePathAction* PathAction = UCActionFactory::MakeFollowBezierPathAction(target, Curve, dur);

	if (endCallback)
	{
		PathAction->m_OnComplete = *endCallback;
	}

	ADiabloGameMode::Get->GetItemDropActionManager()->AddAction(PathAction);
}

FVector URewardManager::GetQuadControlPoint(FVector start, FVector end, float height)
{
	FVector Result;

	Result.X = start.X + ((end.X - start.X) / 2.f);
	Result.Y = start.Y + ((end.Y - start.Y) / 2.f);
	Result.Z = start.Z + ((end.Z - start.Z) / 2.f);
	Result.Z += height;
	return Result;
}

ACollisionInteract* URewardManager::SpawnInteractActor(TSubclassOf<ACollisionInteract> classWant)
{
	FActorSpawnParameters Param;
	
	Param.bNoFail = true;

	FRotator Rot(0.f,0.f,0.f);
	
	ACollisionInteract* DroppedActor = UDiabloGameInstance::Get->GetWorld()->SpawnActor<ACollisionInteract>(classWant,m_HidingPoint,Rot,Param);

	return DroppedActor;
}

ADroppedItem* URewardManager::CreateDropItemActor()
{
	ADroppedItem* Created = Cast<ADroppedItem>(SpawnInteractActor(ADroppedItem::StaticClass()));
	Created->GetOnTaskEnd().AddUObject(this, &URewardManager::EnqueItemActor);
	m_AryAllItemActors.Emplace(Created);
	return Created;
}

ADroppedGold* URewardManager::CreateDropGoldActor()
{
	ADroppedGold* Created = Cast<ADroppedGold>(SpawnInteractActor(ADroppedGold::StaticClass()));
	Created->GetOnTaskEnd().AddUObject(this, &URewardManager::EnqueGoldActor);
	m_AryAllGoldActors.Emplace(Created);
	return Created;
}

AHealthSphere* URewardManager::CreateHealthActor()
{
	AHealthSphere* Created = Cast<AHealthSphere>(SpawnInteractActor(AHealthSphere::StaticClass()));
	Created->GetOnTaskEnd().AddUObject(this, &URewardManager::EnqueHpSphereActor);
	m_AryAllHpActors.Emplace(Created);
	return Created;
}

void URewardManager::EnqueItemActor(ACollisionInteract* collActor)
{
	Cast<ITickHideable>(collActor)->HideAll();
	m_PoolItem.Enqueue(Cast<ADroppedItem>(collActor));
	collActor->SetActorLocation(m_HidingPoint);
	
	collActor->m_bIsActingForGame=false;
}

void URewardManager::EnqueGoldActor(ACollisionInteract* collActor)
{
	Cast<ITickHideable>(collActor)->HideAll();
	m_PoolGold.Enqueue(Cast<ADroppedGold>(collActor));
	collActor->SetActorLocation(m_HidingPoint);

	collActor->m_bIsActingForGame=false;
}

void URewardManager::EnqueHpSphereActor(ACollisionInteract* collActor)
{
	Cast<ITickHideable>(collActor)->HideAll();
	m_PoolHp.Enqueue(Cast<AHealthSphere>(collActor));
	collActor->SetActorLocation(m_HidingPoint);

	collActor->m_bIsActingForGame=false;
}

ADroppedItem* URewardManager::GetDropItemActor()
{
	ADroppedItem* DropItem;

	if (!m_PoolItem.Dequeue(DropItem) || !DropItem)
	{
		PRINTF("NoItemActor,Deque");

		DropItem = m_AryAllItemActors[m_nItemIndex++];

		Cast<ITickHideable>(DropItem)->HideAll();

		DropItem->SetActorLocation(m_HidingPoint);

		if (m_nItemIndex >= m_AryAllItemActors.Num())
		{
			m_nItemIndex = 0;
		}
	}


	return DropItem;
}

ADroppedGold* URewardManager::GetDropGoldActor()
{
	ADroppedGold* DropGold;

	if (!m_PoolGold.Dequeue(DropGold) || !DropGold)
	{
		PRINTF("NoGoldActor,Deque");

		DropGold = m_AryAllGoldActors[m_nGoldIndex++];

		Cast<ITickHideable>(DropGold)->HideAll();

		DropGold->SetActorLocation(m_HidingPoint);

		if (m_nGoldIndex >= m_AryAllGoldActors.Num())
		{
			m_nGoldIndex = 0;
		}
	}

	return DropGold;
}

AHealthSphere* URewardManager::GetDropHealthActor()
{
	AHealthSphere* DropHpSphere;

	if (!m_PoolHp.Dequeue(DropHpSphere) || !DropHpSphere)
	{
		PRINTF("NoHealthActor,Deque");

		DropHpSphere = m_AryAllHpActors[m_nHpIndex++];

		Cast<ITickHideable>(DropHpSphere)->HideAll();

		DropHpSphere->SetActorLocation(m_HidingPoint);

		if (m_nHpIndex >= m_AryAllHpActors.Num())
		{
			m_nHpIndex = 0;
		}
	}

	return DropHpSphere;
}
