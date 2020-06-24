// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DefaultMenu.generated.h"


class Inventory;
class EquipmentSystem;
class PlayerSkillSystem;
class APlayerDiabloCharacter;
class ADiabloPlayerController;
class ItemInstance;
class UDiaInvenGridPanel;

UCLASS()
class DIABLOM_API UDefaultMenu : public UUserWidget
{
	GENERATED_BODY()
	//인벤,스텟,장비 그냥 만들것

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UDiaInvenGridPanel* m_InvenGridPanel;
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

	void UpdateStatPanel();

	//UFUNCTION(BlueprintImplementableEvent)
	//void AddItemToIndex(int Index,ItemInstance* itemInstance);
	//스텟
	//장비칸
	//인벤

	//아이템인스턴스 구조체로 바꿀것
};
