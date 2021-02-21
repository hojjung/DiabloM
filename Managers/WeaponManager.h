// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponManager.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UWeaponManager : public UObject
{
	GENERATED_BODY()

public:
	FName m_LoadedPlayerWeaponID;
	
};
