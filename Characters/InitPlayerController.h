// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/SetNickname.h"

#include "InitPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API AInitPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AInitPlayerController();
	
	virtual void Tick(float DeltaSeconds) override;

	bool m_bLock = false;
	virtual void SetupInputComponent() override;
	UFUNCTION()
void OnDeviceBackKey();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
