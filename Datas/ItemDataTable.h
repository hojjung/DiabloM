#pragma once

#include "DiabloM.h"
#include "AbilitySystem/Ability/DiabloAbility.h"
#include "AbilitySystem/Ability/PlayerAbility/PlayerDiabloAbility.h"
#include "AbilitySystem/GameEffect/ItemOptionGameEffect.h"
#include "Animations/DiaAniminstance.h"
#include "Datas/OptionDataTable.h"
#include "Item/EquipmentActor.h"
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
enum class ESlots: uint32 //use for bitmask
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
        m_fSellValueRate=0.9f;
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
    float m_fSellValueRate;

};

USTRUCT(BlueprintType)
struct FItemTierHandle :public FDataTableRowHandle
{
    GENERATED_USTRUCT_BODY()
public:
    FItemTierHandle();
};



USTRUCT(BlueprintType)
struct FItemType : public FTableRowBase//not only equipment item
{
    GENERATED_BODY()
public:
    FItemType();
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UTexture2D* m_ItemTypeIcon;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMesh* m_DropItemMesh;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName m_TypeID;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText m_ShowingName;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UParticleSystem* m_UseEffect;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    USoundBase* m_UseSound;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool m_bEquipable;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool m_bStackable;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "m_bStackable"))
    int m_nInitStack;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "m_bStackable"))
    int m_nMaxStack;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Bitmask, BitmaskEnum = "ESlots",EditCondition = "m_bEquipable"))
    int32 m_EquipableSlot;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Bitmask, BitmaskEnum = "ESlots",EditCondition = "m_bEquipable"))
    int32 m_EquipInterruptSlot;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FOptionHandle m_MainOption;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "m_bEquipable"))
    TArray<FOptionRollData> m_SubOptions;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float m_fSellValueRate;
    
public:
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
    FItemTypeHandle m_ItemType;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UTexture* m_ItemIcon;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FItemTierHandle m_ItemTierHandle;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FOptionHandle> m_UniqueOptions;//세트 아이템은 옵션 이펙트로 세트 구현할것
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<AEquipmentActor> m_EquipmentBP;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName m_EquipSocketName;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float m_nDefaultSellValue;
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
                  TArray<FOptionSpec>& aryUseEffect, int lv=0);//in cpp

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
    
    IItemHolder* m_Holder=nullptr;

    const FItemData* m_ItemData=nullptr;

    const FItemTier* m_ItemTier=nullptr;

    float m_fBuyCost;
    
public:

    bool IsEmpty() const
    {
        return m_ItemID == NAME_None || !m_ItemData || !m_ItemTier;
    }

    void SetGridNewIndex(int newIndex)
    {
        m_nGridIndex = newIndex;
    }

    bool CheckCanStack() const
    {
        return m_nCurrentStack < m_ItemData->m_ItemType.GetRow<FItemType>("")->m_nMaxStack;
    }
    
    bool GetIsStackable() const
    {
        return m_ItemData->m_ItemType.GetRow<FItemType>("")->m_bStackable;
    }

    int GetMaxStack()
    {
        return m_ItemData->m_ItemType.GetRow<FItemType>("")->m_nMaxStack;
    }

    void ClearData()
    {
        m_TierID= NAME_None;
        m_ItemID = NAME_None;
        m_nCurrentStack = -1;
        m_nGridIndex = -1;
        m_AryOptions.Empty();
        m_nItemLevel=-1;
        m_ItemData = nullptr;
        m_ItemTier=nullptr;
        m_Holder=nullptr;
        m_fBuyCost=0.f;
    }

    float GetFullStackSellValue() const
    {
        return ((m_ItemTier->m_fSellValueRate*m_nItemLevel*m_ItemData->m_ItemType.GetRow<FItemType>("")->m_fSellValueRate)+m_ItemData->m_nDefaultSellValue) * m_nCurrentStack;
    }

    float GetOneStackSellValue() const
    {
        return (m_ItemTier->m_fSellValueRate*m_nItemLevel*m_ItemData->m_ItemType.GetRow<FItemType>("")->m_fSellValueRate)+m_ItemData->m_nDefaultSellValue;
    }

    bool IsHighValue() const
    {
        FName Tier=this->m_ItemTier->m_TierID;
        
        if(Tier=="Set"||Tier=="Immortal"||Tier=="Legend"||Tier=="Epic")
        {
            return true;
        }

        return false;
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
    TArray<FItemTypeHandle> m_AryRightHandNeed;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FItemTypeHandle> m_AryLeftHandNeed;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UDiaAniminstance> m_StanceAnimation;
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
struct FAnimStanceDataHandle :public FDataTableRowHandle
{
    GENERATED_USTRUCT_BODY()
public:
    FAnimStanceDataHandle()
    {
        DataTable=UItemDataTable::GetAnimStanceTable;
    }

};