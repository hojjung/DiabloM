// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "SaveLoadManager.h"
#include "Datas/SkillDataTable.h"
#include "GameFramework/SaveGame.h"
#include "SaveSkill.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API USaveSkill : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	ESaveVersion m_SaveVersion;
	UPROPERTY(EditAnywhere)
	int m_nRemainSkillPoint;
	UPROPERTY(EditAnywhere)
	int m_nSpentSkillPoint;
	UPROPERTY(EditAnywhere)
	TArray<FSkillDataSpec> m_ArySkillDataSpec1;
	UPROPERTY(EditAnywhere)
	TArray<FSkillDataSpec> m_ArySkillDataSpec2;
	UPROPERTY(EditAnywhere)
	TArray<FSkillDataSpec> m_ArySkillDataSpec3;
	UPROPERTY(EditAnywhere)
	TArray<FSkillDataSpec> m_ArySkillDataSpec4;
	UPROPERTY(EditAnywhere)
	TArray<FSkillDataSpec> m_ArySkillDataSpec5;
	UPROPERTY(EditAnywhere)
	TArray<FSkillDataSpec> m_ArySkillDataSpec6;

public:
	void SetSaveSkill(ESaveVersion saveVersion,int remainPoint,int spentPoint,TArray<FSkillDataSpec>& skill1,
		TArray<FSkillDataSpec>& skill2,
		TArray<FSkillDataSpec>& skill3,
		TArray<FSkillDataSpec>& skill4,
		TArray<FSkillDataSpec>& skill5,
		TArray<FSkillDataSpec>& skill6);


private:
	void ClearSkillAry(TArray<FSkillDataSpec>& skill);
};
