#include "ItemManager.h"

#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"


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

FItemInstance UItemManager::CreateItemInstance(FName id, float magicItemBonus, float rareItemBonus, float epicItemBonus,
                                               int itemLevel)
{
    const FItemData* ItemData = UItemDataTable::GetItemDataPtr(id);

    const FItemTier& TierRolled = GetDefaultTierRoll();

    int TierMaxOptionCount = TierRolled.m_AryOptionCount.GetRandom();

    float TierBonusValue = TierRolled.m_fBonusValue;

    TArray<FOptionSpec> RandomOptionForItem;

    CreateRandomOption(*ItemData, RandomOptionForItem, TierMaxOptionCount, TierBonusValue, itemLevel);

    return FItemInstance(ItemData, TierRolled.m_TierID, m_nCurrentIndex, this, RandomOptionForItem, itemLevel,
                         &TierRolled);
}

FItemInstance UItemManager::CreateUniqueItem(const FUniqueEquipData* unique_item, int item_level)
{
    const FItemTier& Tier = *unique_item->m_UniqueItemTierHandle.GetRow<FItemTier>("");

    int TierMaxOptionCount = Tier.m_AryOptionCount.GetRandom();

    float TierBonusValue = Tier.m_fBonusValue;

    TArray<FOptionSpec> RandomOptionForItem;

    CreateRandomOptionWithUnique(*unique_item, RandomOptionForItem, TierMaxOptionCount, TierBonusValue, item_level);

    return FItemInstance(unique_item, Tier.m_TierID, m_nCurrentIndex, this, RandomOptionForItem, item_level, &Tier);
}


bool UItemManager::CreateRandomOption(const FItemData& itemData, TArray<FOptionSpec>& outOption, int TierMaxOption,
                                      float bonus, int level)
{
    if (itemData.m_bStackable || !itemData.m_bEquipable)
    {
        PRINTF("ItemOption - the item is not equipment");
        return false;
    }


    FItemType* ItemTT = itemData.m_ItemType.GetRow<FItemType>("");

    FOptionSpec MainOp = ItemTT->m_MainOption.GetRow<FOption>("")->MakeOptionInst(level);

    MainOp.m_fValue *= ItemTT->m_MainOptionBonusRate;

    MainOp.m_fValue *= bonus;

    outOption.Add(MainOp);
    //
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
    for (FOptionHandle OO : AryAvailableOptions) //옵션 랜덤카운트 만큼만 넣어야함? 이대로면 무조건 넣는거아님?
    {
        outOption.Add(OO.GetRow<FOption>("")->MakeOptionInst(level));
    }

    return true;
}

bool UItemManager::CreateRandomOptionWithUnique(const FUniqueEquipData& itemData, TArray<FOptionSpec>& outOption,
                                                int TierMaxOption, float bonus, int level)
{
    if (itemData.m_bStackable || !itemData.m_bEquipable)
    {
        PRINTF("ItemOption - the item is not equipment");
        return false;
    }

    FItemType* ItemTT = itemData.m_ItemType.GetRow<FItemType>("");

    FOptionSpec MainOp = ItemTT->m_MainOption.GetRow<FOption>("")->MakeOptionInst(level);

    MainOp.m_fValue *= ItemTT->m_MainOptionBonusRate;

    MainOp.m_fValue *= bonus;

    outOption.Add(MainOp);
    //
    for (const FOptionHandle& UniqueOps : itemData.m_UniqueOptions)
    {
        FOptionSpec UniqueOptionSpec = UniqueOps.GetRow<FOption>("")->MakeOptionInst(level);

        outOption.Add(UniqueOptionSpec); //유니크 옵션 표시 어떻게?
    }
    //
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
    for (FOptionHandle OO : AryAvailableOptions) //옵션 랜덤카운트 만큼만 넣어야함? 이대로면 무조건 넣는거아님?
    {
        outOption.Add(OO.GetRow<FOption>("")->MakeOptionInst(level));
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
            return *TierData;
        }
    }

    PRINTF("Error? - TierDrop Roll Fucked");

    return *m_AryItemTier[0];
}

FItemInstance UItemManager::CreateItemManual(const FShopItemSell& item_sell) //cant make unique
{
    const FItemData* ItemDataFromTable = item_sell.m_ItemData.GetRow<FItemData>("");

    bool IsEquipItem = ItemDataFromTable->m_bEquipable;

    const FItemTier* Tier = nullptr;

    FName TierId = NAME_None;

    int ItemLevel = ADiabloPlayerController::Get->GetPlayerPawn()->GetCharacterLevel();

    TArray<FOptionSpec> AryOp;

    if (IsEquipItem)
    {
        bool AutoLevel = item_sell.m_bAutoLevel;

        if (!AutoLevel)
        {
            ItemLevel = item_sell.m_nLevel;
        }

        bool AutoTier = item_sell.m_bAutoTier;

        if (!AutoTier)
        {
            Tier = item_sell.m_ItemTier.GetRow<FItemTier>("");
        }
        else
        {
            Tier = &GetDefaultTierRoll();
        }

        TierId = Tier->m_TierID;

        int TierMaxOptionCount = (*Tier).m_AryOptionCount.GetRandom();

        float TierBonusValue = (*Tier).m_fBonusValue;

        CreateRandomOption(*ItemDataFromTable, AryOp, TierMaxOptionCount, TierBonusValue, ItemLevel);
    }

    FItemInstance Inst = FItemInstance(ItemDataFromTable, TierId, m_nCurrentIndex, this, AryOp, ItemLevel);

    return Inst;
}
