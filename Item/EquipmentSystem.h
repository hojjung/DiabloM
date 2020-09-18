#pragma once

#include "DiabloM.h"
#include "ItemHolder.h"
#include "Datas/ItemDataTable.h"
#include  "Datas/OptionDataTable.h"

#include "EquipmentSystem.generated.h"


class USaveEquipment;
// UENUM(BlueprintType)
// enum class EAnimStance :uint8 //애니매이션으로 사용될 세트ㅜ
// {
//     None,
//     OneHandSword,
//     TwohandSword,
//     Dagger,
//     Katana,
//     Bow,
//     Staff,
//     Shield,
//     DualSword,
//     Length
// };


struct FEquipSlot
{
public:
    FEquipSlot(): m_Slot(), m_EquippedType(nullptr)
    {
    }

public:
    ESlots m_Slot;
    
    const FItemType* m_EquippedType;

    FItemInstance m_Item;

    FActiveGameplayEffectHandle m_OptionHandle;
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

    const FItemType* GetEquippedItem(ESlots slot);

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
