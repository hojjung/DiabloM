// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Engine/EngineTypes.h"

#include "AnimNotify_TraceBox.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UAnimNotify_TraceBox : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_TraceBox();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName m_ID="BaseAttack";
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float m_fRange;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FVector m_BoxHalfSize;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> m_ObjType;

protected:
	TArray<AActor*> m_IgnoreActors;

public:
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;
};
