// My First Hack n Slash

#pragma once

#include "DiabloM.h"

#include "Widgets/HUD/MyHUD.h"
#include "MagicDgHUD.generated.h"

class UMagicDgCanvas;
/**
 * 
 */
UCLASS()
class DIABLOM_API AMagicDgHUD : public AMyHUD
{
	GENERATED_BODY()
public:
	AMagicDgHUD();
	
	UPROPERTY()
	TSubclassOf<UMagicDgCanvas> m_ClassWidget;

	UPROPERTY()
	UMagicDgCanvas* m_Canvas;
	
	virtual void BeginPlay() override;

	virtual void ReqeustPopupText(FText str) override;

};
