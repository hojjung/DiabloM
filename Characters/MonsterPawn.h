// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UnitPawn.h"
#include "Logic/MonsterSensing.h"
#include "Widgets/WorldMap/WorldWidget/FloatingStatusBarWidgetCompo.h"

#include "MonsterPawn.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API AMonsterPawn : public AUnitPawn
{
	GENERATED_BODY()
public:
	AMonsterPawn(const FObjectInitializer& objInit);

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UFloatingStatusBarWidgetCompo* m_StatusBar;
	UPROPERTY(EditAnywhere)
	FMonsterEntityHandle m_MonsterUnitHandle;
	UPROPERTY()
	UDefaultFSM* m_FSM;
	UPROPERTY()
	UMonsterSensing* m_MonsterSense;
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

	void SetHealthPercentage(AUnitPawn* target );

	virtual bool HasDropItem() override;

	virtual void FocusTarget(AUnitPawn* target) override;

	bool virtual CanSeeTarget() override;

	virtual FVector GetLastSeenLocation() override;
};
