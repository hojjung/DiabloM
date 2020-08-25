// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "GameFramework/PawnMovementComponent.h"
#include "AI/Navigation/NavigationAvoidanceTypes.h"
#include "AI/RVOAvoidanceInterface.h"
#include "UnitMovement.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UUnitMovement : public UPawnMovementComponent,  public IRVOAvoidanceInterface
{
	GENERATED_BODY()
	
public:
	UUnitMovement();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float m_AvoidanceWeight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	FNavAvoidanceMask m_AvoidanceGroup;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	FNavAvoidanceMask m_GroupsToAvoid;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	FNavAvoidanceMask m_GroupsToIgnore;
	UPROPERTY(EditAnywhere,Category="Movement")
	float m_fMaxSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	FRotator m_RotateSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool m_bUseRVO;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float m_AvoidConsiderRadius;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	int32 m_AvoidanceUID;

protected://pathfollow
	FVector m_MoveVector;

	bool m_bWasAvoidanceUpdated;

	float m_CapsuleHeight;

	float m_CapsuleRadius;

	FVector m_AvoidanceLockVelocity;

	float m_AvoidanceLockTimer;

public:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void TickRotate(float deltaTime);

	virtual void StopActiveMovement() override;

	FRotator ComputeOrientToMovementRotation(const FRotator& CurrentRotation) const;

	void SetMoveSpeed(float newSpeed);
#pragma region RVO
	// IRVOAvoidanceInterface��(��) ���� ��ӵ�
	virtual void SetRVOAvoidanceUID(int32 UID) override;
	virtual int32 GetRVOAvoidanceUID() override;
	virtual void SetRVOAvoidanceWeight(float Weight) override;
	virtual float GetRVOAvoidanceWeight() override;
	virtual FVector GetRVOAvoidanceOrigin() override;
	virtual float GetRVOAvoidanceRadius() override;
	virtual float GetRVOAvoidanceHeight() override;
	virtual float GetRVOAvoidanceConsiderationRadius() override;
	virtual FVector GetVelocityForRVOConsideration() override;
	virtual int32 GetAvoidanceGroupMask() override;
	virtual int32 GetGroupsToAvoidMask() override;
	virtual int32 GetGroupsToIgnoreMask() override;
	void UpdateDefaultRVO();
	void SetAvoidanceVelocityLock(class UAvoidanceManager* Avoidance, float Duration);
	virtual void CalcAvoidanceVelocity(float DeltaTime);
	virtual void NotifyBumpedPawn(APawn* BumpedPawn) override;
	// RVO Avoidance
#pragma endregion
	
};
