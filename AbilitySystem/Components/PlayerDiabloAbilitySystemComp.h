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
	static UPlayerDiabloAbilitySystemComp*Get;

	DECLARE_MULTICAST_DELEGATE(FOnTalentChanged);
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillLevelup,FSkillDataSpec*);
	
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSkillChanged,FSkillDataSpec*,int);
	
	UPlayerDiabloAbilitySystemComp();

public:
	FOnSkillLevelup m_OnSkillLevelChanged;

	FOnSkillChanged m_OnSkillChanged;

	FOnTalentChanged m_OnTalentChanged;

protected:
	FName m_ClassName;
	UPROPERTY()
	UTechnologyTree* m_TechTree;
	UPROPERTY()
	UTechTreeManager* m_TechManager;
	UPROPERTY()
	APlayerDiabloCharacter* m_PlayerPawn;
	
	const FSkillDataRow* m_SkillDataTableRow;
	
	TArray<FSkillDataSpec> m_AryBaseSkill;

	TArray<FSkillDataSpec> m_AryPowerSkill;

	TArray<FSkillDataSpec> m_AryDefensvieSkill;

	TArray<FSkillDataSpec> m_ArySpecialSkill;

	TArray<FSkillDataSpec> m_AryUltimateSkill;

	int m_nSkillPoints;

	int m_nTotalSkillPointSpents;

	TMap<FSkillDataSpec*,FGameplayAbilitySpecHandle> m_EquippedSkill;
	//

	UPROPERTY()
	TMap<UDiaTechnologyAsset*,FGameplayAbilitySpecHandle> m_EquippedTalent;

public:
	virtual void BeginPlay()override;
	
	int GetSkillPoints();
	
	void SetLoadedSkillData(TArray<FSkillDataSpec>& skill1,
        TArray<FSkillDataSpec>& skill2,
        TArray<FSkillDataSpec>& skill3,
        TArray<FSkillDataSpec>& skill4,
        TArray<FSkillDataSpec>& skill5);
	bool LevelupSkill(FSkillDataSpec* skillSpec);

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
	
	void ResetTech();


	friend UDiaSkillPanel;
	friend UDiabloCheatManager;
	friend USaveLoadManager;
	friend UPlayerStatusBar;
	//
public:
	int GetTalentPoint();

	int GetTotalTalentPoint();

	bool IsTalentUnlock(UDiaTechnologyAsset* tech);

	bool LevelupTalent(UDiaTechnologyAsset* talentTech);

	void UnlockTalent(UDiaTechnologyAsset* tech);

	UTechnologyTree* GetCurrentTalentTree();

	void AssignTechTreeWidget(UTechnologyTree* tree,UTechTreeWidget* widget);

	void SetLoadedTalentTree(FName className,UTechnologySaveGame* saveGame);

	FName GetCharacterClass();

	UTechTreeManager* GetTechtreeManager()
	{
		return m_TechManager;
	}
	UFUNCTION()
	void OnTalentLevelChanged(UTechnologyAsset* tech, int level);
	//UTTTechNode*, TechNode, bool, IsUnlocked
private:
	void SetSkillFromSaveData(TArray<FSkillDataSpec>& my,const TArray<FSkillDataSpec>& loadedData);
	
};



