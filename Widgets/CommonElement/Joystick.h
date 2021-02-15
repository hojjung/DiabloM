// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "CanvasPanel.h"
#include "Image.h"
#include "Blueprint/UserWidget.h"
#include "Objs/Actor/SkillIndicator.h"

#include "Joystick.generated.h"



UCLASS()
class DIABLOM_API UJoystick : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UJoystick(const FObjectInitializer& objInit);

	DECLARE_MULTICAST_DELEGATE(FOnDrop);

	FOnDrop m_OnDropEnd;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_Canvas;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UImage* m_SkillIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_Background;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_Picker;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ActualDragger;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fCamRotate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fPickerRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fDragRadius;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<ASkillIndicator> m_ClassIndicator;

protected:
	UPROPERTY()
	UCanvasPanelSlot* m_SlotBackground;
	
	UPROPERTY()
	UCanvasPanelSlot* m_SlotActualDragger;
	
	UPROPERTY()
	UCanvasPanelSlot* m_SlotPicker;
	
	UPROPERTY()
	FVector2D m_StartPickerPos;

	UPROPERTY()
	FVector2D m_StartDraggerPos;
	
	UPROPERTY()
	FVector2D m_CurrentCursorPos;

	UPROPERTY()
	FVector2D m_BGHalfSize;
	
	UPROPERTY()
	float m_fPickerRadiusSqr;

	UPROPERTY()
	float m_fDragRadiusSqr;
	
	UPROPERTY()
	bool m_bIsPressed;
	UPROPERTY()
	bool m_bIsDragUse;
	UPROPERTY()
	float m_fWorldActorRange = 750.f;
	UPROPERTY()
	bool m_bIsSuccessDragged;//너무 짧게 드래그 되는것 방지용

	float m_fRadius;

public:
	void UpdateTouchInput(FVector2D input);
	
	void StartJoystickDrag();
	
	void EndJoystickDrag();

	virtual void NativeOnInitialized() override;
	void SetIndicatorLocation(FVector NewActorLocation);

	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	void SetUseDrag(bool useDrag);

	void SetIcon(UTexture2D* textureWant);

	void ClearIcon();

	void SetRadius(float radius,float range);
};
