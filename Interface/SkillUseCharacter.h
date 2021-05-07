// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Engine/EngineTypes.h"
#include "UObject/Interface.h"
#include "SkillUseCharacter.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USkillUseCharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DIABLOM_API ISkillUseCharacter
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float PlaySkillMontageSection(FName& nameID, int nSectionIndex, float& currentCD, float maxCD)=0;
	
	virtual float GetRage()=0;

	virtual void TriggerSkill(const FName& name,TArray<FHitResult>* aryHits=nullptr)=0;
};
