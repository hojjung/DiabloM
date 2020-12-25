// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "GameFramework/Actor.h"
#include "Objs/Interfaces/Interactable.h"

#include "VillageActor.generated.h"

UCLASS()
class DIABLOM_API AVillageActor : public AActor,public IInteractable
{
	GENERATED_BODY()
	
public:	
	AVillageActor(const FObjectInitializer& objInit);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* m_Capsule;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UWidgetComponent* m_WidgetComp;

public:	
	virtual void Interact(AActor* instigator)override;
};
