#include "Joystick.h"
#include "CanvasPanelSlot.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Kismet/KismetInputLibrary.h"

UJoystick::UJoystick(const FObjectInitializer& objInit):Super(objInit)
{
	m_fPickerRadius=64.f;
	m_fDragRadius=250.f;
	m_bIsPressed=false;
}

void UJoystick::NativeOnInitialized()
{
	m_SlotActualDragger = Cast<UCanvasPanelSlot>(m_ActualDragger->Slot);
	
	m_SlotPicker = Cast<UCanvasPanelSlot>(m_Picker->Slot);

	m_StartPickerPos = m_SlotPicker->GetPosition();

	m_fPickerRadiusSqr = m_fPickerRadius*m_fPickerRadius;

	m_fDragRadiusSqr = m_fDragRadius*m_fDragRadius;
}

void UJoystick::UpdateTouchInput(FVector2D input)
{
	m_SlotActualDragger->SetPosition(input);
	
	FVector2D PickerDelta = input - m_StartPickerPos;

	float PickerDeltaSqr = PickerDelta.SizeSquared();

	FVector2D PickerNormal = PickerDelta.GetSafeNormal();

	FVector2D MaxDrag;
	
	if(PickerDeltaSqr > m_fDragRadiusSqr)
	{
		MaxDrag = m_StartPickerPos + PickerNormal*m_fDragRadius;
	}
	else
	{
		MaxDrag = input;
	}

	if(PickerDeltaSqr > m_fPickerRadiusSqr)
	{
		FVector2D LimitedPos = m_StartPickerPos + PickerNormal*m_fPickerRadius;
		
		m_SlotPicker->SetPosition(LimitedPos);
	}
	else
	{
		m_SlotPicker->SetPosition(input);			
	}

	FVector2D Diff = MaxDrag - m_StartPickerPos;

	FVector2D NormalDiff = FVector2D(Diff.X / m_fDragRadius , Diff.Y / m_fDragRadius);

	PRINTF("DIFF:%s",*NormalDiff.ToString());

	//m_OnDrag.Broadcast(NormalDiff);
	
	//m_OnDragBP.Broadcast(NormalDiff);
	
	FVector NormalDiff3 = FVector(NormalDiff.X,NormalDiff.Y,0.f);

	NormalDiff3 = NormalDiff3.RotateAngleAxis(-45.f,FVector::UpVector);
	//
	FVector CalcLoc = NormalDiff3 * m_fMaximumTest;
	
	FVector PlayerLocation =  ADiabloPlayerController::Get->GetPlayerPawn()->GetActorLocation();

	FVector NewActorLocation = PlayerLocation - CalcLoc;

	m_Actor->SetActorLocation(NewActorLocation);

	float Dist = FVector::Dist(PlayerLocation,NewActorLocation);

	PRINTF("Dist:%f",Dist);
}

FReply UJoystick::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Result = Super::NativeOnMouseButtonDown(InGeometry, InGestureEvent);
	
	m_bIsPressed = true;

	FVector Loc = ADiabloPlayerController::Get->GetPlayerPawn()->GetActorLocation();
	
	FRotator Rot(0.f,0.f,0.f);
	
	FActorSpawnParameters Param;
	
	Param.bNoFail=true;
	
	m_Actor = ADiabloPlayerController::Get->GetWorld()->SpawnActor<AActor>(m_ClassTest,Loc,Rot,Param);	

	return Result;
}

FReply UJoystick::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

FReply UJoystick::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply DD =Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	
	m_bIsPressed = false;

	UpdateTouchInput(m_StartPickerPos);

	if(m_Actor)
	{
		m_Actor->Destroy();
	}
	
	return DD;
}

