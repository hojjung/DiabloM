// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objs/Actor/CollisionInteract.h"
#include "HealthSphere.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API AHealthSphere : public ACollisionInteract
{
	GENERATED_BODY()

public:
    AHealthSphere(const FObjectInitializer& objInit);
    
protected:
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    UParticleSystemComponent* m_Particle;
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    FGameplayTag m_TagMagnitude;
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    TSubclassOf<UGameplayEffect> m_GEEffect;
    
protected:
    virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

public:
    void DropEnd();

    virtual void ShowAll(bool hasBeenShowed) override ;
	
    virtual void HideAll(bool hasBeenShowed) override ;
};
