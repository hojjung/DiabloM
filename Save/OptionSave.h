// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OptionSave.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UOptionSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool m_bOptionFPSLimit = false;
	UPROPERTY(EditAnywhere)
	float m_fOptionVolumeSFX = 1.0f;
	UPROPERTY(EditAnywhere)
	float m_fOptionVolumeBGM= 1.0f;
};
