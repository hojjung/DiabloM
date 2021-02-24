// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "UObject/NoExportTypes.h"
#include "FSMTick.generated.h"

class AUnitPawn;
/**
 * 
 */
UCLASS()
class DIABLOM_API UFSMTick : public UObject
{
	GENERATED_BODY()
	
public:
	enum EFSM
	{
		Idle,
		Chase,
		Combat,
		Length
	};

protected:
	UPROPERTY()
	AUnitPawn* m_Owner;
	
	EFSM m_CurrentState;

	FVector m_StartPoint;

	float m_fIdleTimer;

	float m_fChaseFindTimer;

	float m_fAttackRange; //from startPos

	float m_fAttackRangeSqr; //from startPos

	typedef void (UFSMTick::*FPtrState)(void);

	FPtrState m_AryStateFunction[static_cast<int>(EFSM::Length)];

protected:
	void OnIdle();

	void OnChase();

	void OnCombat();

	void TryAttack();

public:
	void Init(AUnitPawn* pawnUnit);
	
	FORCEINLINE float GetAttackRange()
	{
		return m_fAttackRange;
	}

	FORCEINLINE float GetAttackRangeSqr()
	{
		return m_fAttackRangeSqr;
	}

	void TickFSM();
};
