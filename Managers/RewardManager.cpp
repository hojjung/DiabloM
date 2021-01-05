#include "RewardManager.h"
#include "CBezierCurve.h"
#include "DiabloGameInstance.h"
#include "DiabloGameMode.h"
#include "Datas/MonsterItemDropTable.h"


void URewardManager::CreateActorPool()
{
	m_HidingPoint = FVector(100000, 100000, 100000);

	m_nItemIndex = 0;
	m_nGoldIndex = 0;
	m_nHpIndex = 0;

	CreateAllItemPool(75, 40, 4); //75/40/4
}

void URewardManager::RequestMonsterDropItem(AMonsterPawn* dropActor, const FMonsterItemDropRow& dropData, int level)
{
	int MinItemLevel = level - 2;

	MinItemLevel = FMath::Max(MinItemLevel, 1);

	int MaxItemLevel = level + 2;

	MaxItemLevel = FMath::Min(MaxItemLevel,MAXLEVEL);

	float MagicItemBonus = 0.f;

	float RareItemBonus = 0.f;

	float EpicItemBonus = 0.f;

	float UniqueItemBonus = 0.f;
	//
	int CountHp = dropData.m_AryHpDropRand.GetRandom();

	int IterHp = 0;

	while (IterHp++ < CountHp)
	{
		DropHpSphereActor(dropActor, 600.f);
	}
	//
	//Spawn Gold
	float GoldAmount = dropActor->GetAttributeSet()->GetGoldBounty();

	if (GoldAmount > 0.f)
	{
		int CountGoldActor = FMath::RandRange(1, 3);
		float GoldAmountEach = GoldAmount / CountGoldActor;

		int IterGold = 0;

		while (IterGold++ < CountGoldActor)
		{
			DropGoldActor(dropActor, 400.f, GoldAmountEach);
		}
	}
	//
	//Spawn Normal Item

	int CountItem = dropData.m_AryItemDropRand.GetRandom();

	if (CountItem > 0)
	{
		int IterItem = 0;

		float MaxRate = 0.f;

		//Max Setting
		for (auto& ItemDataFromTable : dropData.m_AryDropItems)
		{
			MaxRate += ItemDataFromTable.m_fDropRatePriority;
		}

		//아이템 떨구는 개수
		while (IterItem++ < CountItem)
		{
			float DropRateCount = 0.f;

			float RandomValue = FMath::RandRange(0.f, MaxRate);

			for (const FItemDropData& ItemDataFromTable : dropData.m_AryDropItems)
			{
				DropRateCount += ItemDataFromTable.m_fDropRatePriority;

				if (DropRateCount >= RandomValue)
				{
					int RandItemLevel = FMath::RandRange(MinItemLevel, MaxItemLevel);

					FItemInstance CreatedItem = UDiabloGameInstance::Get->CreateItem(
						ItemDataFromTable.m_DropHandle.RowName, MagicItemBonus, RareItemBonus, EpicItemBonus,
						RandItemLevel);

					DropItemActor(dropActor, 400.f, CreatedItem);
				}
			}
		}
	}
	//
	//Spawn Unique Item
	int CountUniqueItem = dropData.m_AropDropUniqueItems.Num();

	int IterUnique = 0;

	while (IterUnique < CountUniqueItem)
	{
		float DropPercentMaxOne = dropData.m_AropDropUniqueItems[IterUnique].m_fDropRateOnePerMax;

		DropPercentMaxOne *= (1.f + UniqueItemBonus);

		float RandPercent = FMath::RandRange(0.f, 1.f);

		if (DropPercentMaxOne < RandPercent)
		{
			continue;
		}

		const FUniqueEquipData* UniqueData = dropData.m_AropDropUniqueItems[IterUnique].m_DropHandle.GetRow<
			FUniqueEquipData>("");

		int RandItemLevel = FMath::RandRange(MinItemLevel, MaxItemLevel);

		FItemInstance CreatedUniqueItem = UDiabloGameInstance::Get->CreateUniqueItem(UniqueData, RandItemLevel);
		DropItemActor(dropActor, 300.f, CreatedUniqueItem);

		IterUnique++;
	}
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

	ADiabloGameMode::Get->GetItemActionManager()->CompleteAll();

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
		if (!DropItem || DropItem->IsHidden() || !DropItem->IsValidLowLevel() || DropItem == wantIgnore)
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
		if (!DropGold || DropGold->IsHidden() || !DropGold->IsValidLowLevel() || DropGold == wantIgnore)
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
		if (!DropHp || DropHp->IsHidden() || !DropHp->IsValidLowLevel() || DropHp == wantIgnore)
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

	ADiabloGameMode::Get->GetItemActionManager()->RunAction(PathAction);
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
	ADroppedItem* Created = Cast<ADroppedItem>(SpawnInteractActor(UMonsterItemDropTable::ClassDropItemActor));
	Created->GetOnTaskEnd().AddUObject(this, &URewardManager::EnqueItemActor);
	m_AryAllItemActors.Emplace(Created);
	return Created;
}

ADroppedGold* URewardManager::CreateDropGoldActor()
{
	ADroppedGold* Created = Cast<ADroppedGold>(SpawnInteractActor(UMonsterItemDropTable::ClassDropGoldActor));
	Created->GetOnTaskEnd().AddUObject(this, &URewardManager::EnqueGoldActor);
	m_AryAllGoldActors.Emplace(Created);
	return Created;
}

AHealthSphere* URewardManager::CreateHealthActor()
{
	AHealthSphere* Created = Cast<AHealthSphere>(SpawnInteractActor(UMonsterItemDropTable::ClassDropHealthSphere));
	Created->GetOnTaskEnd().AddUObject(this, &URewardManager::EnqueHpSphereActor);
	m_AryAllHpActors.Emplace(Created);
	return Created;
}

void URewardManager::EnqueItemActor(ACollisionInteract* collActor)
{
	Cast<ITickHideable>(collActor)->HideAll(false);
	m_PoolItem.Enqueue(Cast<ADroppedItem>(collActor));
	collActor->SetActorLocation(m_HidingPoint);
}

void URewardManager::EnqueGoldActor(ACollisionInteract* collActor)
{
	Cast<ITickHideable>(collActor)->HideAll(false);
	m_PoolGold.Enqueue(Cast<ADroppedGold>(collActor));
	collActor->SetActorLocation(m_HidingPoint);
}

void URewardManager::EnqueHpSphereActor(ACollisionInteract* collActor)
{
	Cast<ITickHideable>(collActor)->HideAll(false);
	m_PoolHp.Enqueue(Cast<AHealthSphere>(collActor));
	collActor->SetActorLocation(m_HidingPoint);
}

ADroppedItem* URewardManager::GetDropItemActor()
{
	ADroppedItem* DropItem;

	if (!m_PoolItem.Dequeue(DropItem) || !DropItem)
	{
		PRINTF("NoItemActor,Deque");

		DropItem = m_AryAllItemActors[m_nItemIndex++];

		Cast<ITickHideable>(DropItem)->HideAll(false);

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

		Cast<ITickHideable>(DropGold)->HideAll(false);

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

		Cast<ITickHideable>(DropHpSphere)->HideAll(false);

		DropHpSphere->SetActorLocation(m_HidingPoint);

		if (m_nHpIndex >= m_AryAllHpActors.Num())
		{
			m_nHpIndex = 0;
		}
	}

	return DropHpSphere;
}
