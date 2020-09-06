 #pragma once

#include "CoreMinimal.h"

#include "CharCreate.h"
#include "Blueprint/UserWidget.h"
#include "StartMenuCanvas.generated.h"

/**
 * StartMenuMaiNCanvas
 */
UCLASS()
class DIABLOM_API UStartMenuCanvas : public UUserWidget
{
	GENERATED_BODY()
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    UCharCreate* m_CharCreate;
    
public:
    void InitStartMenu();
    
    
};
