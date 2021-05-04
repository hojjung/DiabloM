// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "UObject/NoExportTypes.h"
#include "MagicStoneDgManager.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UMagicStoneDgManager : public UObject
{
	GENERATED_BODY()
	//드래곤 한마리 소환

public:
	UMagicStoneDgManager();

	FDelegateHandle m_LevelLoadHandle;

	int m_nDgLevel;
	
	void RequestMoveMagicStoneDg(int dgLevel);

	void OnLevelLoaded(UWorld*);
	
	void Tick(float delta_seconds);
};
