#pragma once

#include "DiabloM.h"
#include "Managers/RewardManager.h"
#include "Objs/Actor/CollisionInteract.h"
#include "PlayerAutoPlayFSM.generated.h"

class UMonsterSpawnManager;
class APlayerDiabloCharacter;

UCLASS()
class DIABLOM_API UPlayerAutoPlayFSM : public UObject
{
	GENERATED_BODY()

protected:
	enum EFSM
	{
		SearchIdle,
		ChaseItem,
		PickupItem,
		SearchEnemy,
		ChaseEnemy,
		Combat,
		ChasePortal,
		UsePortal,
		Length
	};

protected:
	EFSM m_CurrentState;

	UPROPERTY()
	APlayerDiabloCharacter* m_OwnedPlayer;
	UPROPERTY()
	URewardManager* m_RewardManager;
	UPROPERTY()
	UMonsterSpawnManager* m_SpawnManager;
	UPROPERTY()
	UDungeonManager* m_DgManager;
	UPROPERTY()
	ACollisionInteract* m_FocusedInteract;
	UPROPERTY()
	AMonsterPawn* m_FocusedMonster;

	float m_fPickupAbleRadius;

	float m_fPickItemTimer;
	
public:
	void Init(APlayerDiabloCharacter* player);

	void TickFSM(float deltaTime);

protected:
	typedef void (UPlayerAutoPlayFSM::*FPtrState)(void);

	FPtrState m_AryStateFunction[static_cast<int>(EFSM::Length)];

protected:
	void OnSearchIdle();

	void OnChaseItem();

	void OnChaseEnemy();

	void OnPickupItem();

	float GetAttackRangeSqr();
	
	void OnCombat();

	void TryAttack();

	void OnChasePortal();

	void OnUsePortal();
};
