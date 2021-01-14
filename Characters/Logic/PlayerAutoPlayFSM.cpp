#include "PlayerAutoPlayFSM.h"

#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"
#include "Managers/MonsterSpawnManager.h"

void UPlayerAutoPlayFSM::Init(APlayerDiabloCharacter* player)
{
	m_OwnedPlayer = player;
	m_CurrentState = EFSM::SearchIdle;
	m_RewardManager = UDiabloGameInstance::Get->GetRewardManager();
	m_SpawnManager = UDiabloGameInstance::Get->GetMonsterSpawn();
	m_DgManager = UDiabloGameInstance::Get->GetDungeonManager();
	m_fPickupAbleRadius = 300.f;
	m_fPickupAbleRadius = m_fPickupAbleRadius * m_fPickupAbleRadius;
	//
	m_AryStateFunction[static_cast<int>(EFSM::SearchIdle)] = &UPlayerAutoPlayFSM::OnSearchIdle;
	m_AryStateFunction[static_cast<int>(EFSM::ChaseItem)] = &UPlayerAutoPlayFSM::OnChaseItem;
	m_AryStateFunction[static_cast<int>(EFSM::PickupItem)] = &UPlayerAutoPlayFSM::OnPickupItem;
	m_AryStateFunction[static_cast<int>(EFSM::ChaseEnemy)] = &UPlayerAutoPlayFSM::OnChaseEnemy;
	m_AryStateFunction[static_cast<int>(EFSM::Combat)] = &UPlayerAutoPlayFSM::OnCombat;
	m_AryStateFunction[static_cast<int>(EFSM::ChasePortal)] = &UPlayerAutoPlayFSM::OnChasePortal;
	m_AryStateFunction[static_cast<int>(EFSM::UsePortal)] = &UPlayerAutoPlayFSM::OnUsePortal;


	//m_AryStateFunction[static_cast<int>(EFSM::Return)] = &UMobFSM_Swamer::OnReturn;
}

void UPlayerAutoPlayFSM::TickFSM(float deltaTime)
{
	(this->*m_AryStateFunction[static_cast<int>(m_CurrentState)])();

	if (m_FocusedInteract)
	{
		DrawDebugLine(m_OwnedPlayer->GetWorld(), m_OwnedPlayer->GetActorLocation(),
		              m_FocusedInteract->GetActorLocation(), FColor::Red);
	}
}

void UPlayerAutoPlayFSM::OnSearchIdle()
{
	FVector PlayerLoc = m_OwnedPlayer->GetActorLocation();

	m_FocusedInteract = m_RewardManager->GetNearestCollActor(m_OwnedPlayer);

	AMonsterPawn* MobContainHide = m_SpawnManager->GetNearestMonster(PlayerLoc, true);

	AMonsterPawn* MobNotContainHide = m_SpawnManager->GetNearestMonster(PlayerLoc, false);

	if (MobNotContainHide)
	{
		m_FocusedMonster = MobNotContainHide;
	}
	else
	{
		m_FocusedMonster = MobContainHide;
	}


	if (!m_FocusedMonster)
	{
		m_FocusedMonster = m_SpawnManager->GetNearestMonster(PlayerLoc, true);
	}

	if (m_FocusedMonster && m_FocusedInteract)
	{
		FVector InteractLoc = m_FocusedInteract->GetActorLocation();

		FVector MonsterLoc = m_FocusedMonster->GetActorLocation();

		float InteractDist = FVector::DistSquared2D(PlayerLoc, InteractLoc);

		float MonsterDist = FVector::DistSquared2D(PlayerLoc, MonsterLoc);
		//
		if (InteractDist <= MonsterDist)
		{
			m_CurrentState = EFSM::ChaseItem;

			return;
		}

		m_CurrentState = EFSM::ChaseEnemy;

		return;
	}

	if (m_FocusedInteract)
	{
		m_CurrentState = EFSM::ChaseItem;

		return;
	}

	if (m_FocusedMonster)
	{
		m_CurrentState = EFSM::ChaseEnemy;

		return;
	}

	if (m_DgManager->GetDgCompletePortalOpen())
	{
		m_CurrentState = EFSM::ChasePortal;
	}
}

