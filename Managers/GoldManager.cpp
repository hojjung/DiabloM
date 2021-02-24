#include "GoldManager.h"

#include "Lib/DiaBlueprintFunctionLibrary.h"


void UGoldManager::SetCurrentGold(const FString& v)
{
	m_CurrentGold.Parse(v);
	PRINTF("SetGold:%s",*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(m_CurrentGold));
}

void UGoldManager::AddGold(const BigInt& v)
{
	m_CurrentGold.Add(v);

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
