#include "MobFSM_Shooter.h"

#include "NavigationSystem.h"

void UMobFSM_Shooter::Init(AUnitPawn* pawnUnit)
{
	m_fFleeStateCooldownTimer = -1.f;
	
	m_fFleeTimer = -1.f;
	
	m_fIdleTimer = -1.f;

	m_fChaseFindTimer = -1.f;

	m_OwnerMonster = Cast<AMonsterPawn>(pawnUnit);

	check(m_OwnerMonster);
//아니 무엇보다 AttackSpeed도 0인데
	m_fAttackRange = pawnUnit->GetAttackRange(); //캐릭터마다 다름

	m_fFleeRange = 300.f;

	m_CurrentState = EFSM::Idle;

	m_fAcceptRotationEuler = 45.f;
	//
	m_AryStateFunction[static_cast<int>(EFSM::Idle)] = &UMobFSM_Shooter::OnIdle;

	m_AryStateFunction[static_cast<int>(EFSM::Chase)] = &UMobFSM_Shooter::OnChase;

	m_AryStateFunction[static_cast<int>(EFSM::TryShoot)] = &UMobFSM_Shooter::OnTryShoot;

	m_AryStateFunction[static_cast<int>(EFSM::Flee)] = &UMobFSM_Shooter::OnFlee;

	m_AryStateFunction[static_cast<int>(EFSM::Return)] = &UMobFSM_Shooter::OnReturn;
	//
	m_StartPoint = m_OwnerMonster->GetActorLocation();
	//
	m_fFleeRangeSqr = m_fFleeRange * m_fFleeRange;

	m_fAttackRangeSqr = m_fAttackRange * m_fAttackRange;

	m_fAcceptRotationDot = FMath::Cos(FMath::DegreesToRadians(m_fAcceptRotationEuler));
	//
	m_fFleeDestinationRange = 1500.f;
}

void UMobFSM_Shooter::TickFSM()
{
	if(m_OwnerMonster->GetFocusedTarget())
	{
		m_fTargetDistSqr = FVector::DistSquared2D(m_OwnerMonster->GetActorLocation(), m_OwnerMonster->GetFocusedTarget()->GetActorLocation());
	}
	
	(this->*m_AryStateFunction[static_cast<int>(m_CurrentState)])();

	if(m_OwnerMonster)
		DrawDebugString(m_OwnerMonster->GetWorld(),m_OwnerMonster->GetActorLocation(),GetEnumName(),nullptr,FColor::White,0.1f,false,3);
}

void UMobFSM_Shooter::DecisionByDistance()
{
	m_fFleeStateCooldownTimer-=m_OwnerMonster->m_fTickDeltaTime;

	if (m_fFleeStateCooldownTimer<=0.f && m_fTargetDistSqr <= m_fFleeRangeSqr)
	{
		m_CurrentState = EFSM::Flee;

		m_fFleeStateCooldownTimer =  FMath::FRandRange(1.f, 3.f);
	}
	else if (m_fTargetDistSqr >= m_fAttackRangeSqr)
	{
		m_CurrentState = EFSM::Chase;
	}
	else
	{
		m_CurrentState = EFSM::TryShoot;
	}
}

void UMobFSM_Shooter::OnIdle()
{
	AUnitPawn* FocusedTarget = m_OwnerMonster->GetFocusedTarget();

	if (!FocusedTarget) //없으면 그냥 돌아다님
	{
		EPathFollowingStatus::Type Status = m_OwnerMonster->GetPfComp()->GetStatus();

		if (m_fIdleTimer > 0.f) //돌아다니기 타이머 체크
		{
			m_fIdleTimer -= m_OwnerMonster->m_fTickDeltaTime;

			return;
		}

		FNavLocation Result;

		if (EPathFollowingStatus::Idle == Status) //돌아다니기 세팅
		{
			if (!m_OwnerMonster->m_NavSys->GetRandomPointInNavigableRadius(m_StartPoint, 500.f, Result))
			{
				return;
			}

			m_OwnerMonster->MoveToLocation(Result);

			m_fIdleTimer = FMath::FRandRange(2.f, 5.f);
		}

		return;
	}

	DecisionByDistance();
}

