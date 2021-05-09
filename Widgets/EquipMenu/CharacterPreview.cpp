#include "CharacterPreview.h"

#include "Managers/DiabloGameInstance.h"
#include "Managers/GameMode/DiabloGameMode.h"


FReply UCharacterPreview::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re =  Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	UDiabloGameInstance::Get->m_NormalDgManager->OnTouchStart();
	
	return FReply::Handled();
}

FReply UCharacterPreview::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
	
	float DeltaX = InGestureEvent.GetCursorDelta().X;

	if(DeltaX!=0.f)
	{
		UDiabloGameInstance::Get->m_NormalDgManager->RotatePawn(DeltaX);
	}

	return FReply::Handled();
}

FReply UCharacterPreview::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
	
	UDiabloGameInstance::Get->m_NormalDgManager->OnTouchEnd();

	return FReply::Handled();
}

