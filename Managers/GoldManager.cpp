#include "GoldManager.h"


#include "DiabloGameInstance.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"


void UGoldManager::SetCurrentGold(const FString& v)
{
	m_CurrentGold.Parse(v);
	PRINTF("SetGold:%s",*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(m_CurrentGold));
	m_OnGoldChanged.Broadcast();
}

void UGoldManager::AddGold(const BigInt& v)
{
	BigInt FinalV = v;

	if(UDiabloGameInstance::Get->m_EquipManager->GetCurrentPet())
	{
		FinalV.Multiply(100);
		FinalV.Multiply(UDiabloGameInstance::Get->m_EquipManager->GetCurrentPet()->m_GoldBonusValue);
		FinalV.Divide(10000);	
	}
	
	m_CurrentGold.Add(FinalV);

	m_OnGoldChanged.Broadcast();
}

bool UGoldManager::SubtractGold(const BigInt& v)
{
	if(m_CurrentGold<v)
	{
		return false;
	}
	
	m_CurrentGold.Subtract(v);

	m_OnGoldChanged.Broadcast();
	
	return true;
}
