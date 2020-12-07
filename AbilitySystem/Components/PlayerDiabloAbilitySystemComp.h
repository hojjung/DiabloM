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
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillLevelup,FSkillDataSpec*);
	UPlayerDiabloAbilitySystemComp();

public:
	virtual void BeginPlay()override;
	
	void CreateClassSkillSpecs(const FSkillDataHandle& skillDataHandle);
	
	int GetSkillPoints();

	FOnSkillLevelup m_OnSkillLevelChanged;
protected:
	UPROPERTY()
	APlayerDiabloCharacter* m_PlayerPawn;
	
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
