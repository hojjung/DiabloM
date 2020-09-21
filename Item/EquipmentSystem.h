#pragma once

#include "DiabloM.h"
#include "ItemHolder.h"
#include "Datas/ItemDataTable.h"

#include "EquipmentSystem.generated.h"

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
    FEquipSlot(): m_Slot(), m_EquippedType(nullptr),m_bIsOccupied(false)
    {
    }

public:
    bool m_bIsOccupied;
    
    ESlotsEquipAry m_Slot;
    
    const FItemType* m_EquippedType;

    FItemInstance m_Item;

    FActiveGameplayEffectHandle m_OptionHandle;

    void ClearSlot()
    {
        m_Item.ClearData();
        m_EquippedType=nullptr;
    }
};


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

    UDiabloAbilitySystemComp* m_TargetAbilitySys;
    
    const FAnimStance* m_CurrentStance;

    FOnItemSlotChanged m_ItemChanged;

protected:
    void OnItemSlotChanged(int index);

public:
    void Init(UDiabloAbilitySystemComp* abilitySysCompo);

    virtual bool AddItem(int droppedIndex, FItemInstance& itemWantAdd) override;

    virtual void RemoveItem(FItemInstance& itemWantErase) override;

    virtual void RemoveItemByIndex(int index) override;
    
    bool CheckSlotOccupied(int index);

    virtual bool SwapMove(FItemInstance& Drop, FItemInstance& Drag) override;

    virtual bool CheckSlotValid(int droppedIndex, FItemInstance& itemWantAdd) override;

    virtual void SetItem(int droppedIndex, FItemInstance& itemWantAdd) override;

    FItemInstance& GetItem(int index);

    void PrintEquipStats();

    const FItemType* GetEquippedItem(ESlotsEquipAry slot);

    const FItemType* GetEquippedItem(int slotIndex);

    friend USaveEquipment;

    void SetItemAry(TArray<FItemInstance>& equipSlot);

    virtual FOnItemSlotChanged& GetItemChangeCallback() override
    {
        return m_ItemChanged;
    }

    const FAnimStance* GetCurrentStance() const
    {
        return m_CurrentStance;
    }

    TArray<FEquipSlot*>& GetArySlotPtr();
};
