#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Item/EquipmentSystem.h"
#include "DiaEquipmentPanel.generated.h"

struct FItemInstance;
class UItemPopupInfo;
class UDiaInvenGridSlot;
UCLASS()
class DIABLOM_API UDiaEquipmentPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaInvenGridSlot* m_SlotHead;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaInvenGridSlot* m_SlotNeck;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaInvenGridSlot* m_SlotTorso;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaInvenGridSlot* m_SlotWaist;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaInvenGridSlot* m_SlotLeg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaInvenGridSlot* m_SlotHand;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaInvenGridSlot* m_SlotShoulder;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaInvenGridSlot* m_SlotWeaponLeft;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaInvenGridSlot* m_SlotWeaponRight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaInvenGridSlot* m_SlotFingerLeft;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaInvenGridSlot* m_SlotFingerRight;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UItemPopupInfo* m_ItemPopup;

protected:
	UPROPERTY()
	UEquipmentSystem* m_EquipSys;
	UPROPERTY()
	TArray<UDiaInvenGridSlot*> m_ArySlots;

public:
	FORCEINLINE TArray<UDiaInvenGridSlot*>& GetArySlots()
	{
		return m_ArySlots;
	}

protected:
	int m_nPopupSelectedIndex;

public:
	void Init(UEquipmentSystem* equipContainer);

	bool EquipItem(int dropIndex, FItemInstance& drag);
	
	void UnequipItem(int dropIndex);

protected:
	bool CheckItemEquipable(int dropIndex, FItemInstance& drag);

	void UpdateSlot(int index,  FItemInstance& itemInst);

	void UpdateStance(EAnimStance currentStance);


	void HideItemInfo();

public:
	UFUNCTION()
	void ShowItemInfo(const FGeometry &theInstigator,  FItemInstance& itemInst);
	static UDiaEquipmentPanel* GetEquipWidgetInst;
	
};
