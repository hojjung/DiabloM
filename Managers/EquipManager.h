// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Datas/EquipmentData.h"
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


USTRUCT()
struct FWeaponSpec
{
	GENERATED_BODY()
public:
	int m_nLv =0;
	int m_nIsEquipped = 0;
	int m_nStackCount = 0;
	BigInt m_Value;
	BigInt m_LvlUpCost;
	int m_nAccuracy = 1;
	const FWeaponTable* m_EquipData;

	void SetLevel(int v)
	{
		m_nLv = v;
		m_Value = m_EquipData->GetDmgPer(m_nLv);
		m_LvlUpCost = m_EquipData->GetCost(m_nLv);
		m_nAccuracy = m_EquipData->GetAccuracy(m_nLv);
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
		StrW.AppendInt(m_nIsEquipped);
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
		m_nIsEquipped = FCString::Atoi(*OutStrAry[1]);
		m_nStackCount = FCString::Atoi(*OutStrAry[2]);
	}
	
};


USTRUCT()
struct FPlayerClassSpec
{
	GENERATED_BODY()
public:
	int m_nIsUnlocked = 0;
	int m_nIsEquipped = 0;
	int m_nStackCount = 0;
	float m_fAttackSpeed=1.f;
	const FPlayerSkinTable* m_PlayerData;

	void SetValue()
	{
		m_fAttackSpeed = m_PlayerData->GetAtkSpdBonus();
	}

	FString ParseToString()
	{
		FString StrW;
		StrW.AppendInt(m_nIsUnlocked);
		StrW.Append(":");
		StrW.AppendInt(m_nIsEquipped);
		StrW.Append(":");
		StrW.AppendInt(m_nStackCount);
		StrW.Append(":");

		return StrW;
	}

	void ParseFromString(const FString& str)
	{
		TArray<FString> OutStrAry;
	
		str.ParseIntoArray(OutStrAry,TEXT(":"));

		m_nIsUnlocked = FCString::Atoi(*OutStrAry[0]);
		m_nIsEquipped = FCString::Atoi(*OutStrAry[1]);
		m_nStackCount = FCString::Atoi(*OutStrAry[2]);
	}
};

USTRUCT()
struct FWingSpec
{
	GENERATED_BODY()
public:
	int m_nIsUnlocked = 0;
	int m_nIsEquipped = 0;
	int m_nStackCount = 0;
	const FWingTable* m_WingData;

	FString ParseToString()
	{
		FString StrW;
		StrW.AppendInt(m_nIsUnlocked);
		StrW.Append(":");
		StrW.AppendInt(m_nIsEquipped);
		StrW.Append(":");
		StrW.AppendInt(m_nStackCount);
		StrW.Append(":");

		return StrW;
	}

	void ParseFromString(const FString& str)
	{
		TArray<FString> OutStrAry;
	
		str.ParseIntoArray(OutStrAry,TEXT(":"));

		m_nIsUnlocked = FCString::Atoi(*OutStrAry[0]);
		m_nIsEquipped = FCString::Atoi(*OutStrAry[1]);
		m_nStackCount = FCString::Atoi(*OutStrAry[2]);
	}
};


USTRUCT()
struct FPetSpec
{
	GENERATED_BODY()
public:
	int m_nLv =0;
	int m_nIsEquipped = 0;
	int m_nStackCount = 0;
	
	BigInt m_GoldBonusValue;
	BigInt m_LvlUpCost;
	
	const FPetTable* m_PetData;

	void SetLevel(int lv)
	{
		m_nLv = lv;
		m_GoldBonusValue = m_PetData->GetGoldBonusValue(m_nLv);
		m_LvlUpCost = m_PetData->GetCost(m_nLv);
	}

	FString ParseToString()
	{
		FString StrW;
		StrW.AppendInt(m_nLv);
		StrW.Append(":");
		StrW.AppendInt(m_nIsEquipped);
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
		m_nIsEquipped = FCString::Atoi(*OutStrAry[1]);
		m_nStackCount = FCString::Atoi(*OutStrAry[2]);
	}
};

