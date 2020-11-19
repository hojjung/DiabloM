#pragma once

#include "DiabloM.h"
#include "AbilitySystem/Ability/DiabloAbility.h"
#include "AbilitySystem/Ability/PlayerAbility/PlayerDiabloAbility.h"
#include "AbilitySystem/GameEffect/ItemOptionGameEffect.h"
#include "Animations/DiaAniminstance.h"
#include "Datas/OptionDataTable.h"
#include "Item/ItemHolder.h"
//#include "Item/Weapon.h"
#include "ItemDataTable.generated.h"

class AWeapon;

//아예 다시 만들어야한다는데?
USTRUCT(BlueprintType)
struct FOptionHandle :public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()
public:
	FOptionHandle()
	{
		DataTable=UOptionDataTable::GetOptionTable;
	}
};

USTRUCT(BlueprintType)
struct FOptionRollData 
{
    GENERATED_USTRUCT_BODY()
public:
    FOptionRollData()
    {
        m_nMinLevel=0;
        m_fRollPriorityRate=0.f;
    }

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0"))
    FOptionHandle m_OptionHandle;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0"))
    int m_nMinLevel;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0"))
    float m_fRollPriorityRate;
};

USTRUCT(BlueprintType)
struct FItemTypeHandle :public FDataTableRowHandle
{
    GENERATED_USTRUCT_BODY()
    FItemTypeHandle();
};

//
UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ESlots: uint32
{
    Head=1,
    Neck=2,
    Torso=4,
    Waist=8,
    Leg=16,
    Hand=32,
    Shoulder=64,
    WeaponRight=128,
    WeaponLeft=256,
    FingerRight=512,
    FingerLeft=1024,
    Length
};
ENUM_CLASS_FLAGS(ESlots);


USTRUCT(BlueprintType) //���̵�,Ƽ��
struct FItemTier : public FTableRowBase
{
    GENERATED_BODY()

public:
    FItemTier(): m_fDefaultDropRate(0)
    {
        m_fBonusPowerRate=1.f;
        m_ShowingName = FText::FromString("Normal");
        m_TierColor = FColor(242, 242, 242, 255);
        m_AryOptionCount.Reset();
        m_AryOptionCount.Add(0);
        m_AryOptionCount.Add(1);
        m_AryOptionCount.Add(2);
        m_TierID = "SetSameTableID";
        m_fGoldCostRate=0.9f;
    }

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName m_TierID;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText m_ShowingName;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
    float m_fDefaultDropRate;//there is no maximum
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FLinearColor m_TierColor;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<int> m_AryOptionCount;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", UIMin = "1"))
    float m_fBonusPowerRate;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", UIMin = "1"))
    float m_fGoldCostRate;
};

USTRUCT(BlueprintType)
struct FItemTierHandle :public FDataTableRowHandle
{
    GENERATED_USTRUCT_BODY()
public:
    FItemTierHandle();
};



USTRUCT(BlueprintType)
struct FItemType : public FTableRowBase
{
    GENERATED_BODY()
public:
    FItemType();
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName m_TypeID;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText m_ShowingName;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Bitmask, BitmaskEnum = "ESlots"))
    int32 m_EquipableSlot;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Bitmask, BitmaskEnum = "ESlots"))
    int32 m_EquipInterruptSlot;//like says twohand sword,LeftHand is interrupt slot
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<AWeapon> m_EquipmentBP;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float m_MainOptionBonusRate;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float m_SellCostRate;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FOptionHandle m_MainOption;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FOptionRollData> m_SubOptions;
  
    FORCEINLINE TArray<FOptionHandle> GetAvailableOptions(int level) const
    {
        TArray<FOptionHandle> AryOptions;

        for(auto& OO : m_SubOptions)
        {
            if(OO.m_nMinLevel<level)
            {
                AryOptions.Add(OO.m_OptionHandle);
            }
        }

        return AryOptions;
    }
    //equipable class
};


USTRUCT(BlueprintType) //���̵�,Ƽ��
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

public:
    FItemData();

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName m_ItemID;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText m_ShowingName;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText m_FlavorText;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FDataTableRowHandle m_ItemType;
    //무기 같은건 소켓과 액터
    //무기는 스켈레탈 스태틱 둘다 있지 않나? 미리 박아놓으면 소켓이고 뭐고 할게 없다
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    USkeletalMesh* m_SkEquipment;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMesh* m_StEquipment;
    //하지만 방어구 또한 어디로 들어갈지 알아야한다.
    //방어구는 아님

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool m_bEquipable;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool m_bStackable;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int m_nInitStack;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int m_nMaxStack;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float m_nDefaultSellValue;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMesh* m_ItemMesh;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UTexture* m_ItemIcon;


};

