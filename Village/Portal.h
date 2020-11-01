// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Village/VillageActor.h"
#include "Portal.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API APortal : public AVillageActor
{
	GENERATED_BODY()
	
protected:
	APortal();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Item")
   	UParticleSystemComponent* m_ParticleEffect;
	
protected:
	virtual void BeginPlay() override; // need binding

public:
	void SetPortal(bool bIsOn);
	
	virtual void Interact(AActor* instigator)override;
};
