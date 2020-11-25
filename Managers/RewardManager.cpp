#include "RewardManager.h"
#include "CBezierCurve.h"
#include "DiabloGameInstance.h"
#include "DiabloGameMode.h"
#include "Datas/MonsterItemDropTable.h"

void URewardManager::CreateActorPool()
{
    m_HidingPoint = FVector(100000, 100000, 100000);
    CreateAllItemPool(75, 75, 75);
}

void URewardManager::RequestMonsterDropItem(AMonsterPawn* dropActor, const FMonsterItemDropRow& dropData, int level)
{
    int MinItemLevel = level-2;
                    
    MinItemLevel=FMath::Max(MinItemLevel,1);
                    
    int MaxItemLevel= level+2;

    MaxItemLevel=FMath::Min(MaxItemLevel,MAXLEVEL);

    float MagicItemBonus=0.f;
                    
    float RareItemBonus=0.f;
                    
    float EpicItemBonus=0.f;

    float UniqueItemBonus=0.f;

    //
    
    int CountHp = dropData.m_AryHpDropRand.GetRandom();

    int IterHp = 0;

    while (IterHp++ < CountHp)
    {
        DropHpSphereActor(dropActor, 600.f);
    }
    //
    //Spawn Gold
    int GoldAmount = dropActor->GetAttributeSet()->GetGoldBounty();

    if (GoldAmount > 0)
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
        
        float MaxRate=0.f;
        
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
                    int RandItemLevel = FMath::RandRange(MinItemLevel,MaxItemLevel);
                    
                    FItemInstance CreatedItem = UDiabloGameInstance::Get->CreateItem(ItemDataFromTable.m_DropHandle.RowName,MagicItemBonus,RareItemBonus,EpicItemBonus,RandItemLevel);
                    
                    DropItemActor(dropActor,400.f,CreatedItem);
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
        
        DropPercentMaxOne*=(1.f+UniqueItemBonus);
        
        float RandPercent = FMath::RandRange(0.f, 1.f);

        if (DropPercentMaxOne < RandPercent)
        {
            continue;
        }

        const FUniqueEquipData* UniqueData = dropData.m_AropDropUniqueItems[IterUnique].m_DropHandle.GetRow<FUniqueEquipData>("");

        int RandItemLevel = FMath::RandRange(MinItemLevel,MaxItemLevel);

        FItemInstance CreatedUniqueItem = UDiabloGameInstance::Get->CreateUniqueItem(UniqueData,RandItemLevel);

        DropItemActor(dropActor, 300.f, CreatedUniqueItem);

        IterUnique++;
    }
}

ADroppedItem* URewardManager::DropItemActor(APawn* dropCenterActor, float dropRadius, FItemInstance& myItem)
{
    ADroppedItem* DropItem=nullptr;

    if (!m_PoolItem.Dequeue(DropItem)||!DropItem)
    {
        DropItem = Cast<ADroppedItem>(SpawnInteractActor(UMonsterItemDropTable::ClassDropItemActor));
        DropItem->GetOnTaskEnd().AddUObject(this, &URewardManager::EnqueItemActor);
        m_AryAllPoolItem.Emplace(DropItem);
    }

    if(myItem.IsEmpty())
    {
        PRINTF("ISEmpty Item");
    }

    myItem.m_Holder = UDiabloGameInstance::Get->GetItemManager();

    DropItem->SetItemInstance(myItem);

    DropItem->SetActorHiddenInGame(false);

    FOnEnd OnDropEnd;

    OnDropEnd.AddUObject(DropItem, &ADroppedItem::DropEnd);

    DropRandomPoint(dropCenterActor, dropRadius, DropItem, 700.f, &OnDropEnd);

    return DropItem;
}

ADroppedGold* URewardManager::DropGoldActor(APawn* dropCenterActor, float dropRadius, float goldAmount)
{
    ADroppedGold* DropGold=nullptr;

    if (!m_PoolGold.Dequeue(DropGold)||!DropGold)
    {
        DropGold = Cast<ADroppedGold>(SpawnInteractActor(UMonsterItemDropTable::ClassDropGoldActor));
        DropGold->GetOnTaskEnd().AddUObject(this, &URewardManager::EnqueGoldActor);
        m_AryAllPoolGold.Emplace(DropGold);
    }

    DropGold->SetGoldAmount(goldAmount);

    DropGold->SetActorHiddenInGame(false);

    FOnEnd OnDropEnd;
    OnDropEnd.AddUObject(DropGold, &ADroppedGold::DropEnd);

    DropRandomPoint(dropCenterActor, dropRadius, DropGold, 450.f, &OnDropEnd);

    return DropGold;
}

AHealthSphere* URewardManager::DropHpSphereActor(APawn* dropCenterActor, float dropRadius)
{
    AHealthSphere* DropHp=nullptr;

    if (!m_PoolHp.Dequeue(DropHp)||!DropHp)
    {
        DropHp = Cast<AHealthSphere>(SpawnInteractActor(UMonsterItemDropTable::ClassDropHealthSphere));
        DropHp->GetOnTaskEnd().AddUObject(this, &URewardManager::EnqueHpSphereActor);
        m_AryAllPoolHp.Emplace(DropHp);
    }

    DropHp->SetActorHiddenInGame(false);

    DropRandomPoint(dropCenterActor, dropRadius, DropHp, 450.f);

    return DropHp;
}

