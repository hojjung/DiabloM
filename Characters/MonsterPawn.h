// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UnitPawn.h"
#include "MonsterPawn.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API AMonsterPawn : public AUnitPawn
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditAnywhere)
	FMonsterTypeHandle m_MonsterUnitHandle;
	public: //need more monster
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void ShowStatusBar();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void HideStatusBar();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    bool IsStatusBarActive();

	virtual void BeginPlay() override;

	void SetUnitStat(FDataTableRowHandle unitID, int level);
};
