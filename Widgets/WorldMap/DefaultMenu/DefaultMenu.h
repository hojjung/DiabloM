// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/WorldMap/DefaultMenu/DiaEquipmentPanel.h"
#include "Widgets/WorldMap/DefaultMenu/ItemPopupInfo.h"
#include "DefaultMenu.generated.h"


class InventoryOld;
class EquipmentSystemOld;
class PlayerSkillSystemOld;
class APlayerDiabloCharacter;
class ADiabloPlayerController;
struct FItemInstance;
class UDiaInvenGridPanel;
class UItemDrop;
UCLASS()
class DIABLOM_API UDefaultMenu : public UUserWidget
{
	GENERATED_BODY()
	//인벤,스텟,장비 그냥 만들것

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaInvenGridPanel* m_InvenGridPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaEquipmentPanel* m_EquipPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UItemDrop* m_ItemDropPanel;
protected:
	UPROPERTY()
	ADiabloPlayerController* m_PlayerCon;
	UPROPERTY()
	APlayerDiabloCharacter* m_PlayerChar;

	PlayerSkillSystemOld* m_Skill;

	EquipmentSystemOld* m_Equipment;

	InventoryOld* m_Inven;

public:
	void Init(ADiabloPlayerController* playerCon, APlayerDiabloCharacter* playerChar, PlayerSkillSystemOld* skill, EquipmentSystemOld* equipment,InventoryOld* inven);

	void OpenMainMenu();

	void CloseMainMenu();

	UFUNCTION(BlueprintCallable)
	void ThrowItem(const FItemInstance& itemThrow);

};
