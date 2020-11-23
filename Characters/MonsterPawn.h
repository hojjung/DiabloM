// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UnitPawn.h"

#include "Widgets/WorldMap/WorldWidget/FloatingStatusBarWidgetCompo.h"

#include "MonsterPawn.generated.h"

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
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UFloatingStatusBarWidgetCompo* m_StatusBar;
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
public:
	bool m_bIsMoving;
public: //need more monster
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void ShowStatusBar();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void HideStatusBar();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    bool IsStatusBarActive();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateHealthBar(float perOne);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void InitMonster(FDataTableRowHandle unitID, int level);

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

protected:
	void UpdateBound();
};
