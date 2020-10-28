#include "MonsterItemDropTable.h"

UDataTable* UMonsterItemDropTable::GetMonsterRewardDropTable = nullptr;
TSubclassOf<AHealthSphere> UMonsterItemDropTable::ClassDropHealthSphere = nullptr;
TSubclassOf<ADroppedItem> UMonsterItemDropTable::ClassDropItemActor = nullptr;
TSubclassOf<ADroppedGold> UMonsterItemDropTable::ClassDropGoldActor = nullptr;

FMonsterItemDropRow::FMonsterItemDropRow()
{
    m_AryHpDropRand.Reset();
    m_AryHpDropRand.Add(0);
    m_AryHpDropRand.Add(0);
    m_AryHpDropRand.Add(0);
    m_AryHpDropRand.Add(1);
    m_AryHpDropRand.Add(1);
    m_AryHpDropRand.Add(2);

    m_AryItemDropRand.Reset();
    m_AryItemDropRand.Add(0);
    m_AryItemDropRand.Add(1);
    m_AryItemDropRand.Add(1);
    m_AryItemDropRand.Add(2);
    m_AryItemDropRand.Add(2);
    m_AryItemDropRand.Add(3);
    //ㄴ그냥 이걸 랜덤으로 돌려서 액터 소환
}

UMonsterItemDropTable::UMonsterItemDropTable()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundDropTable(
     TEXT("DataTable'/Game/DataTables/Dungeon/MonsterRewardTable.MonsterRewardTable'"));
    UMonsterItemDropTable::GetMonsterRewardDropTable=FoundDropTable.Object;
    
     static ConstructorHelpers::FClassFinder<ADroppedGold>FoundDropGold(
        TEXT("Blueprint'/Game/Blueprints/DropActor/BP_DroppedGold.BP_DroppedGold_C'"));
    ClassDropGoldActor=FoundDropGold.Class;

    static ConstructorHelpers::FClassFinder<ADroppedItem>FoundDropItem(
        TEXT("Blueprint'/Game/Blueprints/DropActor/BP_DroppedItem.BP_DroppedItem_C'"));
    ClassDropItemActor=FoundDropItem.Class;
    
    static ConstructorHelpers::FClassFinder<AHealthSphere>FoundDropHpSphere(
        TEXT("Blueprint'/Game/Blueprints/DropActor/BP_HealthSphere.BP_HealthSphere_C'"));
    ClassDropHealthSphere=FoundDropHpSphere.Class;
}

const FMonsterItemDropRow& UMonsterItemDropTable::GetDropRewardTableRow(FName id)
{
    return *UMonsterItemDropTable::GetMonsterRewardDropTable->FindRow<FMonsterItemDropRow>(id,"");   
}

const FMonsterItemDropRow* UMonsterItemDropTable::GetDropRewardTableRowPtr(FName id)
{
    return UMonsterItemDropTable::GetMonsterRewardDropTable->FindRow<FMonsterItemDropRow>(id,"");
}

FMonsterItemDropRowHandle::FMonsterItemDropRowHandle()
{
    DataTable=UMonsterItemDropTable::GetMonsterRewardDropTable;
}
