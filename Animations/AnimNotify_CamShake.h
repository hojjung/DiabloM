// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Camera/CameraShake.h"

#include "AnimNotify_CamShake.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UAnimNotify_CamShake : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UCameraShake> m_ClassCamShake;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;
};
