#pragma once

#include "DiabloM.h"
#include "Characters/MonsterPawn.h"
#include "MonsterSensing.generated.h"

class APlayerDiabloCharacter;
class ADiabloPlayerController;

UCLASS()
class DIABLOM_API UMonsterSensing : public UObject
{
	GENERATED_BODY()
	
	friend AUnitPawn;
	friend APlayerDiabloCharacter;
	friend ADiabloPlayerController;
	friend  AMonsterPawn;

public:
	DECLARE_DELEGATE_OneParam(FSeePawnDelegate, APawn*);
	DECLARE_DELEGATE_ThreeParams(FHearNoiseDelegate, APawn*, const FVector&, float);

public:
	UMonsterSensing();
	
protected:
	float m_AttackableAngle;

	float m_AttackableAngleCosine;

	float m_SightRadius;

	float m_SensingInterval;

	UPROPERTY()
	AMonsterPawn* m_OwnedPlayer;

public:
	FSeePawnDelegate OnSeePawn;

	FSeePawnDelegate OnCantSeePawn;

	FSeePawnDelegate OnSeePawnBlocked;

protected:
	void SensePawn(AMonsterPawn& Pawn);

	void OnTimer();

	FTimerHandle TimerHandle_OnTimer;

	void SetTimer(const float TimeDelay);

	void UpdateAISensing();

	AMonsterPawn* GetCloseMonster(const TArray<FHitResult>& aryMobs);

	float DistSqr(AActor* want);
public:
	void InitSense(AMonsterPawn* mobs);

	bool TickTryFoundInteraction();

	bool TickTryFoundEnemy();

	void SetSensingInterval(const float NewSensingInterval);

	void SetSensingUpdatesEnabled(const bool bEnabled);

	void SetPeripheralVisionAngle(const float NewPeripheralVisionAngle);

	void SetViewRadius(const float radius);

	void SetFocusRange(const float range);

	float GetPeripheralVisionAngle() const;

	float GetPeripheralVisionCosine() const;

	bool IsSensorActor(const AActor* Actor) const;

	bool ShouldCheckVisibilityOf(APawn* Pawn) const;

	bool CouldSeePawn(APawn* Other, bool bMaySkipChecks = false) const;

	bool HasLineOfSightTo(const AActor* Other) const;

	FVector GetSensorLocation() const;

	FRotator GetSensorRotation() const;

	AActor* GetSensorActor() const; // Get the actor used as the actual sensor location is derived from this actor.	
};
