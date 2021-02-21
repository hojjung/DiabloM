#include "AnimNotify_CamShake.h"

#include "Characters/DiabloPlayerController.h"

void UAnimNotify_CamShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	if(!m_ClassCamShake)
	{
		return;
	}

	UWorld* const World = GEngine->GetWorldFromContextObjectChecked(MeshComp);

	if(World->WorldType == EWorldType::EditorPreview)
	{
		return;
	}
     
	// if(World->WorldType != EWorldType::Game)
	// {
	// 	return;
	// }

	
//	ADiabloPlayerController::Get->ClientPlayCameraShake(m_ClassCamShake);	
}
