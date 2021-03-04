// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Skill/SkillExecute.h"
#include "UObject/NoExportTypes.h"
#include "SkillCastManager.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API USkillCastManager : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	USkillExecute* m_CurrentCastingSkill;
	
public:
	void StartUseSkill(USkillExecute* skillExe);

	void OnEndCurrentSkill(USkillExecute* skillExe);

	void ForceEndCurrentSkill();
};
