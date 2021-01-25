#include "Joystick.h"
#include "CanvasPanelSlot.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Kismet/KismetInputLibrary.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"

UJoystick::UJoystick(const FObjectInitializer& objInit):Super(objInit)
{
	m_fRadius = 300.f;
	m_fCamRotate=-45.f;
	m_fPickerRadius=64.f;
	m_fDragRadius=200.f;
	m_bIsPressed=false;
	m_bIsDragUse=false;
	m_bIsSuccessDragged=false;
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

	m_SkillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_Picker->SetVisibility(ESlateVisibility::Hidden);

	ClearIcon();
	
}

void UJoystick::SetIndicatorLocation(FVector NewActorLocation)
{
	if(!ASkillIndicator::GetCurrent)
	{
		return;
	}

	ASkillIndicator::GetCurrent->SetRadiusScale(m_fRadius);
	
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	
	if(!NavSys)
	{
		return;
	}
	
	FNavLocation OutLoc;

	APlayerDiabloCharacter* Dia = ADiabloPlayerController::Get->GetPlayerPawn();

	FHitResult Hit;
	
	FQuat QQ = Dia->GetCapsule()->GetComponentRotation().Quaternion();

	FCollisionShape Shape = FCollisionShape();
	//Shape.SetCapsule(Dia->GetCapsule()->Ext);
	
	if(Dia->GetCapsule()->SweepComponent(Hit,NewActorLocation,NewActorLocation,QQ,Shape,false))
	{
		PRINTF("Sweep!");
	}
	
	if(!NavSys->ProjectPointToNavigation(NewActorLocation,OutLoc))
	{
		//ASkillIndicator::GetCurrent->SetActorLocation(OutLoc.Location);			
	}
	else
	{
		ASkillIndicator::GetCurrent->SetActorLocation(NewActorLocation);//Danger?
	}
}

void UJoystick::UpdateTouchInput(FVector2D input)
{
	PRINTF("Unput:%s",*input.ToString());
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

		m_bIsSuccessDragged=true;
	}
	else
	{
		m_SlotPicker->SetPosition(ActualCursorPos);

		m_bIsSuccessDragged=false;
	}

	FVector2D Diff = MaxDrag - m_StartPickerPos;
	
	Diff /= m_fDragRadius;

	FVector NormalDiff3 = FVector(Diff.X,Diff.Y,0.f);

	NormalDiff3 = NormalDiff3.RotateAngleAxis(m_fCamRotate,FVector::UpVector);
	//
	FVector CalcLoc = NormalDiff3 * m_fMaximumRadius;
	
	FVector PlayerLocation =  ADiabloPlayerController::Get->GetPlayerPawn()->GetActorLocation();

	FVector NewActorLocation = PlayerLocation - CalcLoc;

	SetIndicatorLocation(NewActorLocation);
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

void UJoystick::SetIcon(UTexture2D* textureWant)
{
	m_SkillIcon->SetBrushFromTexture(textureWant);
}

void UJoystick::ClearIcon()
{
	SetIcon(nullptr);
}

void UJoystick::SetRadius(float radius)
{
	m_fRadius = radius;
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

	m_ActualDragger->SetRenderScale(FVector2D(2.5f,2.5f));

	if(ASkillIndicator::GetCurrent)
	{
		ASkillIndicator::GetCurrent->SetActorHiddenInGame(false);
	}
	m_Picker->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}


void UJoystick::EndJoystickDrag()
{
	if(!m_bIsPressed)
	{
		return;
	}
	
	m_bIsPressed = false;

	m_ActualDragger->SetRenderScale(FVector2D(1.0f,1.0f));

	m_SlotActualDragger->SetPosition(m_StartDraggerPos);
	
	m_SlotPicker->SetPosition(m_StartPickerPos);
	
	if(ASkillIndicator::GetCurrent)
	{
		ASkillIndicator::GetCurrent->SetActorHiddenInGame(true);
		
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		
		if(NavSys)
		{
			FNavLocation OutLoc;

			m_bIsSuccessDragged = NavSys->ProjectPointToNavigation(ASkillIndicator::GetCurrent->GetActorLocation(),OutLoc);

			if(!m_bIsSuccessDragged)
			{
				PRINTF("DragFail");
			}
		}
	}

	m_Picker->SetVisibility(ESlateVisibility::Hidden);

	if(m_bIsSuccessDragged)
	{
		m_OnDropEnd.Broadcast();
	}

	SetRadius(100.f);
}

