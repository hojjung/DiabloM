#pragma once

#include "CoreMinimal.h"
#include "Widgets/HUD/MyHUD.h"
#include "InitLevelHUD.generated.h"

class UGameStartCanvas;
/**
 * 
 */
UCLASS()
class DIABLOM_API AInitLevelHUD : public AMyHUD
{
	GENERATED_BODY()
public:
	AInitLevelHUD();

	UPROPERTY()
	TSubclassOf<UGameStartCanvas> m_ClassWidget;
	UPROPERTY()
	UGameStartCanvas* m_Canvas;

	virtual void BeginPlay() override;

	virtual void ReqeustPopupText(FText str) override;
};
