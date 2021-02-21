// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Datas/CharacterDataTable.h"
#include "UObject/NoExportTypes.h"
#include "PlayerClassManager.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPlayerClassManager : public UObject
{
	GENERATED_BODY()

protected:
	FName m_IDClassData;

public:
	void LoadPlayerClass(FName classData);

	const FPlayerEntityTable*  GetPlayerEntity();
};
