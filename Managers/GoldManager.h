// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "DiabloM.h"
#include "UObject/NoExportTypes.h"
#include "GoldManager.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnGoldChanged);
UCLASS()
class DIABLOM_API UGoldManager : public UObject
{
	GENERATED_BODY()

protected:
	BigInt m_CurrentGold;

	BigInt m_OfflineGold;

	int m_nMinute = 0;
public:
	FOnGoldChanged m_OnGoldChanged;
	
public:
	void SetCurrentGold(const FString& v);
	
	const BigInt& GetCurrentGold() const
	{
		return m_CurrentGold;
	}

	const BigInt& GetFinalOfflineGold() const
	{
		return m_OfflineGold;
	}

	int GetClampedOfflineMinutes()
	{
		return m_nMinute;
	}

	BigInt AddGold(const BigInt& v);

	bool SubtractGold(const BigInt& v);
	
	void GainOfflineGold();

	void SetOfflineMinutes(int minutes);
};
