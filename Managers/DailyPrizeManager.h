// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "UObject/NoExportTypes.h"
#include "DailyPrizeManager.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UDailyPrizeManager : public UObject
{
	GENERATED_BODY()
public:
	UDailyPrizeManager();
	
public:
	UPROPERTY()
	int m_nDDay;
	UPROPERTY()
	bool m_bIsAbleGetDailyPrize;
	UPROPERTY()
	TArray<int> m_AryClaimGemstone;
	
	//
public:
	void SetPrizeManager(const FString& prizeJsonStr,const FDateTime& currentTime,bool bIsFirst);
	UFUNCTION()
	void Claim();
};
