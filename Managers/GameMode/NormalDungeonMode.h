// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "DiabloGameMode.h"
#include "GameFramework/GameMode.h"
#include "NormalDungeonMode.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API ANormalDungeonMode : public ADiabloGameMode
{
	GENERATED_BODY()
	
	
public:
	ANormalDungeonMode();
	
	UPROPERTY()
	UPlayfabManager* m_PlManager;
	UPROPERTY()
	UChatManager* m_ChatManager;

public:
	virtual void StartPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
};
