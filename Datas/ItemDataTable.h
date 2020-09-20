#pragma once

#include "DiabloM.h"
#include "AbilitySystem/AbilityTypes.h"
#include "AbilitySystem/ItemOptionGameEffect.h"
#include "Animations/DiaAniminstance.h"
#include "Datas/OptionDataTable.h"
#include "Item/ItemHolder.h"

#include "ItemDataTable.generated.h"

//아예 다시 만들어야한다는데?
USTRUCT(BlueprintType)
struct FOptionHandle :public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()
	FOptionHandle()
	{
		DataTable=UOptionDataTable::GetOptionTable;
	}
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
        m_ShowingName = FText::FromString("Normal");
        m_TierColor = FColor(242, 242, 242, 255);
        m_nOptionMaxCount = 0;
        m_TierID = "SetSameTableID";
    }

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
    float m_fDefaultDropRate;//there is no maximum
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText m_ShowingName;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FLinearColor m_TierColor;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int m_nOptionMaxCount;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName m_TierID;
};


USTRUCT(BlueprintType)
struct FItemType : public FTableRowBase
{
    GENERATED_BODY()
public:
    FItemType(): m_EquipableSlot(), m_EquipInterruptSlot()
    {
        m_TypeID = "SetSameWithRowID";
        m_ShowingName = FText::FromString("TheShowNameLikeOneHandSword");
        //Blueprint'/Game/Blueprints/Abilities/GE_ItemOptionEffect.GE_ItemOptionEffect_C'
        static ConstructorHelpers::FClassFinder<UItemOptionGameEffect>GameEffectAsset
        (TEXT("Blueprint'/Game/Blueprints/Abilities/GE_ItemOptionEffect.GE_ItemOptionEffect_C'"));

        if(GameEffectAsset.Succeeded())
        {
            m_OptionGameEffect=GameEffectAsset.Class;
        }
    };
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Bitmask, BitmaskEnum = "ESlots"))
    int32  m_EquipableSlot;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Bitmask, BitmaskEnum = "ESlots"))
    int32  m_EquipInterruptSlot;//like says twohand sword,LeftHand is interrupt slot
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UItemOptionGameEffect> m_OptionGameEffect;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FOptionHandle> m_Options;
  
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText m_ShowingName;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName m_TypeID;
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
    int m_nSellValue;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMesh* m_ItemMesh;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UTexture* m_ItemIcon;


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
                  TArray<FOptionSpec>& aryUseEffect, const FItemTier* itemTier = nullptr);//in cpp

public:
    UPROPERTY(EditAnywhere)
    int m_nCurrentStack;
    UPROPERTY(EditAnywhere)
    int m_nGridIndex;
    UPROPERTY(EditAnywhere)
    TArray<FOptionSpec> m_AryOptions;
    UPROPERTY(EditAnywhere)
    FName m_ItemID = NAME_None;
    UPROPERTY(EditAnywhere)
    FName m_TierID;

    int m_nMaxStack;
    
    bool m_bStackable;
    
    IItemHolder* m_Holder;

    const FItemData* m_ItemData;

    const FItemTier* m_ItemTier;
public:

    bool IsEmpty()
    {
        return m_ItemID == NAME_None && !m_ItemData;
    }

    void SetGridNewIndex(int newIndex)
    {
        m_nGridIndex = newIndex;
    }

    bool CheckCanStack() const
    {
        return m_nCurrentStack < m_nMaxStack;
    }
    
    bool GetIsStackable() const
    {
        return m_bStackable;
    }

    void ClearData()
    {
        m_ItemID = NAME_None;
        m_nCurrentStack = -1;
        m_nGridIndex = -1;
        m_nMaxStack = -1;
        m_bStackable = false;
        m_ItemData = nullptr;
        m_AryOptions.Empty();
    }
};


USTRUCT(BlueprintType)
struct FAnimStance: public FTableRowBase
{
    GENERATED_BODY()
public:
    FAnimStance();
    //만일 쌍검 요구 1개랑
    //쌍검 +헬멧 요구가 만나면 어떡할것?

    //무기가 첫번째 비교
    //이후에 중복가능하면
    //개수가 많이 충족된것으로 고름
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float m_fStancePriority;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FItemTypeHandle> m_AryRightHandNeed;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FItemTypeHandle> m_AryLeftHandNeed;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UDiaAniminstance> m_StanceAnimation;
};


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