#include "Joystick.h"
#include "CanvasPanelSlot.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Kismet/KismetInputLibrary.h"

UJoystick::UJoystick(const FObjectInitializer& objInit):Super(objInit)
{
	m_fCamRotate=-45.f;
	m_fPickerRadius=64.f;
	m_fDragRadius=200.f;
	m_bIsPressed=false;
	m_bIsDragUse=true;//TEST 0119
}

void UJoystick::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_SlotBackground = Cast<UCanvasPanelSlot>(m_Background->Slot);
	
	m_SlotActualDragger = Cast<UCanvasPanelSlot>(m_ActualDragger->Slot);

	m_StartDraggerPos = m_SlotActualDragger->GetPosition();
	
	m_SlotPicker = Cast<UCanvasPanelSlot>(m_Picker->Slot);

	m_StartPickerPos = m_SlotPicker->GetPosition();//
	
	m_fPickerRadiusSqr = m_fPickerRadius*m_fPickerRadius;

	m_fDragRadiusSqr = m_fDragRadius*m_fDragRadius;
}

void UJoystick::UpdateTouchInput(FVector2D input)
{
	m_BGHalfSize = GetCachedGeometry().GetDrawSize() / 2.f;
	
	m_BGHalfSize /=GetCachedGeometry().Scale;
	
	FVector2D ActualCursorPos = input - m_BGHalfSize;
	
	m_SlotActualDragger->SetPosition(ActualCursorPos);//-

	FVector2D PickerDelta = ActualCursorPos - m_StartPickerPos;

	float PickerDeltaSqr = PickerDelta.SizeSquared();

	FVector2D PickerNormal = PickerDelta.GetSafeNormal();

	FVector2D MaxDrag;
	
	if(PickerDeltaSqr > m_fDragRadiusSqr)
	{
		MaxDrag = m_StartPickerPos + PickerNormal * m_fDragRadius;
	}
	else
	{
		MaxDrag = ActualCursorPos;
	}

	if(PickerDeltaSqr > m_fPickerRadiusSqr)
	{
		FVector2D LimitedPos = m_StartPickerPos + PickerNormal*m_fPickerRadius;
		
		m_SlotPicker->SetPosition(LimitedPos);
	}
	else
	{
		m_SlotPicker->SetPosition(ActualCursorPos);			
	}

	FVector2D Diff = MaxDrag - m_StartPickerPos;
	
	Diff /= m_fDragRadius;

	FVector NormalDiff3 = FVector(Diff.X,Diff.Y,0.f);

	NormalDiff3 = NormalDiff3.RotateAngleAxis(m_fCamRotate,FVector::UpVector);
	//
	FVector CalcLoc = NormalDiff3 * m_fMaximumRadius;
	
	FVector PlayerLocation =  ADiabloPlayerController::Get->GetPlayerPawn()->GetActorLocation();

	FVector NewActorLocation = PlayerLocation - CalcLoc;

	if(ASkillIndicator::GetCurrent)
	{
		ASkillIndicator::GetCurrent->SetActorLocation(NewActorLocation);

		m_OnDrag.Broadcast(NewActorLocation);
	
		m_OnDragBP.Broadcast(NewActorLocation);
	}
}

FReply UJoystick::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Result = Super::NativeOnMouseButtonDown(InGeometry, InGestureEvent);

	StartJoystickDrag();	

	return Result;
}

FReply UJoystick::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply ASD =Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	if(!m_bIsPressed)
	{
		return ASD;
	}
	
	m_CurrentCursorPos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

	UpdateTouchInput(m_CurrentCursorPos);

	return  ASD;
}

void UJoystick::NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{
	Super::NativeOnMouseCaptureLost(CaptureLostEvent);

	PRINTF("MouseCapture");
}

void UJoystick::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
	PRINTF("FocusLost");
}

FReply UJoystick::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply ASD =Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
	
	EndJoystickDrag();

	return ASD;
}

void UJoystick::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	
	EndJoystickDrag();
}

void UJoystick::SetUseDrag(bool useDrag)
{
	m_bIsDragUse = useDrag;
}

void UJoystick::StartJoystickDrag()
{
	if(!m_bIsDragUse)
	{
		return;
	}
	
	if(m_bIsPressed)
	{
		return;
	}
	
	m_bIsPressed = true;

	m_OnPressChanged.Broadcast(m_bIsPressed);

	m_ActualDragger->SetRenderScale(FVector2D(2.5f,2.5f));

	if(ASkillIndicator::GetCurrent)
	{
		ASkillIndicator::GetCurrent->SetActorHiddenInGame(false);
	}
}


void UJoystick::EndJoystickDrag()
{
	if(!m_bIsPressed)
	{
		return;
	}
	
	m_bIsPressed = false;

	m_OnPressChanged.Broadcast(m_bIsPressed);

	m_ActualDragger->SetRenderScale(FVector2D(1.0f,1.0f));

	m_SlotActualDragger->SetPosition(m_StartDraggerPos);
	
	m_SlotPicker->SetPosition(m_StartPickerPos);
	
	if(ASkillIndicator::GetCurrent)
	{
		ASkillIndicator::GetCurrent->SetActorHiddenInGame(true);
	}
}