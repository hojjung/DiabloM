// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UnitPawn.h"
#include "Widgets/WorldMap/WorldWidget/FloatingStatusBarWidgetCompo.h"


#include "MonsterPawn.generated.h"

class UDungeonManager;
/**
 * 
 */
class UMobFSMBase;
class UMonsterSensing;
UCLASS()
class DIABLOM_API AMonsterPawn : public AUnitPawn,public ITickHideable
{
	GENERATED_BODY()
public:
	AMonsterPawn(const FObjectInitializer& objInit);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* m_StShadow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFloatingStatusBarWidgetCompo* m_WorldHpBar;
protected:
	UPROPERTY()
	UDungeonManager* m_SpawnedManager;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMonsterEntityHandle m_MonsterUnitHandle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> m_GEExpReward;
	UPROPERTY()
	UMobFSMBase* m_FSM;
	UPROPERTY()
	UMonsterSensing* m_MonsterSense;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool m_bIsPlaced;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float m_fDropRadius;
	FMonsterItemDropRow* m_DropDataRow;

	QuadtreeNode* m_CurrentNode;

	bool m_bIsVisible;
	
public: //need more monster
    void ShowStatusBar();
	
    void HideStatusBar();
	
    bool IsStatusBarActive();
	
	void UpdateHealthBar(float perOne);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void InitMonster(FDataTableRowHandle unitID, int level=1,UDungeonManager* dgManager=nullptr);

	void GiveExpToPlayer();

	virtual void Die() override;
	void RequestDropRewards();
	virtual void OnDeathAnimEnd() override;

	void SetHealthPercentage(const FOnAttributeChangeData& data);

	virtual void FocusTarget(AUnitPawn* target) override;

	bool virtual CanSeeTarget() override;

	virtual FVector GetLastSeenLocation() override;

public:
	virtual FVector GetActorLocation() override;

	virtual void RegisterToQuadTreeBound() override ;

	virtual void ShowAll(bool hasBeenShowed) override;
	
	virtual void HideAll(bool hasBeenShowed) override;

	virtual void SetNode(QuadtreeNode* quadtree_node)override;

	virtual QuadtreeNode* GetCurrentNode()override;

	UFUNCTION(BlueprintCallable)
	bool IsMoving()
	{
		return !GetMovementComponent()->Velocity.IsNearlyZero(0.01f);
	}

protected:
	void UpdateBound();
};
