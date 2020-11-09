// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "ItemDataTable.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "ShopItemTable.generated.h"


USTRUCT(BlueprintType) //���̵�,Ƽ��
struct FShopItemSell 
{
    GENERATED_BODY()

public:
    FShopItemSell();

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FItemDataHandle m_ItemData;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool m_bAutoLevel;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int m_nLevel;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool m_bAutoTier;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FItemTierHandle m_ItemTier;
};

USTRUCT(BlueprintType) //���̵�,Ƽ��
struct FShopItemSellData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FShopItemSell> m_AryShopItemSell;
};


//아예 다시 만들어야한다는데?
USTRUCT(BlueprintType)
struct FShopItemSellHandle :public FDataTableRowHandle
{
    GENERATED_USTRUCT_BODY()
public:
    FShopItemSellHandle();
};

UCLASS()
class DIABLOM_API UShopItemTable : public UObject
{
    GENERATED_BODY()

public:
    UShopItemTable();

public:
    static UDataTable* GetShopItemData;

public:
    static const FShopItemSellData& GetShopSellItemData(FName id);

    static const FShopItemSellData* GetShopSellItemDataPtr(FName id);
};
