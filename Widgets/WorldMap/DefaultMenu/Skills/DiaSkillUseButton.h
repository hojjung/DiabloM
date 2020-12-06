// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DiaSkillUseButton.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UDiaSkillUseButton : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

};
