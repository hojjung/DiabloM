// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MobFSMBase.h"

#include "MobFSM_Swamer.generated.h"

class AMonsterPawn;

UCLASS()
class DIABLOM_API UMobFSM_Swamer : public UMobFSMBase
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
    UPROPERTY()
    AMonsterPawn* m_OwnerMonster;

    FVector m_StartPoint;

    float m_fIdleTimer;

    float m_fChaseFindTimer;

    float m_fAttackRange; //from startPos
    
    float m_fAttackRangeSqr; //from startPos

    typedef void (UMobFSM_Swamer::*FPtrState)(void);
	
    FPtrState m_AryStateFunction[static_cast<int>(EFSM::Length)];
    
protected:
    virtual void Init(AUnitPawn* pawnUnit) override;

    virtual void TickFSM() override;
    
    void OnIdle();

    void OnChase();

    void OnCombat();

    void TryAttack();

    void OnReturn();

public:
    FORCEINLINE float GetAttackRange()
    {
        return m_fAttackRange;
    }

    FORCEINLINE float GetAttackRangeSqr()
    {
        return m_fAttackRangeSqr;
    }

};
