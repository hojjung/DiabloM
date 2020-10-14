#include "ItemManager.h"
#include "Managers/DiabloGameInstance.h"

UItemManager::~UItemManager()
{
}


void UItemManager::Init(UDiabloGameInstance* gameInstance)
{
    m_GameInstance = gameInstance;
    m_nCurrentIndex = 0;
    m_fTierMaxRate = 0;

    UItemDataTable::GetTierTable->GetAllRows("Error", m_AryItemTier);

    for (auto* ItemTier : m_AryItemTier)
    {
        m_fTierMaxRate += ItemTier->m_fDefaultDropRate;
    }

    PRINTF("UItemManager Init");
}

FItemInstance UItemManager::CreateItemInstance(FName itemID, int level)
{
    const FItemData* ItemData = UItemDataTable::GetItemDataPtr(itemID);

    const FItemTier& TierRolled = GetDefaultTierRoll();

    int TierMaxOptionCount = TierRolled.m_AryOptionCount.GetRandom();

    float TierBonusValue=TierRolled.m_fBonusValue;
    
    TArray<FOptionSpec> RandomOptionForItem;

    CreateRandomOption(*ItemData, RandomOptionForItem, TierMaxOptionCount,TierBonusValue ,level);

    return FItemInstance(ItemData, TierRolled.m_TierID, m_nCurrentIndex, this, RandomOptionForItem,level, &TierRolled);
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


bool UItemManager::CreateRandomOption(const FItemData& itemData, TArray<FOptionSpec>& outOption, int TierMaxOption,float bonus,int level)
{
    if (itemData.m_bStackable || !itemData.m_bEquipable)
    {
        PRINTF("ItemOption - the item is not equipment");
        return false;
    }

    FItemType* ItemTT =itemData.m_ItemType.GetRow<FItemType>("");

    FOptionSpec MainOp=ItemTT->m_MainOption.GetRow<FOption>("")->MakeOptionInst();
    
    MainOp.m_fValue*=bonus;
    
    MainOp.m_fValue*=ItemTT->m_AryMainOptionBonusRand.GetRandom();
    
    outOption.Add(MainOp);

    TArray<FOptionHandle> AryAvailableOptions = ItemTT->GetAvailableOptions(level);

    int NumMaxOption = AryAvailableOptions.Num();

    if (NumMaxOption <= 0 || TierMaxOption <= 0)
    {
        return false;
    }

    int OptionRandomCount = FMath::Rand() % TierMaxOption; //생성할 옵션의 개수는 등급과 옵션의 개수에 따라 상이하다.

    OptionRandomCount = FMath::Min<int>(OptionRandomCount, NumMaxOption);

    CreateIntAryForShuffle(OptionRandomCount, AryAvailableOptions);
    //옵션 랜덤이 프라이오리티 및 중복 안되야함
    for (FOptionHandle OO : AryAvailableOptions)//옵션 랜덤카운트 만큼만 넣어야함? 이대로면 무조건 넣는거아님?
    {
        outOption.Add(OO.GetRow<FOption>("")->MakeOptionInst());
    }

    return true;
}

void UItemManager::CreateIntAryForShuffle(int maxAryLen, TArray<FOptionHandle>& outOptionAry)
{
    for (int k = 0; k < 3; k++)
    {
        for (int i = 0; i < maxAryLen; i++)
        {
            FOptionHandle OO = outOptionAry[i];
            int index = FMath::Rand() % maxAryLen;
            outOptionAry[i] = outOptionAry[index];
            outOptionAry[index] = OO;
        }
    }
}

FOptionSpec UItemManager::CreateRandomOptionValue(int indexRandomd, const FItemData& itemData)
{
    return itemData.m_ItemType.GetRow<FItemType>("")->m_SubOptions[indexRandomd].GetRow<FOption>("")->MakeOptionInst();
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

const FItemTier& UItemManager::GetDefaultTierRoll() const
{
    float RandomValue = FMath::RandRange(0.f, m_fTierMaxRate);
    //76/100
    float DropRateCount = 0.f;

    for (auto* TierData : m_AryItemTier)
    {
        DropRateCount += TierData->m_fDefaultDropRate;

        if (DropRateCount >= RandomValue)
        {
            PRINTF("Rand:%f,DropRate:%f,Tier:%s", RandomValue, DropRateCount, *TierData->m_ShowingName.ToString());

            return *TierData;
        }
    }

    PRINTF("Error? - TierDrop Roll Fucked");

    return *m_AryItemTier[0];
}
