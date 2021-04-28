// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FloatingTextWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UFloatingTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UFloatingTextWidgetComponent();
	
	void SetFloatingText(const FText& ttWant);
};
