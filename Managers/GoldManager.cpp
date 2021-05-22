#include "GoldManager.h"
#include "DiabloGameInstance.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"


UGoldManager::UGoldManager()
{
	m_nOfflineMinutes = -1;
	m_bIsReceivedOfflineGoldThisTime = false;
}

void UGoldManager::SetCurrentGold(const FString& v, bool bIsNewCreatedPlayer, const FDateTime& currentTime,
                                  const FDateTime& lastLoginTime, const FDateTime& lastLogoutTime)
{
	m_CurrentGold.Parse(v);
	SetOfflineMinutes(bIsNewCreatedPlayer, currentTime, lastLoginTime, lastLogoutTime);
	m_OnCurrencyChanged.Broadcast();
}


void UGoldManager::SetWeaponStones(int v)
{
	m_WeaponStone.SetValue(v);

	m_OnCurrencyChanged.Broadcast();
}

void UGoldManager::SetSkillStones(int v)
{
	m_SkillStone.SetValue(v);

	m_OnCurrencyChanged.Broadcast();
}

void UGoldManager::SetWingTickets(int v)
{
	m_WingTicket.SetValue(v);

	m_OnCurrencyChanged.Broadcast();
}

void UGoldManager::SetGemStones(int v)
{
	m_GemStones.SetValue(v);

	m_OnCurrencyChanged.Broadcast();
}

void UGoldManager::SetDgKeys(int v)
{
	m_DungeonKeys.SetValue(v);

	m_OnCurrencyChanged.Broadcast();
}

void UGoldManager::SetPetTickets(int v)
{
	m_PetGachaTicket.SetValue(v);

	m_OnCurrencyChanged.Broadcast();
}

BigInt UGoldManager::AddGold(const BigInt& v, bool useBonus)
{
	BigInt FinalV = v;

	if (useBonus)
	{
		if (UDiabloGameInstance::Get->m_EquipManager->GetCurrentPet())
		{
			FinalV = UDiaBlueprintFunctionLibrary::MultiplePercent(
				FinalV, UDiabloGameInstance::Get->m_EquipManager->GetCurrentPet()->m_GoldBonusValue);
		}

		// if (UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce01).Level > 0)
		// {
		// 	FinalV = UDiaBlueprintFunctionLibrary::MultiplePercent(
		// 		FinalV, UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce01).m_Value);
		// }

		if (UDiabloGameInstance::Get->m_ShopManager->GetPackagePurchased(0))
		{
			int MultipleFactor = 5;

			if (UDiabloGameInstance::Get->m_ShopManager->GetPackagePurchased(1))
			{
				MultipleFactor = 10;

				if (UDiabloGameInstance::Get->m_ShopManager->GetPackagePurchased(2))
				{
					MultipleFactor = 30;

					if (UDiabloGameInstance::Get->m_ShopManager->GetPackagePurchased(3))
					{
						MultipleFactor = 100;

						if (UDiabloGameInstance::Get->m_ShopManager->GetPackagePurchased(4))
						{
							MultipleFactor = 1000;
						}
					}
				}
			}

			FinalV.Multiply(MultipleFactor);
		}
	}

	m_CurrentGold.Add(FinalV);

	m_OnCurrencyChanged.Broadcast();

	return FinalV;
}

bool UGoldManager::SubtractGold(const BigInt& v)
{
	if (m_CurrentGold < v)
	{
		return false;
	}

	m_CurrentGold.Subtract(v);

	m_OnCurrencyChanged.Broadcast();

	return true;
}

int UGoldManager::AddWeaponStones(int v, bool useBonus)
{
	m_WeaponStone += v;
	
	UDiabloGameInstance::Get->m_PlayfabManager->AddWeaponStones(v);

	m_OnCurrencyChanged.Broadcast();

	return m_WeaponStone.GetValue();
}

bool UGoldManager::SubtractWeaponStones(int v)
{
	if (m_WeaponStone < v)
	{
		return false;
	}

	m_WeaponStone -= v;

	m_OnCurrencyChanged.Broadcast();

	return true;
}

int UGoldManager::AddSkillStones(int v, bool useBonus)
{
	m_SkillStone += v;
	
	UDiabloGameInstance::Get->m_PlayfabManager->AddSkillStones(v);

	m_OnCurrencyChanged.Broadcast();

	return m_SkillStone.GetValue();
}

