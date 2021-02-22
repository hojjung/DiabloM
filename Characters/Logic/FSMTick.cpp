#include "FSMTick.h"

#include "NavigationSystem.h"
#include "Characters/UnitPawn.h"

void UFSMTick::Init(AUnitPawn* pawnUnit)
{
	m_fIdleTimer = -1.f;
	m_fChaseFindTimer = -1.f;
	m_OwnerMonster = Cast<AUnitPawn>(pawnUnit);
	
	check(m_OwnerMonster);

	m_CurrentState = EFSM::Idle;
	
	m_fAttackRange = pawnUnit->GetAttackSpeed();
	//
	m_AryStateFunction[static_cast<int>(EFSM::Idle)] = &UFSMTick::OnIdle;

	m_AryStateFunction[static_cast<int>(EFSM::Chase)] = &UFSMTick::OnChase;

	m_AryStateFunction[static_cast<int>(EFSM::Combat)] = &UFSMTick::OnCombat;

	//
	m_StartPoint = m_OwnerMonster->GetActorLocation();
}

void UFSMTick::TickFSM()
{
	QUICK_SCOPE_CYCLE_COUNTER(MOBFSM_Swamer_TickFSM);

	(this->*m_AryStateFunction[static_cast<int>(m_CurrentState)])();
}

void UFSMTick::OnIdle()
{
	if (m_OwnerMonster->GetFocusedTarget())
	{
		//m_StartPoint=m_OwnerMonster->GetActorLocation();

		m_CurrentState = EFSM::Chase;

		return;
	}

	EPathFollowingStatus::Type Status = m_OwnerMonster->GetPfComp()->GetStatus();

	if (m_fIdleTimer > 0.f)
	{
		m_fIdleTimer -= m_OwnerMonster->m_fTickDeltaTime;

		if (EPathFollowingStatus::Idle == Status)
		{
		}
		return;
	}


	FNavLocation Result;

	if (EPathFollowingStatus::Idle == Status)
	{
		if (!m_OwnerMonster->m_NavSys->GetRandomPointInNavigableRadius(m_StartPoint, 500.f, Result))
		{
			return;
		}

		m_OwnerMonster->MoveToLocation(Result);

		m_fIdleTimer = FMath::FRandRange(3.f, 7.f);
	}
}

void UFSMTick::OnChase()
{
	EPathFollowingRequestResult::Type Result = EPathFollowingRequestResult::Failed;

	Result = m_OwnerMonster->MoveToActor(m_OwnerMonster->GetFocusedTarget());

	if (Result == EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		m_CurrentState = EFSM::Combat;
	}
	
}

void UFSMTick::OnCombat()
{
	if (!m_OwnerMonster->GetFocusedTarget() || !m_OwnerMonster->GetFocusedTarget()->IsAlive())
	{
		m_CurrentState = EFSM::Idle;
		m_OwnerMonster->FocusTarget(nullptr);

		return;
	}

	TryAttack();

	float DistSqr = FVector::DistSquared(m_OwnerMonster->GetActorLocation(),
	                                     m_OwnerMonster->GetFocusedTarget()->GetActorLocation());

	if (DistSqr > GetAttackRange())
	{
		m_CurrentState = EFSM::Chase;
	}
}

void UFSMTick::TryAttack()
{
	m_OwnerMonster->HomingRotateToTarget();

	m_OwnerMonster->TryAttack();
}

