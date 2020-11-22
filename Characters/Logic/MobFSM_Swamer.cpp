#include "MobFSM_Swamer.h"
#include "Characters/MonsterPawn.h"
#include "NavigationSystem.h"

void UMobFSM_Swamer::Init(AUnitPawn* pawnUnit)
{
	m_fIdleTimer=-1.f;
	m_fChaseFindTimer=-1.f;
    m_OwnerMonster=Cast<AMonsterPawn>(pawnUnit);
	check(m_OwnerMonster);
	
	m_fAttackRange = 100.f;
	m_CurrentState = EFSM::Idle;
	//
	m_AryStateFunction[static_cast<int>(EFSM::Idle)] = &UMobFSM_Swamer::OnIdle;
	m_AryStateFunction[static_cast<int>(EFSM::Chase)] = &UMobFSM_Swamer::OnChase;
	m_AryStateFunction[static_cast<int>(EFSM::Combat)] = &UMobFSM_Swamer::OnCombat;
	m_AryStateFunction[static_cast<int>(EFSM::Return)] = &UMobFSM_Swamer::OnReturn;
	//
	m_StartPoint=m_OwnerMonster->GetActorLocation();

}

void UMobFSM_Swamer::TickFSM()
{
	(this->*m_AryStateFunction[static_cast<int>(m_CurrentState)])();
}

void UMobFSM_Swamer::OnIdle()
{
	if (m_OwnerMonster->GetFocusedTarget())
	{
		m_StartPoint=m_OwnerMonster->GetActorLocation();
		m_CurrentState = EFSM::Chase;
		
		return;
	}
	
	EPathFollowingStatus::Type Status = m_OwnerMonster->m_PFComp->GetStatus();
	
	if (m_fIdleTimer > 0.f)
	{
		m_fIdleTimer -= m_OwnerMonster->m_fTickDeltaTime;

		if(EPathFollowingStatus::Idle == Status)
		{
			m_OwnerMonster->m_bIsMoving=false;
		}
		return;
	}

	
	FNavLocation Result;

	if(EPathFollowingStatus::Idle == Status)
	{
		if (!m_OwnerMonster->m_NavSys->GetRandomPointInNavigableRadius(m_StartPoint, 500.f, Result))
		{
			return;
		}
		
		m_OwnerMonster->MoveToLocation(Result);

		m_fIdleTimer = FMath::FRandRange(3.f,7.f);
		
		m_OwnerMonster->m_bIsMoving=true;
	}
}

void UMobFSM_Swamer::OnChase()
{
	
	bool CanSeeTarget = m_OwnerMonster->CanSeeTarget();

	EPathFollowingRequestResult::Type Result=EPathFollowingRequestResult::Failed;

	if(CanSeeTarget)
	{
		Result = m_OwnerMonster->MoveToActor(m_OwnerMonster->GetFocusedTarget());

		if (Result == EPathFollowingRequestResult::Type::AlreadyAtGoal)
		{
			m_CurrentState = EFSM::Combat;
			m_OwnerMonster->m_bIsMoving=false;
		}
	}
	else//안보일때,안보이는채로 시간이 너무길면
	{
		if(m_OwnerMonster->GetFocusedTarget())
		{
			if(m_fChaseFindTimer>0.f)
			{
				m_fChaseFindTimer -= m_OwnerMonster->m_fTickDeltaTime;

				if(m_fChaseFindTimer<=0.f)
				{
					m_OwnerMonster->FocusTarget(nullptr);
					m_CurrentState = EFSM::Return;
					m_fChaseFindTimer=-1.f;
				}
				return;
			}

			if(FMath::RandBool())
			{
				Result = m_OwnerMonster->MoveToLocation(m_OwnerMonster->GetLastSeenLocation());
				m_OwnerMonster->m_bIsMoving=true;
			}
			
			m_fChaseFindTimer = FMath::FRandRange(6.f,12.f);//길게 뽑힌애는 계속 쫓아가고 짧은애는 중도 포기함
		}
		else
		{
			//null target
			m_CurrentState = EFSM::Return;
			m_OwnerMonster->m_bIsMoving=true;
			m_fChaseFindTimer=-1.f;
		}
	}
}

void UMobFSM_Swamer::OnCombat()
{
	if(!m_OwnerMonster->GetFocusedTarget() ||!m_OwnerMonster->GetFocusedTarget()->IsAlive())
	{
		m_CurrentState = EFSM::Return;
		m_OwnerMonster->m_bIsMoving=true;
		m_OwnerMonster->FocusTarget(nullptr);
		
		return;
	}
	
	TryAttack();

	

	float DistSqr = FVector::DistSquared(m_OwnerMonster->GetActorLocation(), m_OwnerMonster->GetFocusedTarget()->GetActorLocation());

	if (DistSqr > GetAttackRange())
	{
		m_CurrentState = EFSM::Chase;
		m_OwnerMonster->m_bIsMoving=true;
	}
}

void UMobFSM_Swamer::TryAttack()
{
	m_OwnerMonster->HomingRotateToTarget();

	m_OwnerMonster->DoBaseAttack();
}

void UMobFSM_Swamer::OnReturn()
{
	if (m_OwnerMonster->GetFocusedTarget())
	{
		m_CurrentState = EFSM::Chase;
		m_OwnerMonster->m_bIsMoving=true;
		return;
	}
	auto Result= m_OwnerMonster->MoveToLocation(m_StartPoint);
	
	if (Result == EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		m_CurrentState = EFSM::Idle;
		m_OwnerMonster->m_bIsMoving=false;

	}
}

void UMobFSM_Swamer::OnFlee()
{
}
