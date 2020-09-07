// ReSharper disable All
#pragma once

#include "DiabloM.h"
#include "Datas/ItemDataTable.h"


class USaveEquipment;
UENUM(BlueprintType)
enum class EAnimStance :uint8//애니매이션으로 사용될 세트ㅜ
{
	None,
	OneHandSword,
	TwohandSword,
	Dagger,
	Katana,
	Bow,
	Staff,
	Shield,
	DualSword,
	Length
};

UENUM(BlueprintType)
enum class ESlots:uint8
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
	FEquipSlot()
	{
		m_AbleEquipSlot.Init(false, (int)EItemType::Length);
		m_bIsOccupied = false;
		m_EquippedType = EItemType::None;
	}

public:
	EItemType m_EquippedType;

	TArray<bool> m_AbleEquipSlot;

	bool m_bIsOccupied;

	FItemInstance m_Item;

	void SetOccupie(bool v)
	{
		m_bIsOccupied=v;
	}

	void SetEquipableType(EItemType equipableType,bool isAble)
	{
		m_AbleEquipSlot[(int)equipableType] = isAble;
	}

};

class DIABLOM_API EquipmentSystem : public IItemHolder
{

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FStance, EAnimStance);
public:
	EquipmentSystem() {};
	~EquipmentSystem() {};

protected:
	FEquipSlot	m_Head;
	FEquipSlot	m_Neck;
	FEquipSlot	m_Torso;
	FEquipSlot	m_Waist;
	FEquipSlot	m_Leg;
	FEquipSlot	m_Hand;
	FEquipSlot	m_Shoulder;
	FEquipSlot	m_WeaponLeft;
	FEquipSlot	m_WeaponRight;
	FEquipSlot	m_FingerLeft;
	FEquipSlot	m_FingerRight;

protected:
	TArray<FEquipSlot*> m_ArySlots;

public:
	FORCEINLINE TArray<FEquipSlot*>& GetArySlotPtr()
	{
		return m_ArySlots;
	}

protected:
	typedef void (EquipmentSystem::*FPtrForStance)(void);
	FPtrForStance m_StanceFPtr[(int)EItemType::Shield + 1][(int)EItemType::Shield + 1];

	EAnimStance m_CurrentStance;

	FOnItemSlotChanged m_ItemChanged;

	FStance m_OnStanceChanged;

public:
	virtual FOnItemSlotChanged& GetItemChangeCallback()  override
	{
		return m_ItemChanged;
	}

	FStance& GetStanceChangeCallaback()
	{
		return m_OnStanceChanged;
	}

public:
	void Init();

	virtual bool AddItem(int droppedIndex, FItemInstance& itemWantAdd) override;

	virtual void RemoveItem(FItemInstance& itemWantErase) override;

	virtual void RemoveItemByIndex(int index) override;

protected:
	void OnItemSlotChanged(int index);

	void CalculateStance();

	void SetStanceAllNull();

	void SetStanceNull();

	void SetStanceOneHand();

	void SetStanceDual();

	void SetStanceShield();

	void SetStanceDagger();

	void SetStanceKatana();

	void SetStanceTwoHand();



public:
	bool CheckSlotOccupied(int index);

	virtual bool SwapMove(FItemInstance &Drop, FItemInstance &Drag) override;

	virtual bool CheckSlotValid(int droppedIndex, FItemInstance& itemWantAdd) override;

	virtual void SetItem(int droppedIndex, FItemInstance& itemWantAdd) override;

	EAnimStance GetCurrentStance() const
	{
		return m_CurrentStance;
	}

	FItemInstance& GetItem(int index);

	void PrintEquipStats();

	EItemType GetEquippedItem(ESlots slot);

	EItemType GetEquippedItem(int slotIndex);

	friend USaveEquipment;

	void SetItemAry(const TArray<FItemInstance>& equipSlot);
};


