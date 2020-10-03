// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Characters/UnitPawn.h"
#include "UObject/NoExportTypes.h"
#include "DefaultFSM.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UDefaultFSM : public UObject
{
	GENERATED_BODY()


public:
	UDefaultFSM();

	void Init(AUnitPawn* pawnUnit);
	void TickFSM();

	UPROPERTY()
	AUnitPawn* m_OwnerUnit;
};
