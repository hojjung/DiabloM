// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "PVPCanvas.h"
#include "Widgets/HUD/MyHUD.h"
#include "PVP_HUD.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API APVP_HUD : public AMyHUD
{
	GENERATED_BODY()
	
public:
	APVP_HUD();

	UPROPERTY()
	TSubclassOf<UPVPCanvas> m_ClassWidget;
	UPROPERTY()
	UPVPCanvas* m_Canvas;

	virtual void BeginPlay() override;

	virtual void ReqeustPopupText(FText str) override;
};