void UPlayerAutoPlayFSM::OnChaseItem()
{
	if (!m_FocusedInteract || !m_FocusedInteract->m_bIsActingForGame)
	{
		m_CurrentState = EFSM::SearchIdle;
	}

	float Dist = FVector::DistSquared2D(m_OwnedPlayer->GetActorLocation(), m_FocusedInteract->GetActorLocation());

	if (Dist <= m_fPickupAbleRadius)
	{
		m_CurrentState = EFSM::PickupItem;

		return;
	}

	EPathFollowingRequestResult::Type RequestResult = m_OwnedPlayer->MoveToActor(m_FocusedInteract);

	if (RequestResult == EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		if (m_FocusedInteract && !m_FocusedInteract->IsHidden())
		{
			m_CurrentState = EFSM::PickupItem;
		}
	}
	else if (RequestResult == EPathFollowingRequestResult::Type::Failed)
	{
		if (m_FocusedMonster)
		{
			m_CurrentState = EFSM::ChaseEnemy;

			return;
		}

		if (m_DgManager->GetDgCompletePortalOpen())
		{
			m_CurrentState = EFSM::ChasePortal;

			return;
		}

		m_FocusedInteract = m_RewardManager->GetNearestCollActor(m_OwnedPlayer, m_FocusedInteract);

		if (!m_FocusedInteract)
		{
			m_CurrentState = EFSM::SearchIdle;
		}
	}
}

void UPlayerAutoPlayFSM::OnChaseEnemy()
{
	FVector PlayerLoc = m_OwnedPlayer->GetActorLocation();

	if (!m_FocusedMonster || !m_FocusedMonster->IsAlive())
	{
		m_CurrentState = EFSM::SearchIdle;

		return;
	}

	AMonsterPawn* NewMob = m_SpawnManager->GetNearestMonster(PlayerLoc, false, m_FocusedMonster);

	if (NewMob)
	{
		float Dist1 = FVector::DistSquared2D(PlayerLoc, NewMob->GetActorLocation());

		float Dist2 = FVector::DistSquared2D(PlayerLoc, m_FocusedMonster->GetActorLocation());

		if (Dist1 <= Dist2)
		{
			m_FocusedMonster = NewMob;
		}
	}

	EPathFollowingRequestResult::Type Result = EPathFollowingRequestResult::Failed;

	Result = m_OwnedPlayer->MoveToActor(m_FocusedMonster,5.f);

	if (Result == EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		m_CurrentState = EFSM::Combat;
	}
}

void UPlayerAutoPlayFSM::OnPickupItem()
{
	m_fPickItemTimer -= m_OwnedPlayer->m_fTickDeltaTime;

	if (m_fPickItemTimer <= 0.f)
	{
		m_FocusedInteract->Interact(m_OwnedPlayer);

		m_FocusedInteract = nullptr;

		m_FocusedMonster = nullptr;

		m_CurrentState = EFSM::SearchIdle;

		m_fPickItemTimer = 0.15f;
	}
}

void UPlayerAutoPlayFSM::OnCombat()
{
	if (!m_FocusedMonster || !m_FocusedMonster->IsAlive() || m_FocusedMonster->IsHidden())
	{
		m_CurrentState = EFSM::SearchIdle;

		m_FocusedMonster = nullptr;

		return;
	}
	m_OwnedPlayer->FocusTarget(m_FocusedMonster);

	TryAttack();

	float DistSqr = FVector::DistSquared(m_OwnedPlayer->GetActorLocation(), m_FocusedMonster->GetActorLocation());

	if (DistSqr > GetAttackRangeSqr())
	{
		m_CurrentState = EFSM::ChaseEnemy;
	}
}

void UPlayerAutoPlayFSM::TryAttack()
{
	m_OwnedPlayer->HomingRotateToTarget();

	m_OwnedPlayer->DoBaseAttack();
}

void UPlayerAutoPlayFSM::OnChasePortal()
{
	EPathFollowingRequestResult::Type Result = EPathFollowingRequestResult::Failed;

	Result = m_OwnedPlayer->MoveToActor(m_DgManager->GetDgCompletePortalOpen());

	if (Result == EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		m_CurrentState = EFSM::UsePortal;
	}
	else if (Result == EPathFollowingRequestResult::Type::Failed)
	{
		m_CurrentState = EFSM::SearchIdle;
	}
}

void UPlayerAutoPlayFSM::OnUsePortal()
{
	m_DgManager->GetDgCompletePortalOpen()->Interact(m_OwnedPlayer);
	//
	m_CurrentState = EFSM::SearchIdle;
	//
	m_OwnedPlayer->SetAutoPlay(false);
}

float UPlayerAutoPlayFSM::GetAttackRangeSqr()
{
	//현재 시전 사거리 * 0.85

	//액션 스킬바에서 사용할수 있는 스킬 모음

	//사용할수 있는 스킬 없으면 item이나 잧음

	//스킬들이 사거리 지수를 가지고 있어야함

	return 300 * 300;
}
