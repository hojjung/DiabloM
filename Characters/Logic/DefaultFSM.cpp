
#include "DefaultFSM.h"

#include "NavigationSystem.h"


void UDefaultFSM::Init(AUnitPawn* pawnUnit)
{
    m_OwnerUnit=pawnUnit;
	m_fAttackRange = 100.f;
	m_CurrentState = EFSM::Idle;
	//
	m_AryStateFunction[static_cast<int>(EFSM::Idle)] = &UDefaultFSM::OnIdle;
	m_AryStateFunction[static_cast<int>(EFSM::Chase)] = &UDefaultFSM::OnChase;
	m_AryStateFunction[static_cast<int>(EFSM::Combat)] = &UDefaultFSM::OnCombat;
	m_AryStateFunction[static_cast<int>(EFSM::Return)] = &UDefaultFSM::OnReturn;

	m_StartPoint=m_OwnerUnit->GetActorLocation();
}

void UDefaultFSM::TickFSM()
{
    if (!m_OwnerUnit->m_NavSys)
    {
        return;
    }

	(this->*m_AryStateFunction[static_cast<int>(m_CurrentState)])();
}

void UDefaultFSM::OnIdle()
{
	if (m_OwnerUnit->GetFocusedTarget())
	{
		m_CurrentState = EFSM::Chase;
		return;
	}
	
	if (m_fIdleTimer > 0.f)
	{
		m_fIdleTimer -= m_OwnerUnit->m_fTickDeltaTime;

		return;
	}

	EPathFollowingStatus::Type Status = m_OwnerUnit->m_PFComp->GetStatus();
	FNavLocation Result;
	switch (Status)
	{
	case EPathFollowingStatus::Idle:

		if (!m_OwnerUnit->m_NavSys->GetRandomPointInNavigableRadius(m_StartPoint, 500.f, Result))
		{
			return;
		}
		
		m_OwnerUnit->MoveToLocation(Result);

		m_fIdleTimer = FMath::FRandRange(3.f,7.f);
		break;
	default: ;
	}
}

void UDefaultFSM::OnChase()
{
	bool CanSeeTarget =m_OwnerUnit->CanSeeTarget();

	if(CanSeeTarget)
	{
		EPathFollowingRequestResult::Type Result = m_OwnerUnit->MoveToActor(m_OwnerUnit->GetFocusedTarget());

		if (Result == EPathFollowingRequestResult::Type::AlreadyAtGoal)
		{
			m_CurrentState = EFSM::Combat;
		}
	}
	else
	{
		if(m_OwnerUnit->GetFocusedTarget())
		{
			EPathFollowingRequestResult::Type Result = m_OwnerUnit->MoveToLocation(m_OwnerUnit->GetLastSeenLocation());

			if (Result == EPathFollowingRequestResult::Type::AlreadyAtGoal)
			{
				Result = m_OwnerUnit->MoveToActor(m_OwnerUnit->GetFocusedTarget());
			}
			//라스티씬이 업데이트가 안된상태로 플레이어를 놓치면?
			//마지막으로 본곳으로 가고도 놓처버리면?
		}
	}
	

	
	//계속 도착못하고 쫓아가면

	//그만 쫓음
}

void UDefaultFSM::OnCombat()
{
	TryAttack();

	if (!m_OwnerUnit->GetFocusedTarget() ||m_OwnerUnit->GetFocusedTarget()->IsAlive())
	{
		m_CurrentState = EFSM::Return;
		
		m_OwnerUnit->FocusTarget(nullptr);
		
		return;
	}

	float DistSqr = FVector::DistSquared(m_OwnerUnit->GetActorLocation(), m_OwnerUnit->GetFocusedTarget()->GetActorLocation());

	if (DistSqr > GetAttackRange())
	{
		m_CurrentState = EFSM::Chase;
	}
}

void UDefaultFSM::TryAttack()
{
	m_OwnerUnit->HomingRotateToTarget();

	PRINTF("FSM-TryAttack");

	m_OwnerUnit->DoBaseAttack();
}

void UDefaultFSM::OnReturn()
{
	if (m_OwnerUnit->GetFocusedTarget())
	{
		m_CurrentState = EFSM::Chase;
		return;
	}
	auto Result= m_OwnerUnit->MoveToLocation(m_StartPoint);
	
	if (Result == EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		m_CurrentState = EFSM::Idle;
	}
}

void UDefaultFSM::OnFlee()
{
}
