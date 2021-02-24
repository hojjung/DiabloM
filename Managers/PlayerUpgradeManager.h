// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "DiabloM.h"
#include "PlayfabManager.h"
#include "UObject/NoExportTypes.h"
#include "PlayerUpgradeManager.generated.h"

struct FUpgradeDataRow;

DECLARE_MULTICAST_DELEGATE(FOnUpgradeLevelChanged);

UCLASS()
class DIABLOM_API UPlayerUpgradeManager : public UObject
{
	GENERATED_BODY()
	
public:
	FOnUpgradeLevelChanged m_OnUpgradeChanged;
	
public:
	UPROPERTY()
	UPlayfabManager* m_PlayfabManager;
	UPROPERTY()
	int m_nPlAtkDmg01Lv;
	UPROPERTY()
	bool m_bIsPlAtkDmg01Able;
	const FUpgradeDataRow* m_PlAtkDmg01Upgrade;
	UPROPERTY()
	int m_nPlAtkCri01Lv;
	UPROPERTY()
	bool m_bIsPlAtkCri01Able;
	const FUpgradeDataRow* m_PlAtkCri01Upgrade;
	UPROPERTY()
	int m_nPlAtkCDmg01Lv;
	UPROPERTY()
	bool m_bIsPlAtkCDmg01Able;
	const FUpgradeDataRow* m_PlAtkCDmg01Upgrade;
	
protected:
	BigInt m_PlAtkDmg01;
	BigInt m_PlAtkCri01;
	BigInt m_PlAtkCDmg01;
	
protected:	
	BigInt m_PlAtkDmg01Cost;
	BigInt m_PlAtkCri01Cost;
	BigInt m_PlAtkCDmg01Cost;
	
public:
	void SetUpgradeDataFromServer(UPlayfabManager* plMan);
	void SetPlAtkDmg01();
	void SetPlAtkCri01();
	void SetPlAtkCDmg01();
	
public:
	void SetPlAtkDmg01Cost();
	void SetPlAtkCri01Cost();
	void SetPlAtkCDmgCost();

public://value
	BigInt& GetPlAtkDmg01();
	BigInt& GetPlAtkCri01();
	BigInt& GetPlAtkCDmg01();

public://cost
	BigInt& GetPlAtkDmg01Cost();
	BigInt& GetPlAtkCri01Cost();
	BigInt& GetPlAtkCDmg01Cost();

	//UpgradeAtkDmg01
public:
	void UpgradeAtkDmg01();

protected://callback
	void OnGetPlAtk01DmgSuccess(const FGetUsrDataRSlt& result);
	
	void OnUpdatePlAtk01DmgSuccess(const FUpdateRslt& result);

	void OnErrorPlayfabReq(const FFailRslt& errorResult);
};