void URewardManager::HideAllActor(bool dgOpen)
{
    if(dgOpen)
    {
        return;
    }
    
    for(ACollisionInteract* DropedItem :m_AryAllPoolItem)
    {
        if(!DropedItem->IsHidden())
        {
            EnqueItemActor(DropedItem);
        }
    }

    for(ACollisionInteract* DropedGold :m_AryAllPoolGold)
    {
        if(!DropedGold->IsHidden())
        {
            EnqueItemActor(DropedGold);
        }
    }

    for(ACollisionInteract* DropedHp :m_AryAllPoolHp)
    {
        if(!DropedHp->IsHidden())
        {
            EnqueItemActor(DropedHp);
        }
    }
}

void URewardManager::CreateAllItemPool(int itemPoolCount, int goldPoolCount, int hpPoolCount)
{
    m_PoolItem.Empty();
    m_PoolGold.Empty();
    m_PoolHp.Empty();
    m_AryAllPoolItem.Empty(itemPoolCount);
    m_AryAllPoolGold.Empty(goldPoolCount);
    m_AryAllPoolHp.Empty(hpPoolCount);
    
    
    int i = 0;
    
    while (i++ < itemPoolCount)
    {
        ADroppedItem* Created = Cast<ADroppedItem>(SpawnInteractActor(UMonsterItemDropTable::ClassDropItemActor));
        Created->GetOnTaskEnd().AddUObject(this, &URewardManager::EnqueItemActor);
        Created->SetActorHiddenInGame(true);
        m_PoolItem.Enqueue(Created);
        m_AryAllPoolItem.Emplace(Created);
    }

    i = 0;
    while (i++ < goldPoolCount)
    {
        ADroppedGold* Created = Cast<ADroppedGold>(SpawnInteractActor(UMonsterItemDropTable::ClassDropGoldActor));
        Created->GetOnTaskEnd().AddUObject(this, &URewardManager::EnqueGoldActor);
        Created->SetActorHiddenInGame(true);
        m_PoolGold.Enqueue(Created);
        m_AryAllPoolGold.Emplace(Created);
    }

    i = 0;
    while (i++ < hpPoolCount)
    {
        AHealthSphere* Created = Cast<AHealthSphere>(SpawnInteractActor(UMonsterItemDropTable::ClassDropHealthSphere));
        Created->GetOnTaskEnd().AddUObject(this, &URewardManager::EnqueHpSphereActor);
        Created->SetActorHiddenInGame(true);
        m_PoolHp.Enqueue(Created);
        m_AryAllPoolHp.Emplace(Created);
    }
}

ACollisionInteract* URewardManager::SpawnInteractActor(TSubclassOf<ACollisionInteract> classWant)
{
    FActorSpawnParameters Param;
    Param.bNoFail = true;

    ACollisionInteract* DroppedActor = UDiabloGameInstance::Get->GetWorld()->SpawnActor<ACollisionInteract>(
        classWant, Param);
    DroppedActor->SetActorLocation(m_HidingPoint);

    return DroppedActor;
}

ACollisionInteract* URewardManager::DropRandomPoint(APawn* dropCenterActor, float dropRadius,
                                                    ACollisionInteract* targetActorToDrop, float height,
                                                    FOnEnd* endCallback)
{
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(dropCenterActor->GetWorld());

    if (!NavSys)
    {
        PRINTF("RewardManager-Fail-NoNav");

        return nullptr;
    }

    FVector PosWant = dropCenterActor->GetMovementComponent()->GetActorFeetLocation();

    targetActorToDrop->SetActorLocation(PosWant);

    FVector NewPos = PosWant;

    FNavLocation OutLoc;

    if (NavSys->GetRandomPointInNavigableRadius(PosWant, dropRadius, OutLoc))
    {
        NewPos = OutLoc.Location;
    }

    BezierCurveMove(targetActorToDrop, height, NewPos, endCallback);

    return targetActorToDrop;
}

void URewardManager::BezierCurveMove(AActor* target, float height, FVector destination, FOnEnd* endCallback)
{
    FVector StartPoint = target->GetActorLocation();
    UCBezierCurve* Curve = UCActionFactory::MakeCurve();
    FVector ControlPoint = GetQuadControlPoint(StartPoint, destination, height);
    Curve->InitializeQuad(StartPoint, ControlPoint, destination);

    UCFollowBezierCurvePathAction* PathAction = UCActionFactory::MakeFollowBezierPathAction(target, Curve, 0.75f);

    if (endCallback)
    {
        PathAction->m_OnComplete = *endCallback;
    }

    ADiabloGameMode::Get->GetActionManager()->RunAction(PathAction);
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

void URewardManager::EnqueItemActor(ACollisionInteract* collActor)
{
    collActor->SetActorHiddenInGame(true);
    m_PoolItem.Enqueue(Cast<ADroppedItem>(collActor));
    collActor->SetActorLocation(m_HidingPoint);
}

void URewardManager::EnqueGoldActor(ACollisionInteract* collActor)
{
    collActor->SetActorHiddenInGame(true);
    m_PoolGold.Enqueue(Cast<ADroppedGold>(collActor));
    collActor->SetActorLocation(m_HidingPoint);
}

void URewardManager::EnqueHpSphereActor(ACollisionInteract* collActor)
{
    collActor->SetActorHiddenInGame(true);
    m_PoolHp.Enqueue(Cast<AHealthSphere>(collActor));
    collActor->SetActorLocation(m_HidingPoint);
}