USTRUCT()
struct FAccessorySpec
{
	GENERATED_BODY()
public:
	int m_nLv =0;
	int m_nIsEquipped = 0;
	int m_nStackCount;
	
	BigInt m_Value;
	int m_LvlUpCost;//combine level up
	
	const FAccessoryTable* m_AccessoryData;

	void SetLevel(int lv)
	{
		m_nLv = lv;
		m_Value = m_AccessoryData->GetValue(m_nLv);
		m_LvlUpCost = m_AccessoryData->GetCost(m_nLv);
	}

	FString ParseToString()
	{
		FString StrW;
		StrW.AppendInt(m_nLv);
		StrW.Append(":");
		StrW.AppendInt(m_nIsEquipped);
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
		m_nIsEquipped = FCString::Atoi(*OutStrAry[1]);
		m_nStackCount = FCString::Atoi(*OutStrAry[2]);
	}
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEquipChanged,int /*pre*/,int /*now*/);

UCLASS()
class DIABLOM_API UEquipManager : public UObject
{
	GENERATED_BODY()
	
public:
	UEquipManager();

	static  UDataTable* GetPlayerSkinDataTable;
	static  UDataTable* GetWeaponDataTable;
	static  UDataTable* GetWingDataTable;
	static  UDataTable* GetPetDataTable;
	static  UDataTable* GetAcceeDataTable;
	
	void ClearSelectedIndex();

public:
	FOnEquipChanged m_OnPlSkinChanged;
	FOnEquipChanged m_OnWeaponChanged;
	FOnEquipChanged m_OnWingChanged;
	FOnEquipChanged m_OnPetChanged;
	FOnEquipChanged m_OnAccessoryChanged1;
	FOnEquipChanged m_OnAccessoryChanged2;
	
	TArray<FPlayerClassSpec> m_AryPlayerSkin;
	TArray<FWingSpec> m_AryWings;
	TArray<FWeaponSpec> m_AryWeapons;
	TArray<FPetSpec> m_AryPets;
	TArray<FAccessorySpec> m_AryAcce;

	int m_nSelectedSkin;
	int m_nSelectedWing;
	int m_nSelectedWeapon;
	int m_nSelectedPet;
	int m_nSelectedAccessory1;
	int m_nSelectedAccessory2;

	UPROPERTY()
	UAccessoryOption* m_AcceSpec1;
	UPROPERTY()
	UAccessoryOption* m_AcceSpec2;

protected:
	int StringSplitEachItem(const FString& equipDatas, TArray<FString>& outStrAry) const;
	
	void SetStringSkinUnlocked(FString skinUnlock);
	
	void SetStringWingUnlocked(FString wingUnlock);
	
	void SetStringWeaponUnlocked(FString weaponUnlock);
	
	void SetStringPetUnlocked(FString petUnlock);
	
	void SetStringAccesoryUnlocked(FString acceUnlock);

public:
	void SetEquipDataFromServer(const FString& classSkin,const FString& weapon,const FString& wing,const FString& pet,const FString& acce);

	void EquipAll();
	
	void TryEquipSkin(int index);
	
	void TryEquipWeapon(int index);

	void TryEquipWing(int index);

	void TryEquipPet(int index);

	void TryEquipAccessory1(int index);

	void TryEquipAccessory2(int index);

public:
	bool TryCombineSkin(int index);
	
	bool TryCombineWing(int index);

	bool TryCombineLevelUpAccessory(int index);

	bool TryCombineWeapon(int index);

	bool TryCombinePet(int index);

public:
	bool TryLvUpWeapon(int index);

	bool TryLvUpPet(int index);

	FORCEINLINE const FWeaponSpec& GetCurrentWeapon()
	{
		return m_AryWeapons[m_nSelectedWeapon];
	}

	FORCEINLINE const FPetSpec* GetCurrentPet()
	{
		if(m_nSelectedPet<0)
		{
			return nullptr;
		}
		return &m_AryPets[m_nSelectedPet];
	}
};
