#pragma once


#include "DiabloM.h"

#include "Datas/CharacterDataTable.h"
#include "Datas/ItemDataTable.h"
#include "Datas/PlayerInitDataTable.h"
#include "Item/ItemManager.h"
#include "SaveLoad/SaveLoadManager.h"
#include "DiabloGameInstance.generated.h"

class UPlayerCreateManager;
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
	UPROPERTY()
	UItemManager* m_ItemManager;
	UPROPERTY()
	USaveLoadManager* m_SaveLoadManager;
	UPROPERTY()
	UPlayerCreateManager* m_PlCreateManager;

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

	ADroppedItem* DropItemActor(FItemInstance& myItem);
};
