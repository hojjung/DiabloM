// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "TechnologyTree.h"
#include "AbilitySystem/Components/DiabloAbilitySystemComp.h"
#include "Datas/DiaTechnologyAsset.h"
#include "Datas/SkillDataTable.h"

#include "PlayerDiabloAbilitySystemComp.generated.h"

class UPlayerStatusBar;
class USaveLoadManager;
class UDiabloCheatManager;
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
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSkillChanged,FSkillDataSpec*,int);
	UPlayerDiabloAbilitySystemComp();

public:
	FOnSkillLevelup m_OnSkillLevelChanged;

	FOnSkillChanged m_OnSkillChanged;

protected:
	UPROPERTY()
	UTechTreeManager* m_TechManager1;
	UPROPERTY()
	UTechTreeManager* m_TechManager2;
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

	TMap<FSkillDataSpec*,FGameplayAbilitySpecHandle> m_EquippedSkill;
	//
	int m_nTalentPoints;

	int m_nTotalTalentPointSpents;
	
	TArray<FTalentDataSpec> m_AryTalents1;

	TArray<FTalentDataSpec> m_AryTalents2;

	TMap<FTalentDataSpec*,FGameplayAbilitySpecHandle> m_EquippedTalent;

public:
	virtual void BeginPlay()override;
	
	int GetSkillPoints();
	
	void CreateTalentSpec(const UTechnologyTree* const technology_tree1,const UTechnologyTree* const technology_tree2);

	void SetLoadedTalent(TArray<FTalentDataSpec> talent1, TArray<FTalentDataSpec> talent2);

	//void SetLoadedSkillTree(const UTechnologyTree* const technology_tree);
	
	void SetLoadedSkillData(TArray<FSkillDataSpec>& skill1,
        TArray<FSkillDataSpec>& skill2,
        TArray<FSkillDataSpec>& skill3,
        TArray<FSkillDataSpec>& skill4,
        TArray<FSkillDataSpec>& skill5,
        TArray<FSkillDataSpec>& skill6);
	void LevelupSkill(FSkillDataSpec* skillSpec);

	void EquipSkill(FSkillDataSpec* skillSpec);

	bool UnequipSkill(FSkillDataSpec* skillSpec);

	bool IsCooldownAvailable(FSkillDataSpec* skillSpec);

	FGameplayAbilitySpec* UseSkill(FSkillDataSpec* skillSpec);

	bool CheckAlreadyEquipped(FSkillDataSpec* skillSpec);
	
	FORCEINLINE const FSkillDataRow* GetSkillDataTableRow() const
	{
		return m_SkillDataTableRow;
	}
	void CreateClassSkillSpecs(const FSkillDataHandle& skillDataHandle);
	
	
	void AllEquipTalentData();
	
	friend UDiaSkillPanel;
	friend UDiabloCheatManager;
	friend USaveLoadManager;
	friend UPlayerStatusBar;
	//
public:

private:
	void SetSkillFromSaveData(TArray<FSkillDataSpec>& my,const TArray<FSkillDataSpec>& loadedData);
	
	void SetTalentFromSaveData(TArray<FTalentDataSpec>& my,const TArray<FTalentDataSpec>& loadedData);

	
};



