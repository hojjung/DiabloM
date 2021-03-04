// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Datas/PlayerUpgradeData.h"
#include "Managers/SkillCastManager.h"
#include "UObject/NoExportTypes.h"
#include "SkillExecute.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API USkillExecute : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillEnd,USkillExecute*);

	FOnSkillEnd m_OnSkillEnd;

protected:
	FSkillSpec* m_Spec;
	
public:
	virtual void Init(FSkillSpec* spec);
	
	virtual void StartExecuteSkill();
	
	virtual void CastStart(USkillCastManager* castM){};

	virtual void CastEnd(USkillCastManager* castM){m_OnSkillEnd.Broadcast(this); };

	
};
