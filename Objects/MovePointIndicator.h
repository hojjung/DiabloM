// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"

#include "MovePointIndicator.generated.h"

UCLASS()
class DIABLOM_API AMovePointIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovePointIndicator();
	UPROPERTY(VisibleAnywhere)
	UDecalComponent* m_Decal;

};
