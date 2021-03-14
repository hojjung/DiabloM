// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "DiabloM.h"
#include "PlayfabManager.h"
#include "Datas/PlayerUpgradeData.h"
#include "UObject/NoExportTypes.h"
#include "PlayerUpgradeManager.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnUpgradeLevelChanged);

UENUM()
enum class EAttackType : uint8
{
	BaseAttack,
    Critical,
	CriticalDmg,
    SuperCritical,
	SuperCriticalDmg,
    MagicBomb,
	MagicBombDmg,
    SuperMagicBomb,
	SuperMagicBombDmg,
	Length
};

UENUM()
enum class ESkillType : uint8
{
	MiniSlash,
	MagicBlade,
	WhirlWind,
	DeathBlow,
	WindBlade,
	Length
};

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

	//FOnSkillChanged m_OnSkillTick;

	FOnSkillChanged m_OnSkillUse;

protected:
	float m_fCastTime;
	
	FSkillSpec* m_CurrentCastingSkill;
	
protected:
	UPROPERTY()
	UPlayfabManager* m_PlayfabManager;

	TArray<FUpgradeSpec> m_AryBaseAtkUpgrade;

	TArray<FSkillSpec> m_AryUpgradeSkill;
	
	TArray<FSkillSpec*> m_AryEquippedSkillSpec;
	
public:
	void SetUpgradeDataFromServer(const FString& stat,const FString& skill);
	//
	//UpgradeAtkDmg01
public:
	void UpgradeAtk(EAttackType type);
	
	void UpgradeSkill(ESkillType type);

	void EquipSkill(int index,  FSkillSpec* skill_spec);
	
	void UnequipSkill(int index);

	bool UseSkill(int index);

	void Tick(float deltaTime);

	bool IsSkillCasting();

public:
	FORCEINLINE FUpgradeSpec& GetAtkUp(EAttackType type)
	{
		return m_AryBaseAtkUpgrade[(int)type];
	}

	FORCEINLINE FSkillSpec& GetSkillUp(ESkillType type)
	{
		return m_AryUpgradeSkill[(int)type];
	}

	FORCEINLINE TArray<FSkillSpec*>& GetAryEquippedSkill()
	{
		return m_AryEquippedSkillSpec;
	}

	static BigInt MultiplePercent(BigInt a,BigInt b,int aPrecisionCount =1,int bPrecisionCount =1);
};
