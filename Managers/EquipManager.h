// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Datas/CharacterDataTable.h"
#include "UObject/NoExportTypes.h"
#include "EquipManager.generated.h"

UENUM()
enum EEquipSlot
{
	SKIN = 1,
	WING,
	WEAPON,
	ACCE1,
	ACCE2,
	PET1,
	PET2,
	PET3,
	LEN
};

class UPlayfabManager;
USTRUCT(BlueprintType)
struct FEquipmentDataRow : public FUpgradeDataRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName m_NameID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AEquipmentActor> m_ClassEquipment = nullptr;
};

USTRUCT()
struct FEquipmentSpec
{
	GENERATED_BODY()
public:
	int m_nLv =0;
	int m_nEquippedSlot = 0;
	int m_nStackCount = 0;
	BigInt m_Value;
	BigInt m_LvlUpCost;
	const FEquipmentDataRow* m_EquipData;

	void SetLevel(int v)
	{
		m_nLv = v;
		m_Value = m_EquipData->GetValue(m_nLv);
		m_LvlUpCost = m_EquipData->GetCost(m_nLv);
	}

	int GetMaxLv() const
	{
		return m_EquipData->m_nMaxLevel;
	}

	FString ParseToString()
	{
		FString StrW;
		
		StrW.AppendInt(m_nLv);
		StrW.Append(":");
		StrW.AppendInt(m_nEquippedSlot);
		StrW.Append(":");
		StrW.AppendInt(m_nStackCount);
		StrW.Append(":");

		return StrW;
	}

	void ParseFromString(const FString& str)
	{
		TArray<FString> OutStrAry;
	
		str.ParseIntoArray(OutStrAry,TEXT(":"));

		m_nLv = FCString::Atoi(*OutStrAry[0]);
		m_nEquippedSlot = FCString::Atoi(*OutStrAry[1]);
		m_nStackCount = FCString::Atoi(*OutStrAry[2]);
	}
	
};


USTRUCT()
struct FPlayerClassSpec
{
	GENERATED_BODY()
public:
	int m_nLv =0;
	int m_nEquippedSlot = 0;
	int m_nStackCount = 0;
	BigInt m_Value;
	BigInt m_LvlUpCost;
	const FPlayerEntityTable* m_PlayerData;

	void SetLevel(int v)
	{
		m_nLv = v;
		m_Value = m_PlayerData->GetValue(m_nLv);
		m_LvlUpCost = m_PlayerData->GetCost(m_nLv);
	}

	int GetMaxLv() const
	{
		return m_PlayerData->m_nMaxLevel;
	}

	FString ParseToString()
	{
		FString StrW;
		StrW.AppendInt(m_nLv);
		StrW.Append(":");
		StrW.AppendInt(m_nEquippedSlot);
		StrW.Append(":");
		StrW.AppendInt(m_nStackCount);
		StrW.Append(":");

		return StrW;
	}

	void ParseFromString(const FString& str)
	{
		TArray<FString> OutStrAry;
	
		str.ParseIntoArray(OutStrAry,TEXT(":"));

		m_nLv = FCString::Atoi(*OutStrAry[0]);
		m_nEquippedSlot = FCString::Atoi(*OutStrAry[1]);
		m_nStackCount = FCString::Atoi(*OutStrAry[2]);
	}

	
	
};

UCLASS()
class DIABLOM_API UEquipManager : public UObject
{
	GENERATED_BODY()
	
public:
	UEquipManager();
	
	static  UDataTable* GetWeaponDataTable;
	static  UDataTable* GetWingDataTable;
	static  UDataTable* GetPetDataTable;
	static  UDataTable* GetAcceeDataTable;

public:
	TArray<FPlayerClassSpec> m_AryPlayerSkin;
	TArray<FEquipmentSpec> m_AryWings;
	TArray<FEquipmentSpec> m_AryWeapons;
	TArray<FEquipmentSpec> m_AryPets;
	TArray<FEquipmentSpec> m_AryAcce;

	const FPlayerClassSpec* m_CurrentSelectedSkin;

protected:
	TArray<const FPlayerEntityTable*> m_AryPlayerClass;
	
	TArray<const FEquipmentDataRow*> m_AryEquipDatas;

	TArray<const FEquipmentDataRow*> m_AryWeaponDatas;
	
	TArray<const FEquipmentDataRow*> m_AryAccessDatas;

	TArray<const FEquipmentDataRow*> m_AryPetDatas;
	
protected:
	void StringToIntAry(const FString& skinUnlock, TArray<int>& outContent) const;

	int StringSplitEachItem(const FString& equipDatas, TArray<FString>& outStrAry) const;

	void SetStringSkinUnlocked(FString skinUnlock);
	void SetStringWingUnlocked(FString wingUnlock);
	void SetStringWeaponUnlocked(FString weaponUnlock);
	void SetStringPetUnlocked(FString petUnlock);
	void SetStringAccesoryUnlocked(FString acceUnlock);
	
public:
	void SetEquipDataFromServer(const FString& classSkin,const FString& weapon,const FString& wing,const FString& pet,const FString& acce);
	
	const FPlayerClassSpec* TryEquipSkin(const FPlayerClassSpec* player_class_spec);
	
	void TryEquipEquipment(const FEquipmentSpec* equipment_spec);
};
