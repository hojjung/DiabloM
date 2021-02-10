// My First Hack n Slash


#include "AnimNotify_PlayMobHittenEffect.h"


#include "Animation/AnimNotifies/AnimNotify_PlaySound.h"
#include "Characters/MonsterPawn.h"

void UAnimNotify_PlayMobHittenEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(!MeshComp->GetOwner())
	{
		return;
	}
	
	AMonsterPawn* Mob = Cast<AMonsterPawn>( MeshComp->GetOwner());

	if(!Mob)
	{
		return;
	}

	FMonsterTypeRow* MobType = Mob->GetMonsterDataTable().m_TypeHandle.GetRow<FMonsterTypeRow>("");

	if (MobType->m_HittenSound)
	{
		if (MobType->m_HittenSound->IsLooping())
		{
			PRINTF("SoundISLooping");
			return;
		}

		if (bFollow)
		{
			UGameplayStatics::SpawnSoundAttached(MobType->m_HittenSound, MeshComp, AttachName, FVector(ForceInit), EAttachLocation::SnapToTarget, false, VolumeMultiplier, PitchMultiplier);
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(MeshComp->GetWorld(), MobType->m_HittenSound, MeshComp->GetComponentLocation(), VolumeMultiplier, PitchMultiplier);
		}
	}
}
