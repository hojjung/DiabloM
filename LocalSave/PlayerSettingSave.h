// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSettingSave.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPlayerSettingSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int m_nSkillEquip1 = -1;//-1 none
	UPROPERTY(EditAnywhere)
	int m_nSkillEquip2 = -1;
	UPROPERTY(EditAnywhere)
	int m_nSkillEquip3 = -1;
	UPROPERTY(EditAnywhere)
	int m_nSkillEquip4 = -1;


public:
	void SaveSkillEquipSetting(int sk1,int sk2,int sk3,int sk4); 
};
