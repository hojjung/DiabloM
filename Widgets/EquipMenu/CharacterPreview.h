#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Characters/StartMap/PlayerVisual.h"

#include "CharacterPreview.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UCharacterPreview : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_PreviewImage;
	UPROPERTY()
	APlayerVisual* m_PlayerVisual;

	bool m_bTouched;

	FRotator m_InitVisualRot;
protected:
	void RotatePawn(float x);
	
	
public:
	virtual void NativeOnInitialized() override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
