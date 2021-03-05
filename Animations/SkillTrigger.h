#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Engine/EngineTypes.h"

#include "SkillTrigger.generated.h"


UENUM()
enum class ESkillCastType :uint8
{
	Self,
	Target,
	Sphere,
	Box,
	Length
};

UCLASS()
class DIABLOM_API USkillTrigger : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName m_SkillID;
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;
};
