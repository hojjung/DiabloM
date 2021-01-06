// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "CanvasPanel.h"
#include "Image.h"
#include "Blueprint/UserWidget.h"
#include "Joystick.generated.h"



UCLASS()
class DIABLOM_API UJoystick : public UUserWidget
{
	GENERATED_BODY()
	
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDrag,const FVector2D&);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDragBP,const FVector2D&,coord01);
	
public:
	UJoystick(const FObjectInitializer& objInit);
	
	FOnDrag m_OnDrag;

	UPROPERTY(BlueprintAssignable)
	FOnDragBP m_OnDragBP;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_Canvas;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_Background;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_Picker;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ActualDragger;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fPickerRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fDragRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> m_ClassTest;

protected:
	UPROPERTY()
	UCanvasPanelSlot* m_SlotActualDragger;
	UPROPERTY()
	UCanvasPanelSlot* m_SlotPicker;
	UPROPERTY()
	AActor* m_Actor;

	UPROPERTY()
	bool m_bIsPressed;

	UPROPERTY()
	FVector2D m_StartPickerPos;
	
	UPROPERTY()
	float m_fPickerRadiusSqr;

	UPROPERTY()
	float m_fDragRadiusSqr;

	float m_fMaximumTest = 750.f;

	UPROPERTY()
	FVector2D m_CurrentCursorPos;

public:
	void UpdateTouchInput(FVector2D input);

	virtual void NativeOnInitialized() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
