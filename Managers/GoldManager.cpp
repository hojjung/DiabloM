#include "GoldManager.h"


#include "DiabloGameInstance.h"
#include "DungeonManager.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"


UGoldManager::UGoldManager()
{
	m_nOfflineMinutes = -1;
	m_bIsReceivedOfflineGoldThisTime = false;
}

void UGoldManager::SetCurrentGold(const FString& v, bool bIsNewCreatedPlayer, const FDateTime& currentTime,const FDateTime& lastLoginTime, const FDateTime& lastLogoutTime)
{
	m_CurrentGold.Parse(v);
	SetOfflineMinutes(bIsNewCreatedPlayer, currentTime, lastLoginTime, lastLogoutTime);
	m_OnGoldChanged.Broadcast();
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

		if (UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce01).m_nLv > 0)
		{
			FinalV = UDiaBlueprintFunctionLibrary::MultiplePercent(
				FinalV, UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce01).m_Value);
		}

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

	m_OnGoldChanged.Broadcast();

	return FinalV;
}

bool UGoldManager::SubtractGold(const BigInt& v)
{
	if (m_CurrentGold < v)
	{
		return false;
	}

	m_CurrentGold.Subtract(v);

	m_OnGoldChanged.Broadcast();

	return true;
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

	BigInt Bounty = UDiabloGameInstance::Get->m_DungeonManager->GetMaxDungeonBounty();

	Bounty.Multiply(3 * m_nOfflineMinutes);

	if (UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce08).m_nLv > 0)
	{
		Bounty = UDiaBlueprintFunctionLibrary::MultiplePercent(
			Bounty, UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce08).m_Value);
	}

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
