// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Characters/UnitPawn.h"
#include "DefaultFSM.generated.h"


UCLASS()
class DIABLOM_API UDefaultFSM : public UObject
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
    UPROPERTY()
    AUnitPawn* m_OwnerUnit;

    FVector m_StartPoint;

    EFSM m_CurrentState;
    
    typedef void (UDefaultFSM::*FPtrState)(void);

    FPtrState m_AryStateFunction[static_cast<int>(EFSM::Length)];

    float m_fIdleTimer;

    float m_fAttackRange; //from startPos
    
    float m_fAttackRangeSqr; //from startPos

    float m_fChaseRange;

protected:
    virtual void OnIdle();

    virtual void OnChase();

    virtual void OnCombat();

    virtual void TryAttack();

    virtual void OnReturn();

    virtual void OnFlee();

public:
    void Init(AUnitPawn* pawnUnit);

    void TickFSM();
    
public:
    FORCEINLINE float GetAttackRange()
    {
        return m_fAttackRange;
    }

    FORCEINLINE float GetChaseRange()
    {
        return m_fChaseRange;
    }

    FORCEINLINE float GetAttackRangeSqr()
    {
        return m_fAttackRangeSqr;
    }

};
