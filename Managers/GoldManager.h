// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "UObject/NoExportTypes.h"
#include "GoldManager.generated.h"



UCLASS()
class DIABLOM_API UGoldManager : public UObject
{
	GENERATED_BODY()

public:
	UGoldManager();

	DECLARE_MULTICAST_DELEGATE(FOnCurrencyChanged);

	FOnCurrencyChanged m_OnCurrencyChanged;

protected:
	BigInt m_CurrentGold;

	FSafeInt m_WeaponStone;

	FSafeInt m_SkillStone;

	FSafeInt m_PetGachaTicket;

	FSafeInt m_WingTicket;

	FSafeInt m_GemStones;

	FSafeInt m_DungeonKeys;

	FSafeInt m_CachedDgKeys;

public:
	BigInt m_OfflineGold;
	UPROPERTY()
	int m_nOfflineMinutes;
	UPROPERTY()
	bool m_bIsReceivedOfflineGoldThisTime;

public:
	void SetCurrentGold(const FString& v,bool bIsNewCreatedPlayer,const FDateTime& currentTime,const FDateTime& lastLoginTime,const FDateTime& lastLogoutTime);

	void SetWeaponStones(int v);

	void SetSkillStones(int v);

	void SetWingTickets(int v);

	void SetGemStones(int v);

	void SetDgKeys(int v);

	void SetPetTickets(int v);

	void SetOfflineMinutes(bool bIsNewCreatedPlayer,const FDateTime& currentTime,const FDateTime& lastLoginTime,const FDateTime& lastLogoutTime);

	FString GetGoldDataStr();

	bool IsOfflineGoldAvailable();
	
	void Confirm();

public:
	BigInt AddGold(const BigInt& v,bool useBonus=true);

	bool SubtractGold(const BigInt& v);
	
	const BigInt& GetCurrentGold() const
	{
		return m_CurrentGold;
	}

	const BigInt& GetFinalOfflineGold() const
	{
		return m_OfflineGold;
	}

	int AddWeaponStones(int v,bool useBonus=true);

	bool SubtractWeaponStones(int v);

	int GetWeaponStone() const
	{
		return m_WeaponStone.GetValue();
	}

	int AddSkillStones(int v,bool useBonus=true);

	bool SubtractSkillStones(int v);

	int GetSkillStone() const
	{
		return m_SkillStone.GetValue();
	}

	int AddPetGachaTicket(int v,bool useBonus=true);
	
	bool SubtractPetGachaTicket(int v);

	int GetPetGachaTicket() const
	{
		return m_PetGachaTicket.GetValue();
	}

	int AddWingTicket(int v,bool useBonus=true);

	bool SubtractWingTicket(int v);

	int GetWingTicket() const
	{
		return m_WingTicket.GetValue();
	}

	int AddGemStones(int v);

	bool SubtractGemStones(int v);

	int GetGemStones() const
	{
		return m_GemStones.GetValue();
	}

	int AddDungeonKeys(int v);

	bool SubtractDungeonKeys(int v);

	int GetDungeonKeys() const
	{
		return m_DungeonKeys.GetValue();
	}

	void UploadCachedDgkeys();
};