void UMobFSM_Shooter::OnChase()
{
	bool CanSeeTarget = m_OwnerMonster->CanSeeTarget();

	EPathFollowingRequestResult::Type Result = EPathFollowingRequestResult::Failed;

	if (CanSeeTarget)
	{
		Result = m_OwnerMonster->MoveToActor(m_OwnerMonster->GetFocusedTarget(), m_fAttackRange);

		if (Result == EPathFollowingRequestResult::Type::AlreadyAtGoal)
		{
			m_CurrentState = EFSM::TryShoot;
		}
	}
	else //안보일때,안보이는채로 시간이 너무길면
	{
		if (m_OwnerMonster->GetFocusedTarget())
		{
			if (m_fChaseFindTimer > 0.f)
			{
				m_fChaseFindTimer -= m_OwnerMonster->m_fTickDeltaTime;

				if (m_fChaseFindTimer <= 0.f)
				{
					m_OwnerMonster->FocusTarget(nullptr);

					m_CurrentState = EFSM::Return;

					m_fChaseFindTimer = -1.f;
				}
				return;
			}

			if (FMath::RandBool())
			{
				Result = m_OwnerMonster->MoveToLocation(m_OwnerMonster->GetLastSeenLocation());
			}

			m_fChaseFindTimer = FMath::FRandRange(6.f, 12.f); //길게 뽑힌애는 계속 쫓아가고 짧은애는 중도 포기함
		}
		else
		{
			m_CurrentState = EFSM::Return;

			m_fChaseFindTimer = -1.f;
		}
	}
}

void UMobFSM_Shooter::OnTryShoot()
{
	if (!m_OwnerMonster->GetFocusedTarget() || !m_OwnerMonster->GetFocusedTarget()->IsAlive())
	{
		m_CurrentState = EFSM::Return;
		
		m_OwnerMonster->FocusTarget(nullptr);

		return;
	}

	if (!m_OwnerMonster->IsDotAngleAcceptForTarget(m_fAcceptRotationDot)) //각안나오면 돌림
	{
		m_OwnerMonster->HomingRotateToTarget();
		return;
	}

	if (m_fTargetDistSqr >= GetAttackRangeSqr())
	{
		m_CurrentState = EFSM::Chase;

		return;
	}

	m_OwnerMonster->DoBaseAttack();

	DecisionByDistance();
}

void UMobFSM_Shooter::OnReturn()
{
	if(m_OwnerMonster->GetFocusedTarget())
	{
		DecisionByDistance();

		return;
	}
	
	if (m_OwnerMonster->MoveToLocation(m_StartPoint) == EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		m_CurrentState = EFSM::Idle;//idle이 서칭을 해주기 때문
	}
}

void UMobFSM_Shooter::OnFlee()
{
	if(!m_OwnerMonster->GetFocusedTarget())
	{
		m_CurrentState = EFSM::Return;
		
		return;
	}

	if (m_fFleeTimer > 0.f) //돌아다니기 타이머 체크
	{
		m_fFleeTimer -= m_OwnerMonster->m_fTickDeltaTime;

		if(m_fFleeTimer<=.0f)
		{
			m_OwnerMonster->StopMove();
			DecisionByDistance();
		}

		return;
	}

	m_fFleeTimer = FMath::FRandRange(1.5f, 3.5f);

	FVector Dest = m_OwnerMonster->GetActorLocation();
	
	FNavLocation Result;

	if (m_OwnerMonster->m_NavSys->GetRandomPointInNavigableRadius(m_OwnerMonster->GetActorLocation(), m_fFleeDestinationRange, Result))
	{
		Dest = Result.Location;
	}

	if(m_OwnerMonster->MoveToLocation(Dest) == EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		DecisionByDistance();
	}
}
