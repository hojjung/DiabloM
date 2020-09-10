// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Datas/CharacterDataTable.h"
#include "Datas/ItemDataTable.h"
#include "Datas/PlayerInitDataTable.h"
#include "Managers/ItemManagerOld.h"
#include  "SaveLoad/SaveLoadManagerOld.h"
#include "DiabloGameInstance.generated.h"

class PlayerCreateManagerOld;
struct FItemInstance;
struct FItemTier;
struct FItemTier;

UCLASS()
class DIABLOM_API UDiabloGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UDiabloGameInstance();//
	static UDiabloGameInstance* Get;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ADroppedItem> m_DropItemClass;
	
public:
	ItemManagerOld* m_ItemManagerOld;
	SaveLoadManagerOld* m_SaveLoadManagerOld;
	PlayerCreateManagerOld* m_PlCreateManager;

protected:
	virtual void Init() override;
	
	virtual void Shutdown()override;

public:
	
#pragma region DataGetter
	
	const FEntityTable* GetMonsterUnitPtr(FName id) const;
	const FNPCEntityTable* GetNPCUnitPtr(FName id) const;
	const FPlayerEntityTable* GetPlayerUnitPtr(FName id) const;
	const FItemTier* GetItemTierPtr(FName id)const;
	const FItemData* GetItemDataPtr(FName id)const;
	const FOption* GetOptionPtr(FName id)const;
	const FPlayerHairRow* GetPlayerHairPtr(FName id)const;
	const FPlayerFaceRow* GetPlayerFacePtr(FName id)const;
	const FPlayerArmorRow* GetPlayerArmorPtr(FName id)const;
	const FPlayerItemRow* GetPlayerItemPtr(FName id)const;
	const FPlayerPerkRow* GetPlayerPerkPtr(FName id)const;
	//
	const FEntityTable& GetMonsterUnit(FName id) const;
	const FNPCEntityTable& GetNPCUnit(FName id) const;
	const FPlayerEntityTable& GetPlayerUnit(FName id) const;
	const FItemTier& GetItemTier(FName id)const;
	const FItemData& GetItemData(FName id)const;
	const FOption& GetOption(FName id)const;
	const FPlayerHairRow  & GetPlayerHair(FName id)const;
	const FPlayerFaceRow  & GetPlayerFace(FName id)const;
	const FPlayerArmorRow & GetPlayerArmor(FName id)const;
	const FPlayerItemRow  & GetPlayerItem(FName id)const;
	const FPlayerPerkRow  & GetPlayerPerk(FName id)const;
	
#pragma endregion 
	//
	FItemInstance CreateItem(FName id);
};
