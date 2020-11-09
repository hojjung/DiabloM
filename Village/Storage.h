// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item/ItemHolder.h"
#include "Village/VillageActor.h"
#include "Storage.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API AStorage : public AVillageActor
{
	GENERATED_BODY()

public:
	AStorage();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* m_MeshStorage;
	
public:
	virtual void Interact(AActor* instigator) override;
};
