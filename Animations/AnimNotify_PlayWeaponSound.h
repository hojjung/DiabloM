// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Item/EquipmentSystem.h"

#include "AnimNotify_PlayWeaponSound.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UAnimNotify_PlayWeaponSound : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(ExposeOnSpawn = true))
	ESlotsEquipAry SoundSourceSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(ExposeOnSpawn = true))
	float VolumeMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(ExposeOnSpawn = true))
	float PitchMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	uint32 bFollow:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(EditCondition="bFollow", ExposeOnSpawn = true))
	FName AttachName;
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;	
};