bool UGoldManager::SubtractSkillStones(int v)
{
	if (m_SkillStone < v)
	{
		return false;
	}

	m_SkillStone -= v;

	m_OnCurrencyChanged.Broadcast();

	return true;
}

int UGoldManager::AddPetGachaTicket(int v, bool useBonus)
{
	m_PetGachaTicket += v;

	UDiabloGameInstance::Get->m_PlayfabManager->AddPetTicket(v);

	m_OnCurrencyChanged.Broadcast();

	return m_PetGachaTicket.GetValue();
}

bool UGoldManager::SubtractPetGachaTicket(int v)
{
	if (m_PetGachaTicket < v)
	{
		return false;
	}

	m_PetGachaTicket -= v;

	m_OnCurrencyChanged.Broadcast();

	return true;
}

int UGoldManager::AddWingTicket(int v, bool useBonus)
{
	m_WingTicket += v;

	UDiabloGameInstance::Get->m_PlayfabManager->AddWingTicket(v);

	m_OnCurrencyChanged.Broadcast();

	return m_WingTicket.GetValue();
}

bool UGoldManager::SubtractWingTicket(int v)
{
	if (m_WingTicket < v)
	{
		return false;
	}

	m_WingTicket -= v;

	m_OnCurrencyChanged.Broadcast();

	return true;
}

int UGoldManager::AddGemStones(int v)
{
	m_GemStones += v;

	UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(v);

	m_OnCurrencyChanged.Broadcast();

	return m_GemStones.GetValue();
}

bool UGoldManager::SubtractGemStones(int v)
{
	if (m_GemStones < v)
	{
		return false;
	}

	m_GemStones -= v;

	m_OnCurrencyChanged.Broadcast();

	return true;
}

int UGoldManager::AddDungeonKeys(int v)
{
	m_DungeonKeys += v;
	
	UDiabloGameInstance::Get->m_PlayfabManager->AddDgKey(v);

	m_OnCurrencyChanged.Broadcast();

	return m_DungeonKeys.GetValue();
}

bool UGoldManager::SubtractDungeonKeys(int v)
{
	if (m_DungeonKeys < v)
	{
		return false;
	}

	m_DungeonKeys -= v;

	m_CachedDgKeys+=v;

	m_OnCurrencyChanged.Broadcast();

	return true;
}

void UGoldManager::UploadCachedDgkeys()
{
	if(m_CachedDgKeys.GetValue()>0)
	{
		UDiabloGameInstance::Get->m_PlayfabManager->SubtractDgKey(m_CachedDgKeys.GetValue());
		m_CachedDgKeys=0;
	}
}

void UGoldManager::SetOfflineMinutes(bool bIsNewCreatedPlayer, const FDateTime& currentTime,
                                     const FDateTime& lastLoginTime, const FDateTime& lastLogoutTime)
{
	if (bIsNewCreatedPlayer)
	{
		return;
	}

	if (m_bIsReceivedOfflineGoldThisTime)
	{
		return;
	}

	FTimespan OfflineTimeSpawn;

	bool LogoutTimeIsValid = (lastLogoutTime - lastLoginTime).GetTotalMinutes() > 0; //check time fixed device

	if (LogoutTimeIsValid)
	{
		OfflineTimeSpawn = currentTime - lastLogoutTime;
	}
	else
	{
		OfflineTimeSpawn = currentTime - lastLoginTime;
	}

	m_nOfflineMinutes = OfflineTimeSpawn.GetTotalMinutes();

	if (m_nOfflineMinutes < 2)
	{
		return;
	}

	m_nOfflineMinutes = FMath::Clamp(m_nOfflineMinutes, 2, 1440);

	BigInt Bounty = UDiabloGameInstance::Get->m_NormalDgManager->GetMaxDungeonBounty();

	Bounty.Multiply(3 * m_nOfflineMinutes);

	// if (UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce08).Level > 0)
	// {
	// 	Bounty = UDiaBlueprintFunctionLibrary::MultiplePercent(
	// 		Bounty, UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce08).m_Value);
	// }

	m_OfflineGold = AddGold(Bounty, false);
}

FString UGoldManager::GetGoldDataStr()
{
	return GetCurrentGold().ToString();
}

bool UGoldManager::IsOfflineGoldAvailable()
{
	return !m_bIsReceivedOfflineGoldThisTime && m_nOfflineMinutes >= 2;
}

void UGoldManager::Confirm()
{
	m_bIsReceivedOfflineGoldThisTime = true;

	m_nOfflineMinutes = -1;

	m_OfflineGold.Zero();
}
