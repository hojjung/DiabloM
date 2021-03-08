// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "UserWidget.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UMainCanvas;
/**
 * 
 */
UCLASS()
class DIABLOM_API AMyHUD : public AHUD
{
	GENERATED_BODY()
public:
	AMyHUD();
	
	virtual void ReqeustPopupText(FText str);
};
