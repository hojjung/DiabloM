#include "GoldManager.h"


#include "DiabloGameInstance.h"
#include "DungeonManager.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"


UGoldManager::UGoldManager()
{
	m_nMinute = 0;
	m_bIsServerMinuteGained = false;
}

void UGoldManager::SetCurrentGold(const FString& v)
{
	m_CurrentGold.Parse(v);
	PRINTF("SetGold:%s", *UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(m_CurrentGold));
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

bool UGoldManager::GainOfflineGold()
{
	if (m_nMinute < 2)
	{
		return false;
	}

	m_nMinute = FMath::Clamp(m_nMinute, 1, 1440);

	BigInt Bounty = UDiabloGameInstance::Get->m_DungeonManager->GetCurrentDungeonBounty();

	Bounty.Multiply(5 * m_nMinute);

	if (UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce08).m_nLv > 0)
	{
		Bounty = UDiaBlueprintFunctionLibrary::MultiplePercent(
			Bounty, UDiabloGameInstance::Get->m_EquipManager->GetAccessory(EAccessory::Acce08).m_Value);
	}

	m_OfflineGold = AddGold(Bounty);

	m_bIsServerMinuteGained = false;

	return true;
}

void UGoldManager::SetOfflineMinutes(int minutes)
{
	m_nMinute = minutes;

	m_bIsServerMinuteGained = true;
}
