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
	enum EFSM
	{
		Idle,
        Chase,
        Combat,
        Return,
        Length
    };

protected:
	EFSM m_CurrentState;


protected:
	virtual void OnIdle(){};

	virtual void OnChase(){};

	virtual void OnCombat(){};

	virtual void TryAttack(){};

	virtual void OnReturn(){};

	virtual void OnFlee(){};

public:
	virtual void Init(AUnitPawn* pawnUnit);

	virtual void TickFSM(){};

	
};
