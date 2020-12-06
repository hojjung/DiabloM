// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Components/DiabloAbilitySystemComp.h"
#include "Datas/SkillDataTable.h"

#include "PlayerDiabloAbilitySystemComp.generated.h"

class UDiaSkillPanel;
/**
 * 
 */
UCLASS()
class DIABLOM_API UPlayerDiabloAbilitySystemComp : public UDiabloAbilitySystemComp
{
	GENERATED_BODY()

public:
	UPlayerDiabloAbilitySystemComp();

public:
	void CreateClassSkillSpecs(const FSkillDataHandle& skillDataHandle);
	
	int GetSkillPoints();


protected:
	const FSkillDataRow* m_SkillDataTableRow;
	
	TArray<FSkillDataSpec> m_AryBaseSkill;

	TArray<FSkillDataSpec> m_AryPowerSkill;

	TArray<FSkillDataSpec> m_AryDefensvieSkill;

	TArray<FSkillDataSpec> m_ArySpecialSkill;

	TArray<FSkillDataSpec> m_AryMasterySkill;

	TArray<FSkillDataSpec> m_AryUltimateSkill;

	int m_nSkillPoints;

	int m_nTotalSkillPointSpents;

public:
	void LevelupSkill(FSkillDataSpec* skillSpec);
	
	FORCEINLINE const FSkillDataRow* GetSkillDataTableRow() const
	{
		return m_SkillDataTableRow;
	}

	friend UDiaSkillPanel;
};
