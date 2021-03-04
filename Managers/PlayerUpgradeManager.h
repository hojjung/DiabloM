// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "DiabloM.h"
#include "PlayfabManager.h"
#include "Datas/PlayerUpgradeData.h"
#include "UObject/NoExportTypes.h"
#include "PlayerUpgradeManager.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnUpgradeLevelChanged);


UCLASS()
class DIABLOM_API UPlayerUpgradeManager : public UObject
{
	GENERATED_BODY()
	
public:
	UPlayerUpgradeManager();
	
	FOnUpgradeLevelChanged m_OnUpgradeChanged;

	static UDataTable* StatUpgradeTable;

	static UDataTable* SkillUpgradeTable;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSkillChanged,int,FSkillSpec*);

	FOnSkillChanged m_OnSkillChanged;
	
public:
	UPROPERTY()
	UPlayfabManager* m_PlayfabManager;
	
	FUpgradeSpec m_UpgradeAtkDmg01;
	FUpgradeSpec m_UpgradeAtkCri01;
	FUpgradeSpec m_UpgradeAtkCDmg01;

	TArray<FSkillSpec> m_AryUpgradeSkill;
	
	FSkillSpec* m_AryEquippedSkillSpec[4];
	
public:
	void SetUpgradeDataFromServer(const FString& stat,const FString& skill);
	//
	//UpgradeAtkDmg01
public:
	void UpgradeAtkDmg01();
	void UpgradeAtkCri01();
	void UpgradeAtkCDmg01();
	void UpgradeSkill01();
	void UpgradeSkill02();
	void UpgradeSkill03();
	void EquipSkill(int index,  FSkillSpec* skill_spec);
	void UnequipSkill(int index,  FSkillSpec* skill_spec);
};
