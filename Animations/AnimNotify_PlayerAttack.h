// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayerAttack.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UAnimNotify_PlayerAttack : public UAnimNotify
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName m_ID="BaseAttack";
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;
};
