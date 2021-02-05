#pragma once

#include "CoreMinimal.h"
#include "Village/VillageActor.h"
#include "Portal.generated.h"

UCLASS()
class DIABLOM_API APortal : public AVillageActor
{
	GENERATED_BODY()
	
protected:
	APortal(const FObjectInitializer& objInit);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Item")
   	UParticleSystemComponent* m_ParticleEffect;
	
	
protected:
	virtual void BeginPlay() override; // need binding

public:
	void SetPortal(bool bIsOn);
	
	virtual void Interact(AActor* instigator)override;
};
