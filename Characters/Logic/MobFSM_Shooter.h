#pragma once
#include "DiabloM.h"
#include "MobFSMBase.h"
#include "Characters/MonsterPawn.h"
#include "UObject/NoExportTypes.h"
#include "MobFSM_Shooter.generated.h"

UCLASS(Blueprintable)
class DIABLOM_API UMobFSM_Shooter : public UMobFSMBase
{
	GENERATED_BODY()

public:
    enum EFSM
    {
        Idle,
        Chase,
        TryShoot,
        Return,
        Flee,
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

    float m_fFleeRange;//if too close

    float m_fFleeRangeSqr;//if too close

    float m_fAcceptRotationEuler;

    float m_fAcceptRotationDot;

    float m_fTargetDistSqr;

    float m_fFleeDestinationRange;

    float m_fFleeTimer;

    float m_fFleeStateCooldownTimer;

    typedef void (UMobFSM_Shooter::*FPtrState)(void);
	
    FPtrState m_AryStateFunction[static_cast<int>(EFSM::Length)];
    
protected:
    virtual void Init(AUnitPawn* pawnUnit) override;

    virtual void TickFSM() override;
    void DecisionByDistance();

    void OnIdle();

    void OnChase();

    void OnTryShoot();

    void OnReturn();

    void OnFlee();

public:
    FORCEINLINE float GetAttackRange()
    {
        return m_fAttackRange;
    }

    FORCEINLINE float GetAttackRangeSqr()
    {
        return m_fAttackRangeSqr;
    }

    FString& GetEnumName()
    {
        static FString IdleS ="IDLE";
        static FString ChaseS ="CHASE";
        static FString TryShootS ="TRYSHOOTS";
        static FString ReturnS ="RETURN";
        static FString FleeS ="FLEE";
        static FString None ="None";
        
        switch (m_CurrentState)
        {
        case Idle:
            return IdleS;
        case Chase:
            return ChaseS;
        case TryShoot:
            return TryShootS;
        case Return:
            return ReturnS;
        case Flee:
            return FleeS;
        }
        return None;
    }
};
