// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Engine/EngineTypes.h"

#include "AnimNotify_TraceSphere.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UAnimNotify_TraceSphere : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_TraceSphere();

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float m_fRange;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FGameplayTag m_CachedAttackEvent;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float m_fSphereRadius;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> m_ObjType;

protected:
	TArray<AActor*> m_IgnoreActors;

public:
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;
};
