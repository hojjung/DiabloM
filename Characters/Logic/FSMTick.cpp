#include "FSMTick.h"

#include "NavigationSystem.h"
#include "Characters/UnitPawn.h"

void UFSMTick::Init(AUnitPawn* pawnUnit)
{
	m_fIdleTimer = -1.f;
	m_fChaseFindTimer = -1.f;
	m_Owner = Cast<AUnitPawn>(pawnUnit);
	
	check(m_Owner);

	m_CurrentState = EFSM::Idle;
	
	m_fAttackRange = pawnUnit->GetAttackSpeed();
	//
	m_AryStateFunction[static_cast<int>(EFSM::Idle)] = &UFSMTick::OnIdle;

	m_AryStateFunction[static_cast<int>(EFSM::Chase)] = &UFSMTick::OnChase;

	m_AryStateFunction[static_cast<int>(EFSM::Combat)] = &UFSMTick::OnCombat;

	m_AryStateFunction[static_cast<int>(EFSM::ManualMove)] = &UFSMTick::OnManualMove;

	//
	m_StartPoint = m_Owner->GetActorLocation();
}

void UFSMTick::TickFSM()
{
	QUICK_SCOPE_CYCLE_COUNTER(MOBFSM_Swamer_TickFSM);

	(this->*m_AryStateFunction[static_cast<int>(m_CurrentState)])();
}

void UFSMTick::SetManualMove(FVector goal)
{
	m_ManualMoveLocation = goal;
	m_CurrentState = EFSM::ManualMove;
}

void UFSMTick::ForceSetStateIdle()
{
	m_CurrentState = EFSM::Idle;
}


void UFSMTick::OnIdle()
{
	if (m_Owner->GetFocusedTarget())
	{
		//m_StartPoint=m_OwnerMonster->GetActorLocation();

		m_CurrentState = EFSM::Chase;

		return;
	}

	EPathFollowingStatus::Type Status = m_Owner->GetPfComp()->GetStatus();

	if (m_fIdleTimer > 0.f)
	{
		m_fIdleTimer -= m_Owner->m_fTickDeltaTime;

		if (EPathFollowingStatus::Idle == Status)
		{
		}
		return;
	}


	FNavLocation Result;

	if (EPathFollowingStatus::Idle == Status)
	{
		if (!m_Owner->m_NavSys->GetRandomPointInNavigableRadius(m_StartPoint, 500.f, Result))
		{
			return;
		}

		m_Owner->MoveToLocation(Result);

		m_fIdleTimer = FMath::FRandRange(3.f, 7.f);
	}
}

void UFSMTick::OnChase()
{
	EPathFollowingRequestResult::Type Result = EPathFollowingRequestResult::Failed;

	if(!m_Owner->GetFocusedTarget() ||!m_Owner->GetFocusedTarget()->IsAlive())
	{
		m_CurrentState = EFSM::Idle;
		return;
	}

	Result = m_Owner->MoveToActor(m_Owner->GetFocusedTarget());

	if (Result == EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		m_CurrentState = EFSM::Combat;
	}
	else if(Result == EPathFollowingRequestResult::Type::Failed)
	{
		//No nav path
		//m_Owner->FocusTarget(nullptr);
		//m_CurrentState = EFSM::Idle;
	}
	
}

void UFSMTick::OnCombat()
{
	if (!m_Owner->GetFocusedTarget() || !m_Owner->GetFocusedTarget()->IsAlive())
	{
		m_CurrentState = EFSM::Idle;
		m_Owner->FocusTarget(nullptr);

		return;
	}

	TryAttack();

	float DistSqr = FVector::DistSquared(m_Owner->GetActorLocation(),
	                                     m_Owner->GetFocusedTarget()->GetActorLocation());

	if (DistSqr > GetAttackRange())
	{
		m_CurrentState = EFSM::Chase;
	}
}

void UFSMTick::TryAttack()
{
	m_Owner->HomingRotateToTarget();

	m_Owner->TryAttack();
}

void UFSMTick::OnManualMove()
{
	
	auto Result = m_Owner->MoveToLocation(m_ManualMoveLocation);

	if (Result == EPathFollowingRequestResult::Type::AlreadyAtGoal || Result == EPathFollowingRequestResult::Type::Failed)
	{
		m_CurrentState = EFSM::Idle;
	}
	else//some time already at goal not work
	{
		float Dist = FVector::DistSquared2D(m_Owner->GetActorLocation(),m_ManualMoveLocation);

		if(Dist<40000.f)
		{
			m_CurrentState = EFSM::Idle;
		}
	}
}

