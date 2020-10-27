#include "RewardManager.h"
#include "CBezierCurve.h"
#include "DiabloGameInstance.h"
#include "DiabloGameMode.h"
#include "Datas/MonsterItemDropTable.h"

void URewardManager::Init()
{
    m_HidingPoint=FVector(100000,100000,100000);
    CreateAllItemPool(100,150,50);
}

void URewardManager::RequestMonsterDropItem(APawn* dropActor,const FMonsterItemDropRow& dropData,int level)
{
    int CountHp = dropData.m_AryHpDropRand.GetRandom();
    int IterHp=0;

    while (IterHp++<CountHp)
    {
        DropHpSphereActor(dropActor,600.f);
    }
    //

    int CountItem = dropData.m_AryDropItems.GetRandom();
    int IterHp=0;

    UItemDataTable::GetTierTable->GetAllRows("Error", m_AryItemTier);

    for (auto* ItemTier : m_AryItemTier)
    {
        m_fTierMaxRate += ItemTier->m_fDefaultDropRate;
    }

    float RandomValue = FMath::RandRange(0.f, m_fTierMaxRate);
    //76/100
    float DropRateCount = 0.f;

    for (auto* TierData : m_AryItemTier)
    {
        DropRateCount += TierData->m_fDefaultDropRate;

        if (DropRateCount >= RandomValue)
        {
            return *TierData;
        }
    }

}

ADroppedItem* URewardManager::DropItemActor(APawn* dropCenterActor, float dropRadius, FItemInstance& myItem)
{
    ADroppedItem* DropItem;
    
    if(!m_PoolItem.Dequeue(DropItem))
    {
        DropItem = Cast<ADroppedItem>(SpawnInteractActor(UMonsterItemDropTable::ClassDropItemActor));
        DropItem->GetOnTaskEnd().AddUObject(this,&URewardManager::EnqueItemActor);
    }

    myItem.m_Holder=UDiabloGameInstance::Get->GetItemManager();
    
    DropItem->SetItemInstance(myItem);
    
    DropItem->SetActorHiddenInGame(false);

    FOnEnd OnDropEnd;
    
    OnDropEnd.AddUObject(DropItem,&ADroppedItem::DropEnd);
    
    DropRandomPoint(dropCenterActor,dropRadius,DropItem,600.f,&OnDropEnd);

    return DropItem;
}

ADroppedGold* URewardManager::DropGoldActor(APawn* dropCenterActor,float dropRadius,float goldAmount)
{
   ADroppedGold* DropGold;
    
   if(!m_PoolGold.Dequeue(DropGold))
   {
       DropGold = Cast<ADroppedGold>(SpawnInteractActor(UMonsterItemDropTable::ClassDropGoldActor));
       DropGold->GetOnTaskEnd().AddUObject(this,&URewardManager::EnqueGoldActor);
   }

    DropGold->SetGoldAmount(goldAmount);
    
    DropGold->SetActorHiddenInGame(false);

    FOnEnd OnDropEnd;
    OnDropEnd.AddUObject(DropGold,&ADroppedGold::DropEnd);
    
    DropRandomPoint(dropCenterActor,dropRadius,DropGold,600.f,&OnDropEnd);

    return DropGold;
}

AHealthSphere* URewardManager::DropHpSphereActor(APawn* dropCenterActor, float dropRadius)
{
    AHealthSphere* DropHp;
    
    if(!m_PoolHp.Dequeue(DropHp))
    {
        DropHp = Cast<AHealthSphere>(SpawnInteractActor(UMonsterItemDropTable::ClassDropHealthSphere));
        DropHp->GetOnTaskEnd().AddUObject(this,&URewardManager::EnqueHpSphereActor);
    }

    DropHp->SetActorHiddenInGame(false);

    DropRandomPoint(dropCenterActor,dropRadius,DropHp,600.f);

    return DropHp;
}

void URewardManager::CreateAllItemPool(int itemPoolCount, int goldPoolCount, int hpPoolCount)
{
    m_PoolItem.Empty();
    m_PoolGold.Empty();
    m_PoolHp.Empty();
    
    int i=0;
    while (i++<itemPoolCount)
    {
        ADroppedItem* Created = Cast<ADroppedItem>( SpawnInteractActor(UMonsterItemDropTable::ClassDropItemActor));
        Created->GetOnTaskEnd().AddUObject(this,&URewardManager::EnqueItemActor);
        Created->SetActorHiddenInGame(true);
        m_PoolItem.Enqueue(Created);
    }

    i=0;
    while (i++<goldPoolCount)
    {
        ADroppedGold* Created = Cast<ADroppedGold>( SpawnInteractActor(UMonsterItemDropTable::ClassDropGoldActor));
        Created->GetOnTaskEnd().AddUObject(this,&URewardManager::EnqueGoldActor);
        Created->SetActorHiddenInGame(true);
        m_PoolGold.Enqueue(Created);
    }

    i=0;
    while (i++<hpPoolCount)
    {
        AHealthSphere* Created = Cast<AHealthSphere>( SpawnInteractActor(UMonsterItemDropTable::ClassDropHealthSphere));
        Created->GetOnTaskEnd().AddUObject(this,&URewardManager::EnqueHpSphereActor);
        Created->SetActorHiddenInGame(true);
        m_PoolHp.Enqueue(Created);
    }
}

ACollisionInteract* URewardManager::SpawnInteractActor(TSubclassOf<ACollisionInteract> classWant)
{
    FActorSpawnParameters Param;
    Param.bNoFail=true;
    
    ACollisionInteract* DroppedActor = UDiabloGameInstance::Get->GetWorld()->SpawnActor<ACollisionInteract>(classWant,Param);
    DroppedActor->SetActorLocation(m_HidingPoint);
    
    return DroppedActor;
}

ACollisionInteract* URewardManager::DropRandomPoint(APawn* dropCenterActor,float dropRadius,ACollisionInteract* targetActorToDrop,float height,FOnEnd* endCallback)
{
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(dropCenterActor->GetWorld());

    if(!NavSys)
    {
        PRINTF("RewardManager-Fail-NoNav");

        return nullptr;
    }

    FVector PosWant=dropCenterActor->GetMovementComponent()->GetActorFeetLocation();
    
    FRotator Rot;
    Rot.Yaw=FMath::RandRange(0.f,360.f);
    
    targetActorToDrop->SetActorLocation(PosWant);
    targetActorToDrop->SetActorRotation(Rot);

    FVector NewPos=PosWant;
    
    FNavLocation OutLoc;
    
    if(NavSys->GetRandomPointInNavigableRadius(PosWant,dropRadius,OutLoc))
    {
        NewPos = OutLoc.Location;
    }

    BezierCurveMove(targetActorToDrop,height,NewPos,endCallback);

    return targetActorToDrop;
}

void URewardManager::BezierCurveMove(AActor* target, float height, FVector destination, FOnEnd* endCallback)
{
    FVector StartPoint = target->GetActorLocation();
    UCBezierCurve* Curve = UCActionFactory::MakeCurve();
    FVector ControlPoint = GetQuadControlPoint(StartPoint, destination, height);
    Curve->InitializeQuad(StartPoint, ControlPoint, destination);

    UCFollowBezierCurvePathAction* PathAction = UCActionFactory::MakeFollowBezierPathAction(target, Curve, 3.f);

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
    Result.Z+=height;
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
