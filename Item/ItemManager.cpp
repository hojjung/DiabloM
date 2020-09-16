#include "ItemManager.h"
#include "Managers/DiabloGameInstance.h"

UItemManager::~UItemManager()
{
}


void UItemManager::Init(UDiabloGameInstance* gameInstance)
{
    m_GameInstance = gameInstance;
    m_nCurrentIndex = 0;

    PRINTF("UItemManager Init");
}

FItemInstance UItemManager::CreateItemInstance(FName itemID, int level)
{
    const FItemData* ItemData = m_GameInstance->GetItemDataPtr(itemID);

    FItemInstance ItemCreated;

    TArray<FOptionSpec> RandomOptionForItem;

    if (CreateRandomOption(*ItemData, RandomOptionForItem))
    {
        ItemCreated = FItemInstance(ItemData, m_nCurrentIndex, this, &RandomOptionForItem);
    }
    else
    {
        ItemCreated = FItemInstance(ItemData, m_nCurrentIndex, this);
    }


    return ItemCreated;
}

ADroppedItem* UItemManager::CreateItemActor(FItemInstance& itemWantAdd, FVector posWant)
{
    ADroppedItem* DroppedActor = Cast<ADroppedItem>(
        m_GameInstance->GetWorld()->SpawnActor(m_GameInstance->m_DropItemClass, &posWant));

    itemWantAdd.m_Holder = this;
    itemWantAdd.m_nGridIndex = -2;

    DroppedActor->SetItemInstance(itemWantAdd);


    return DroppedActor;
}


bool UItemManager::CreateRandomOption(const FItemData& itemData, TArray<FOptionSpec>& outOption)
{
    //등급에 따라 옵션의 개수?
    //레벨에 따라 옵션의 종류 및 강함?
    //생각할게 많지만 지금은 옵션의 개수만 랜덤으로 돌린다.
    //등급 자체는 정해저있지 않나?
    //등급이 정해저있는데 옵션의 개수가 무슨 의미인가

    if (itemData.m_bStackable)
    {
        return false;
    }

    int NumMaxOption = itemData.m_Options.Num();

    int TierMaxOption = itemData.GetItemTier().m_nOptionMaxCount;

    if (NumMaxOption <= 0 || TierMaxOption <= 0)
    {
        return false;
    }

    int OptionRandomCount = FMath::Rand() % TierMaxOption; //생성할 옵션의 개수는 등급과 옵션의 개수에 따라 상이하다.

    OptionRandomCount = FMath::Min<int>(OptionRandomCount, NumMaxOption);


    TArray<int> optionRandom;

    CreateIntAryForShuffle(OptionRandomCount, optionRandom);


    int i = 0;
    while (i < OptionRandomCount)
    {
        outOption.Add(CreateRandomOptionValue(optionRandom[i], itemData));

        i++;
    }
    //아웃옵션한텐 인덱스를 줘야한다.

    return true;
}

void UItemManager::CreateIntAryForShuffle(int maxAryLen, TArray<int>& outIndexAry)
{
    int NumMaxOption = maxAryLen;

    outIndexAry.Reset();

    int index = 0;
    while (index < maxAryLen)
    {
        outIndexAry.Add(index++);
    }


    for (int b = 0; b < NumMaxOption; b++)
        for (int a = 0; a < NumMaxOption; a++)
        {
            int RandIndex = FMath::Rand() % NumMaxOption;

            auto First = outIndexAry[a];
            auto Second = outIndexAry[RandIndex];

            outIndexAry[a] = Second;
            outIndexAry[RandIndex] = First;
        }
}

FOptionSpec UItemManager::CreateRandomOptionValue(int indexRandomd, const FItemData& itemData)
{
    return itemData.GetOption(indexRandomd).MakeOptionInst(indexRandomd);
}

bool UItemManager::AddItem(int droppedIndex, FItemInstance& itemWantAdd)
{
    return true;
}

void UItemManager::RemoveItem(FItemInstance& itemWantErase)
{
}

void UItemManager::RemoveItemByIndex(int index)
{
    //20200825
    //버전문제 아님
    //위젯문제도 아니라고봄?

    //일단 이줄을 지우면 인터페이스고 뭐고 작동함
    //일단 위젯대신 마우스클릭은 문제없음

    //블루프린트일때 메모리 등의 문제 있어보임
    //m_AryItemInWorld[index]->ClearData();//what if 0?
}

bool UItemManager::CheckSlotValid(int droppedIndex, FItemInstance& itemWantAdd)
{
    return true;
}

void UItemManager::SetItem(int droppedIndex, FItemInstance& itemWantAdd)
{
}

bool UItemManager::SwapMove(FItemInstance& Drop, FItemInstance& Drag)
{
    return true;
}
