#pragma once

#include "DiabloM.h"
#include "ItemHolder.h"
#include "Datas/ItemDataTable.h"

#include "EquipmentSystem.generated.h"

class UDiabloAbilitySystemComp;
class USaveEquipment;



UENUM(BlueprintType)
enum class ESlotsEquipAry: uint8
{
    Head,
    Neck,
    Torso,
    Waist,
    Leg,
    Hand,
    Shoulder,
    WeaponRight,
    WeaponLeft,
    FingerRight,
    FingerLeft,
    Length
};

struct FEquipSlot
{
public:
    FEquipSlot(): m_bIsOccupied(false),m_Slot(), m_EquippedType(nullptr)
    {
    }

public:
    bool m_bIsOccupied;
    
    ESlotsEquipAry m_Slot;
    
    const FItemType* m_EquippedType;

    FItemInstance m_Item;

    FActiveGameplayEffectHandle m_OptionHandle;

    TArray<FItemType*> m_AryCantEquipable;

    void ClearSlot()
    {
        m_Item.ClearData();
        m_EquippedType=nullptr;
    }
};

DECLARE_MULTICAST_DELEGATE(FOnItemEuipChanged);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEquipSlotChanged,const FItemInstance&  ,const FEquipSlot&);
UCLASS()
class DIABLOM_API UEquipmentSystem : public UObject, public IItemHolder
{
    GENERATED_BODY()

public:
    ~UEquipmentSystem();

protected:
    FEquipSlot m_Head;
    FEquipSlot m_Neck;
    FEquipSlot m_Torso;
    FEquipSlot m_Waist;
    FEquipSlot m_Leg;
    FEquipSlot m_Hand;
    FEquipSlot m_Shoulder;
    FEquipSlot m_WeaponLeft;
    FEquipSlot m_WeaponRight;
    FEquipSlot m_FingerLeft;
    FEquipSlot m_FingerRight;

    TArray<FEquipSlot*> m_ArySlots;

    TMap<FItemInstance*,FActiveGameplayEffectHandle> m_EquipmentEffectContainer;

    UPROPERTY()
    UDiabloAbilitySystemComp* m_TargetAbilitySys;

    TArray<FAnimStance*> m_AryAnimStances;
    
    const FAnimStance* m_CurrentStance;

    FOnItemSlotChanged m_ItemChanged;

    FOnEquipSlotChanged m_EquipSlotChanged;


public:
    FOnItemEuipChanged m_OnOptionChanged;
    
    void Init(UDiabloAbilitySystemComp* abilitySysCompo);

    virtual bool AddItem(int droppedIndex, FItemInstance& itemWantAdd) override;

    virtual void RemoveItem(FItemInstance& itemWantErase) override;

    virtual void RemoveItemByIndex(int index) override;
    
    bool CheckSlotOccupied(int index);

    virtual bool SwapMove(FItemInstance& Drop, FItemInstance& Drag) override;

    virtual bool CheckSlotValid(int droppedIndex, FItemInstance& itemWantAdd) override;

    virtual void SetItem(int droppedIndex, FItemInstance& itemWantAdd) override;

    FItemInstance& GetItem(int index);

    FItemInstance& GetItem(ESlotsEquipAry index);

    void PrintEquipStats();

    const FItemType* GetEquippedItemType(ESlotsEquipAry slot);

    const FItemType* GetEquippedItemType(int slotIndex);

    friend USaveEquipment;

    void SetItemAry(TArray<FItemInstance>& equipSlot);

    TArray<FEquipSlot*>& GetArySlotPtr();
    
    virtual FOnItemSlotChanged& GetItemChangeCallback() override
    {
        return m_ItemChanged;
    }

    FOnEquipSlotChanged& GetEquipChanged()
    {
        return m_EquipSlotChanged;
    }

    const FAnimStance* GetCurrentStance() const
    {
        return m_CurrentStance;
    }


protected:
    void OnItemSlotChanged(int index);
    
    void CalculateAnimStance();

    
    void SetUnequipItemToSlots(TArray<TArray<FItemTypeHandle>> aryAryItemType);
};
//TArray<TArray<FItemTypeHandle>>