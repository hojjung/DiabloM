// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Datas/EquipmentData.h"
#include "EquipManager.generated.h"

class UPlayFabJsonValue;
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

UENUM()
enum class EAccessory :uint8
{
	Acce01,
	Acce02,
	Acce03,
	Acce04,
	Acce05,
	Acce06,
	Acce07,
	Acce08,
    LENGTH
};

class UPlayfabManager;

//레벨업그레이드 있는 장비는 레벨0이 잠김,레벨1이 해금,그이상이 레벨임
USTRUCT()
struct FWeaponSpec
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int Level = 0;
	UPROPERTY(EditAnywhere)
	bool IsEquipped = false;
	UPROPERTY(EditAnywhere)
	int StackCount = 0;
	BigInt m_Value;
	BigInt m_LvlUpCost;
	const FWeaponTable* m_EquipData;

	void SetLevel(int v)
	{
		Level = v;
		m_Value = m_EquipData->GetDmgPer(Level);
		m_LvlUpCost = m_EquipData->GetCost(Level);
	}

	int GetMaxLv() const
	{
		return 100;
	}
};


USTRUCT()
struct FPlayerClassSpec
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	bool IsUnlocked = false;
	UPROPERTY(EditAnywhere)
	bool IsEquipped = false;
	UPROPERTY(EditAnywhere)
	int StackCount = 0;
	
	float m_fAttackSpeed=1.f;
	const FPlayerSkinTable* m_PlayerData;

	void SetValue()
	{
		m_fAttackSpeed = m_PlayerData->GetAtkSpdBonus();
	}
};

USTRUCT()
struct FWingSpec
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	bool IsUnlocked = false;
	UPROPERTY(EditAnywhere)
	bool IsEquipped = false;
	UPROPERTY(EditAnywhere)
	int StackCount = 0;
	
	const FWingTable* m_WingData;
};


USTRUCT()
struct FPetSpec
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int Level = 0;
	UPROPERTY(EditAnywhere)
	bool IsEquipped = false;
	UPROPERTY(EditAnywhere)
	int StackCount = 0;
	
	BigInt m_GoldBonusValue;
	BigInt m_LvlUpCost;
	
	const FPetTable* m_PetData;

	void SetLevel(int lv)
	{
		Level = lv;
		m_GoldBonusValue = m_PetData->GetGoldBonusValue(Level);
		m_LvlUpCost = m_PetData->GetCost(Level);
	}
};

USTRUCT()
struct FAccessorySpec
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int Level;
	UPROPERTY(EditAnywhere)
	bool IsUnlocked = false;
	UPROPERTY(EditAnywhere)
	bool IsEquipped = false;
	UPROPERTY(EditAnywhere)
	int StackCount = 0;
	
	BigInt m_Value;
	int m_LvlUpCost;//combine level up
	float m_fFloatValue;
	
	const FAccessoryTable* m_AccessoryData;

	void SetLevel(int lv)
	{
		Level = lv;
		m_Value = m_AccessoryData->GetValue(Level);
		m_LvlUpCost = m_AccessoryData->GetCost(Level);
		m_fFloatValue=m_AccessoryData->GetFloatValue(Level);
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

protected:
	TMap<const FGachaAbleRow*,int> m_MapPlayerSkin;
	
	TMap<const FGachaAbleRow*,int> m_MapPlayerPet;

	TMap<const FGachaAbleRow*,int> m_MapPlayerWeapon;

	TMap<const FGachaAbleRow*,int> m_MapAccessory;
	
public:
	FOnEquipChanged m_OnPlSkinChanged;
	FOnEquipChanged m_OnWeaponChanged;
	FOnEquipChanged m_OnWingChanged;
	FOnEquipChanged m_OnPetChanged;
	FOnEquipChanged m_OnAccessoryChanged;

	
	TArray<FPlayerClassSpec> m_AryPlayerSkin;
	TArray<FWingSpec> m_AryWings;
	TArray<FWeaponSpec> m_AryWeapons;
	TArray<FPetSpec> m_AryPets;
	TArray<FAccessorySpec> m_AryAcce;

	UPROPERTY()
	int m_nSelectedSkin;
	UPROPERTY()
	int m_nSelectedWing;
	UPROPERTY()
	int m_nSelectedWeapon;
	UPROPERTY()
	int m_nSelectedPet;

protected:
	int StringSplitEachItem(const FString& equipDatas, TArray<FString>& outStrAry) const;
	
	void SetStringSkinUnlocked(const TArray<UPlayFabJsonValue*>&  skinUnlock);
	
	void SetStringWeaponUnlocked(const TArray<UPlayFabJsonValue*>&  weaponUnlock);
	
	void SetStringPetUnlocked(const TArray<UPlayFabJsonValue*>&  petUnlock);
	
	void SetStringAccesoryUnlocked(const TArray<UPlayFabJsonValue*>&  acceUnlock);

public:
	void SetEquipDataFromServer(const TArray<UPlayFabJsonValue*>& classSkin, const TArray<UPlayFabJsonValue*>& weapon,const TArray<UPlayFabJsonValue*>& pet);

	void EquipAll();
	
	void TryEquipSkin(int index);
	
	void TryEquipWeapon(int index);

	void TryEquipWing(int index);

	void TryEquipPet(int index);


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

public:
	void AddWeaponStack(const FGachaAbleRow* weaponData);

	void AddSkinStack(const FGachaAbleRow* skinData);

	void AddAccessoryStack(const FGachaAbleRow* acceData);
	//
	void AddWeaponStack(int index);

	void AddSkinStack(int index);

	void AddPetStack(int index);
	
	void AddWingStack(int index);

	void AddAccessoryStack(int index);

	FAccessorySpec& GetAccessory(EAccessory acces);

	FString GetWeaponDataStr();

	FString GetSkinDataStr();

	FString GetPetDataStr();

	FString GetAccessoryDataStr();

	FString GetWingDataStr();

};


