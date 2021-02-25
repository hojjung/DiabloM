// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "DiabloM.h"
#include "PlayfabManager.h"
#include "Datas/PlayerUpgradeData.h"
#include "UObject/NoExportTypes.h"
#include "PlayerUpgradeManager.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnUpgradeLevelChanged);

USTRUCT()
struct FUpgradeSpec
{
	GENERATED_BODY()
public:
	int m_nLv;
	const FUpgradeDataRow* m_UpgradeData;
	BigInt m_Value;
	BigInt m_Cost;

	void SetLevel(int v)
	{
		m_nLv = v;
		m_Value = m_UpgradeData->GetValue(m_nLv);
		m_Cost = m_UpgradeData->GetCost(m_nLv);
	}

	int GetMaxLv() const
	{
		return m_UpgradeData->m_nMaxLevel;
	}
};
UCLASS()
class DIABLOM_API UPlayerUpgradeManager : public UObject
{
	GENERATED_BODY()
	
public:
	FOnUpgradeLevelChanged m_OnUpgradeChanged;
	
public:
	UPROPERTY()
	UPlayfabManager* m_PlayfabManager;
	
	FUpgradeSpec m_UpgradeAtkDmg01;
	FUpgradeSpec m_UpgradeAtkCri01;
	FUpgradeSpec m_UpgradeAtkCDmg01;
	FUpgradeSpec m_UpgradeSkill01;
	FUpgradeSpec m_UpgradeSkill02;
	FUpgradeSpec m_UpgradeSkill03;
	
public:
	void SetUpgradeDataFromServer(const FString& stat,const FString& skill);
	//
	void SetPlAtkDmg01();
	void SetPlAtkCri01();
	void SetPlAtkCDmg01();
	void SetPlSkill01();
	void SetPlSkill02();
	void SetPlSkill03();
	
	//UpgradeAtkDmg01
public:
	void UpgradeAtkDmg01();
	void UpgradeAtkCri01();
	void UpgradeAtkCDmg01();
	void UpgradeSkill01();
	void UpgradeSkill02();
	void UpgradeSkill03();
};
