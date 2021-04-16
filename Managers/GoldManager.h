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

public:
	UGoldManager();

protected:
	BigInt m_CurrentGold;

	

public:
	BigInt m_OfflineGold;
	UPROPERTY()
	int m_nOfflineMinutes;
	UPROPERTY()
	bool m_bIsReceivedOfflineGoldThisTime;

public:
	FOnGoldChanged m_OnGoldChanged;

public:
	void SetCurrentGold(const FString& v,bool bIsNewCreatedPlayer,const FDateTime& currentTime,const FDateTime& lastLoginTime,const FDateTime& lastLogoutTime);
	
	const BigInt& GetCurrentGold() const
	{
		return m_CurrentGold;
	}

	const BigInt& GetFinalOfflineGold() const
	{
		return m_OfflineGold;
	}


	BigInt AddGold(const BigInt& v,bool useBonus=true);

	bool SubtractGold(const BigInt& v);
	
	void SetOfflineMinutes(bool bIsNewCreatedPlayer,const FDateTime& currentTime,const FDateTime& lastLoginTime,const FDateTime& lastLogoutTime);


	FString GetGoldDataStr();

	bool IsOfflineGoldAvailable();
	
	void Confirm();
};
