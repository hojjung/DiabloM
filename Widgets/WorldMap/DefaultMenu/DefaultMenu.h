// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "DiaStatPanel.h"
#include "Blueprint/UserWidget.h"
#include "Item/DiaEquipmentPanel.h"
#include "Skills/DiaSkillPanel.h"
#include "Storage/DiaStorageGridPanel.h"
#include "Talents/DiaTalentPanel.h"
#include "Talents/DiaTalentPopup.h"

#include "DefaultMenu.generated.h"

class AShopKeeper;
class UInventory;
class UEquipmentSystem;
class APlayerDiabloCharacter;
class ADiabloPlayerController;
struct FItemInstance;
class UItemDrop;
class UDiaInvenGridPanel;
class UDiaShopGridSlot;
class UDiaSkillPopup;

UCLASS()
class DIABLOM_API UDefaultMenu : public UUserWidget
{
	GENERATED_BODY()
	//인벤,스텟,장비 그냥 만들것
public:
	static UDefaultMenu* Get;

	~UDefaultMenu();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_MainCanvas;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaInvenGridPanel* m_InvenGridPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaEquipmentPanel* m_EquipPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaStorageGridPanel* m_StoragePanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaShopPanel* m_ShopPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaStatPanel* m_StatPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaSkillPanel* m_SkillPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaTalentPanel* m_TalentPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UItemDrop* m_ItemDropPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UItemPopupInfo* m_ItemPopup1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UItemPopupInfo* m_ItemPopup2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UItemPopupInfo* m_ItemPopup3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaSkillPopup* m_SkillPopup;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaTalentPopup* m_TalentPopup;
protected:
	UPROPERTY()
	ADiabloPlayerController* m_PlayerCon;
	UPROPERTY()
	APlayerDiabloCharacter* m_PlayerChar;
	UPROPERTY()
	UEquipmentSystem* m_Equipment;
	UPROPERTY()
	UInventory* m_Inven;

	TArray<UInventory*>* m_Storage;
	UPROPERTY()
	TArray<UItemPopupInfo*> m_AryItemPopup;

	bool m_bIsPopupOpened;

	bool m_bIsStorageOpened;

	bool m_bIsShopOpened;

	bool m_bIsSkillOpened;
	
public:
	void SetPopupDelegate(const TArray<UDiaInvenGridSlot*>& arySlots);

	void SetPopupDelegateToShopPanel(const TArray<UDiaShopGridSlot*>* ArySlot1);

	void InitPopup();
	
	void Init(ADiabloPlayerController* playerCon, APlayerDiabloCharacter* playerChar,  UEquipmentSystem* equipment,UInventory* inven,TArray<UInventory*>* aryStorage);

	void OpenMainMenu();
	
	void CloseMainMenu();

	void CompareItem(UItemPopupInfo* wantEquip, UItemPopupInfo* equippedOld);
	
	void OpenItemPopup(const FGeometry& geo,FItemInstance& itemInst);//장착된게 어딘지 파악필요

	void CloseItemPopup();

public:
	void OpenStorage();

	void CloseStorage();
	
	void OpenShopMenu(AShopKeeper* shopKeeper);
	
	void CloseShopMenu();
	
	UDiaShopPanel* GetShopPanelWidget();

public:
	void OpenSkillPanel();

	void CloseSkillPanel();

	void OpenSkillPopup(const FGeometry& geo,FSkillDataSpec& skillSpec);

	UFUNCTION()
	void CloseSkillPopup();

	UFUNCTION()
    void CloseTalentPopup();
	
public:
	void OpenTalentPopup(const FGeometry& geo,UTechnologyAsset* talent);

	friend UMainCanvas;
};

