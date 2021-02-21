#pragma once
#include "DiabloM.h"
#include "PlayerVisual.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCreateController.generated.h"

UCLASS()
class DIABLOM_API APlayerCreateController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerCreateController();
	
	
public:
	virtual void BeginPlay() override;
	
	void InitWidget();	
	
	APlayerVisual* GetPlayerVisual();

protected:
	virtual void SetupInputComponent() override;

	void OnDeviceBackKey();

	void ExitGame();
};
