#include "CharacterPreview.h"

#include "Managers/DiabloGameInstance.h"
#include "Managers/GameMode/DiabloGameMode.h"

void UCharacterPreview::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_PlayerVisual = UDiabloGameInstance::Get->m_NormalDgManager->m_VisualActor;

	m_bTouched = false;

	m_InitVisualRot = m_PlayerVisual->GetSkMesh()->GetComponentRotation();
}



void UCharacterPreview::RotatePawn(float x)
{
	x = -1.f* x;
	FRotator Rot(0.f);
	Rot.Yaw=x*2.f;
	m_PlayerVisual->GetSkMesh()->AddLocalRotation(Rot);
}

FReply UCharacterPreview::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re =  Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	m_bTouched = true;
	
	return FReply::Handled();
}

FReply UCharacterPreview::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
	
	float DeltaX = InGestureEvent.GetCursorDelta().X;

	if(DeltaX!=0.f)
	{
		RotatePawn(DeltaX);
	}

	return FReply::Handled();
}

FReply UCharacterPreview::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
	
	m_bTouched = false;

	return FReply::Handled();
}

void UCharacterPreview::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(m_bTouched)
	{
		return;
	}

	FRotator NewRot = m_PlayerVisual->GetSkMesh()->GetComponentRotation();
	
	NewRot.Yaw = UKismetMathLibrary::RInterpTo(NewRot, m_InitVisualRot,InDeltaTime, 5.5f).Yaw;
	
	m_PlayerVisual->GetSkMesh()->SetWorldRotation(NewRot);

	FRotator NewRot2 = m_PlayerVisual->GetSkMesh()->GetComponentRotation();
}



