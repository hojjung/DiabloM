// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objs/Actor/CollisionInteract.h"
#include "DroppedGold.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API ADroppedGold : public ACollisionInteract
{
	GENERATED_BODY()

public:
	ADroppedGold(const FObjectInitializer& objInit);
	

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float m_fGoldAmount;

	FTextFormat m_Format;
protected:
	virtual void BeginPlay() override;
	void SetRandScale();
	virtual  void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
public:
	void SetGoldAmount(float amount);
};
