// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayMobHittenEffect.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UAnimNotify_PlayMobHittenEffect : public UAnimNotify
{
	GENERATED_BODY()
public:
	// Volume Multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(ExposeOnSpawn = true))
	float VolumeMultiplier;

	// Pitch Multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(ExposeOnSpawn = true))
	float PitchMultiplier;

	// If this sound should follow its owner
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	uint32 bFollow:1;

	// Socket or bone name to attach sound to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(EditCondition="bFollow", ExposeOnSpawn = true))
	FName AttachName;
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;
};
