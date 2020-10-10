// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MobFSMBase.h"
#include "MobFSM_Swamer.generated.h"

UCLASS()
class DIABLOM_API UMobFSM_Swamer : public UMobFSMBase
{
    GENERATED_BODY()

public:
 

   

protected:
    UPROPERTY()
    AUnitPawn* m_OwnerUnit;

    FVector m_StartPoint;

    float m_fIdleTimer;

    float m_fChaseFindTimer;

    float m_fAttackRange; //from startPos
    
    float m_fAttackRangeSqr; //from startPos

    float m_fChaseRange;

    typedef void (UMobFSM_Swamer::*FPtrState)(void);
	
    FPtrState m_AryStateFunction[static_cast<int>(EFSM::Length)];
protected:
    virtual void OnIdle() override;

    virtual void OnChase() override;

    virtual void OnCombat() override;

    virtual void TryAttack() override;

    virtual void OnReturn() override;

    virtual void OnFlee() override;

    virtual void Init(AUnitPawn* pawnUnit) override;

    virtual void TickFSM() override;
    
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
