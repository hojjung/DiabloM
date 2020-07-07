// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/DiaEquipmentPanel.h"
#include "Widgets/ItemPopupInfo.h"
#include "DefaultMenu.generated.h"


class Inventory;
class EquipmentSystem;
class PlayerSkillSystem;
class APlayerDiabloCharacter;
class ADiabloPlayerController;
struct FItemInstance;
class UDiaInvenGridPanel;

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
protected:
	UPROPERTY()
	ADiabloPlayerController* m_PlayerCon;
	UPROPERTY()
	APlayerDiabloCharacter* m_PlayerChar;

	PlayerSkillSystem* m_Skill;

	EquipmentSystem* m_Equipment;

	Inventory* m_Inven;

public:
	void Init(ADiabloPlayerController* playerCon, APlayerDiabloCharacter* playerChar, PlayerSkillSystem* skill, EquipmentSystem* equipment,Inventory* inven);

	

};
