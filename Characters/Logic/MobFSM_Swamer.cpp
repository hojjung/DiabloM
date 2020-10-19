#include "MobFSM_Swamer.h"
#include "Characters/UnitPawn.h"
#include "NavigationSystem.h"

void UMobFSM_Swamer::Init(AUnitPawn* pawnUnit)
{
	m_fIdleTimer=-1.f;
	m_fChaseFindTimer=-1.f;
    m_OwnerUnit=pawnUnit;
	m_fAttackRange = 100.f;
	m_CurrentState = EFSM::Idle;
	//
	m_AryStateFunction[static_cast<int>(EFSM::Idle)] = &UMobFSM_Swamer::OnIdle;
	m_AryStateFunction[static_cast<int>(EFSM::Chase)] = &UMobFSM_Swamer::OnChase;
	m_AryStateFunction[static_cast<int>(EFSM::Combat)] = &UMobFSM_Swamer::OnCombat;
	m_AryStateFunction[static_cast<int>(EFSM::Return)] = &UMobFSM_Swamer::OnReturn;
	//
	m_StartPoint=m_OwnerUnit->GetActorLocation();
}

void UMobFSM_Swamer::TickFSM()
{
	(this->*m_AryStateFunction[static_cast<int>(m_CurrentState)])();
}

void UMobFSM_Swamer::OnIdle()
{
	if (m_OwnerUnit->GetFocusedTarget())
	{
		m_StartPoint=m_OwnerUnit->GetActorLocation();
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

void UMobFSM_Swamer::OnChase()
{
	
	bool CanSeeTarget = m_OwnerUnit->CanSeeTarget();

	EPathFollowingRequestResult::Type Result=EPathFollowingRequestResult::Failed;

	if(CanSeeTarget)
	{
		Result = m_OwnerUnit->MoveToActor(m_OwnerUnit->GetFocusedTarget());

		if (Result == EPathFollowingRequestResult::Type::AlreadyAtGoal)
		{
			m_CurrentState = EFSM::Combat;
		}
	}
	else//안보일때,안보이는채로 시간이 너무길면
	{
		if(m_OwnerUnit->GetFocusedTarget())
		{
			if(m_fChaseFindTimer>0.f)
			{
				m_fChaseFindTimer -= m_OwnerUnit->m_fTickDeltaTime;

				if(m_fChaseFindTimer<=0.f)
				{
					m_OwnerUnit->FocusTarget(nullptr);
					m_CurrentState = EFSM::Return;
					m_fChaseFindTimer=-1.f;
				}
				return;
			}

			if(FMath::RandBool())
			{
				Result = m_OwnerUnit->MoveToLocation(m_OwnerUnit->GetLastSeenLocation());
			}
			
			m_fChaseFindTimer = FMath::FRandRange(6.f,12.f);//길게 뽑힌애는 계속 쫓아가고 짧은애는 중도 포기함
		}
		else
		{
			//null target
			m_CurrentState = EFSM::Return;
			m_fChaseFindTimer=-1.f;
		}
	}
}

void UMobFSM_Swamer::OnCombat()
{
	if(!m_OwnerUnit->GetFocusedTarget() ||!m_OwnerUnit->GetFocusedTarget()->IsAlive())
	{
		m_CurrentState = EFSM::Return;
		
		m_OwnerUnit->FocusTarget(nullptr);
		
		return;
	}
	
	TryAttack();

	

	float DistSqr = FVector::DistSquared(m_OwnerUnit->GetActorLocation(), m_OwnerUnit->GetFocusedTarget()->GetActorLocation());

	if (DistSqr > GetAttackRange())
	{
		m_CurrentState = EFSM::Chase;
	}
}

void UMobFSM_Swamer::TryAttack()
{
	m_OwnerUnit->HomingRotateToTarget();

	m_OwnerUnit->DoBaseAttack();
}

void UMobFSM_Swamer::OnReturn()
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

void UMobFSM_Swamer::OnFlee()
{
}
