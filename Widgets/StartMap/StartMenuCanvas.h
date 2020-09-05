 #pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartMenuCanvas.generated.h"

/**
 * StartMenuMaiNCanvas
 */
UCLASS()
class DIABLOM_API UStartMenuCanvas : public UUserWidget
{
	GENERATED_BODY()
    
public:
    void InitStartMenu();
    
    
};