USTRUCT(BlueprintType) //���̵�,Ƽ��
struct FUniqueEquipData : public FItemData
{
    GENERATED_BODY()

public:
    FUniqueEquipData();

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FItemTierHandle m_UniqueItemTierHandle;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FOptionHandle> m_UniqueOptions;//세트 아이템은 옵션 이펙트로 세트 구현할것
    
};

USTRUCT(BlueprintType)
struct FItemInstance
{
    GENERATED_BODY()

public:
    FItemInstance()
    {
        ClearData();
    }

    FItemInstance(const FItemData* itemData, FName tierID, int gridIndex, IItemHolder* holder,
                  TArray<FOptionSpec>& aryUseEffect, int lv=0,const FItemTier* itemTier = nullptr);//in cpp

public:
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    int m_nCurrentStack;
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    int m_nGridIndex;
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    TArray<FOptionSpec> m_AryOptions;
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    FName m_ItemID = NAME_None;
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    FName m_TierID;
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    int m_nItemLevel;
    
    IItemHolder* m_Holder;

    const FItemData* m_ItemData;

    const FItemTier* m_ItemTier;

    float m_fBuyCost;
    
public:

    bool IsEmpty() const
    {
        return m_ItemID == NAME_None && !m_ItemData;
    }

    void SetGridNewIndex(int newIndex)
    {
        m_nGridIndex = newIndex;
    }

    bool CheckCanStack() const
    {
        return m_nCurrentStack < m_ItemData->m_nMaxStack;
    }
    
    bool GetIsStackable() const
    {
        return m_ItemData->m_bStackable;
    }

    int GetMaxStack()
    {
        return m_ItemData->m_nMaxStack;
    }

    void ClearData()
    {
        m_TierID= NAME_None;
        m_ItemID = NAME_None;
        m_nCurrentStack = -1;
        m_nGridIndex = -1;
        m_ItemData = nullptr;
        m_AryOptions.Empty();
        m_nItemLevel=-1;
    }

    float GetFullStackSellValue() const
    {
        return ((m_ItemTier->m_fGoldCostRate*m_nItemLevel*m_ItemData->m_ItemType.GetRow<FItemType>("")->m_MainOptionBonusRate)+m_ItemData->m_nDefaultSellValue) * m_nCurrentStack;
    }

    float GetOneStackSellValue() const
    {
        return (m_ItemTier->m_fGoldCostRate*m_nItemLevel*m_ItemData->m_ItemType.GetRow<FItemType>("")->m_MainOptionBonusRate)+m_ItemData->m_nDefaultSellValue;
    }
};


USTRUCT(BlueprintType)
struct FAnimStance: public FTableRowBase
{
    GENERATED_BODY()
public:
    FAnimStance();
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText m_ShowingText;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float m_fStancePriority;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FItemTypeHandle> m_AryRightHandNeed;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FItemTypeHandle> m_AryLeftHandNeed;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UDiaAniminstance> m_StanceAnimation;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UPlayerDiabloAbility> m_BaseAttackAbility;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float m_fViewAngle;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float m_fViewRadius;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float m_fFocusRange;
};

//float viewAngle, float viewRadius, float focusRange
UCLASS()
class DIABLOM_API UItemDataTable : public UObject
{
    GENERATED_BODY()

    public:
    UItemDataTable();

 public:
    static  UDataTable* GetTierTable;

    static  UDataTable* GetItemTable;

    static  UDataTable* GetItemTypeTable;

    static  UDataTable* GetUniqueItemTypeTable;

    static  UDataTable* GetAnimStanceTable;

 public:
    
    static const FItemTier& GetItemTier(FName id);

    static const FItemTier* GetItemTierPtr(FName id);

    static const FItemData& GetItemData(FName id);

    static const FItemData* GetItemDataPtr(FName id);

    static const FItemType& GetItemType(FName id);

    static const FItemType* GetItemTypePtr(FName id);

    static const FAnimStance& GetAnimStance(FName id);

    static const FAnimStance* GetAnimStancePtr(FName id);

    static const FUniqueEquipData& GetUniqueItem(FName id);

    static const FUniqueEquipData* GetUniqueItemPtr(FName id);
    
};


USTRUCT(BlueprintType)
struct FItemDataHandle :public FDataTableRowHandle
{
    GENERATED_USTRUCT_BODY()
public:
    FItemDataHandle()
    {
        DataTable=UItemDataTable::GetItemTable;
    }

};

USTRUCT(BlueprintType)
struct FUniqueItemDataHandle :public FDataTableRowHandle
{
    GENERATED_USTRUCT_BODY()
public:
    FUniqueItemDataHandle()
    {
        DataTable=UItemDataTable::GetUniqueItemTypeTable;
    }
};

