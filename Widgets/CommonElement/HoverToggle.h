// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "CheckBox.h"
#include "Blueprint/UserWidget.h"
#include "HoverToggle.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UHoverToggle : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCheckBox* m_Toggle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Style", meta=( DisplayName="Style" ))
	FCheckBoxStyle m_WidgetStyle;
	
	virtual void NativePreConstruct() override;

	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
