// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "MobFSMBase.generated.h"


class AUnitPawn;

UCLASS(Abstract)
 class DIABLOM_API UMobFSMBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init(AUnitPawn* pawnUnit){};

	virtual void TickFSM(){};
};
