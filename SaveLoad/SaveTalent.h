#pragma once
#include "DiabloM.h"
#include "SaveLoadManager.h"
#include "Datas/DiaTechnologyAsset.h"
#include "GameFramework/SaveGame.h"
#include "SaveTalent.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API USaveTalent : public USaveGame
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
	TArray<FTalentDataSpec> m_AryTalentData1;
	UPROPERTY(EditAnywhere)
	TArray<FTalentDataSpec> m_AryTalentData2;
	
public:
	void SetSaveTalent(ESaveVersion saveVersion,int remainPoint,int spentPoint,TArray<FTalentDataSpec>& talent1,TArray<FTalentDataSpec>& talent2);
};
