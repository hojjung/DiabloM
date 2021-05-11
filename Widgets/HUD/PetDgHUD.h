// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "PetDgCanvas.h"
#include "Widgets/HUD/MyHUD.h"
#include "PetDgHUD.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API APetDgHUD : public AMyHUD
{
	GENERATED_BODY()

public:
	APetDgHUD();

	UPROPERTY()
	TSubclassOf<UPetDgCanvas> m_ClassWidget;
	UPROPERTY()
	UPetDgCanvas* m_Canvas;

	virtual void BeginPlay() override;

	virtual void ReqeustPopupText(FText str) override;	
};
