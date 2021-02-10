// My First Hack n Slash


#include "AnimNotify_PlayWeaponSound.h"

#include "Characters/DiabloPlayerController.h"

void UAnimNotify_PlayWeaponSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(!MeshComp->GetOwner())
	{
		return;
	}
	
	APlayerDiabloCharacter* Player = Cast<APlayerDiabloCharacter>( MeshComp->GetOwner());

	if(!Player)
	{
		return;
	}

	const FItemInstance& ItemInst = ADiabloPlayerController::Get->GetEquipment()->GetItem(SoundSourceSlot);

	const FItemType* ItemTypeCurrent = ItemInst.m_ItemData->m_ItemType.GetRow<FItemType>("");

	if(!ItemTypeCurrent)
	{
		PRINTF("ItemTypeNull");
		return;
	}
	
	USoundBase* Sound = ItemTypeCurrent->m_UseSound;

	if (Sound)
	{
		if (Sound->IsLooping())
		{
			PRINTF("SoundISLooping");
			return;
		}

		if (bFollow)
		{
			UGameplayStatics::SpawnSoundAttached(Sound, MeshComp, AttachName, FVector(ForceInit), EAttachLocation::SnapToTarget, false, VolumeMultiplier, PitchMultiplier);
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(MeshComp->GetWorld(), Sound, MeshComp->GetComponentLocation(), VolumeMultiplier, PitchMultiplier);
		}
	}
}
