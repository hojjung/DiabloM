// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Datas/PlayerUpgradeData.h"
#include "UObject/NoExportTypes.h"
#include "AutoSkillUse.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UAutoSkillUse : public UObject
{
	GENERATED_BODY()

public:
	FSkillSpec* GetUsableSkill(ISkillUseCharacter* owner,TArray<FSkillSpec*>& skillSpecs,float gainRagePerSec);
};
