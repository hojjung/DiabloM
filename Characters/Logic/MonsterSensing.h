#pragma once

#include "DiabloM.h"
#include "MonsterSensing.generated.h"

class APlayerDiabloCharacter;
class ADiabloPlayerController;
class AMonsterPawn;
class AUnitPawn;
;
UCLASS()
class DIABLOM_API UMonsterSensing : public UObject
{
	GENERATED_BODY()
	
	friend AUnitPawn;
	friend APlayerDiabloCharacter;
	friend ADiabloPlayerController;
	friend AMonsterPawn;

public:
	DECLARE_DELEGATE_OneParam(FSeePawnDelegate, APawn*);
	DECLARE_DELEGATE_ThreeParams(FHearNoiseDelegate, APawn*, const FVector&, float);

public:
	UMonsterSensing();
	
protected:
	float m_SightRadius;

	float m_SensingInterval;

	UPROPERTY()
	AMonsterPawn* m_OwnedMob;

	FVector m_LastPlayerSeen;

public:
	FSeePawnDelegate OnSeePawn;

	FSeePawnDelegate OnCantSeePawn;

	FSeePawnDelegate OnSeePawnBlocked;

protected:
	void SensePawn(APlayerDiabloCharacter& player);
	

	void OnTimer();

	FTimerHandle TimerHandle_OnTimer;

	void SetTimer(const float TimeDelay);

	void UpdateAISensing();

	float DistSqr(AActor* want);
public:
	void Tick();
	
	void InitSense(AMonsterPawn* mobs);

	void SetSensingInterval(const float newSensingInterval);

	void SetSensingUpdatesEnabled(const bool bEnabled);

	void SetViewRadius(const float radius);

	bool IsSensorActor(const AActor* Actor) const;

	bool ShouldCheckVisibilityOf(APlayerDiabloCharacter* Pawn) const;

	bool CouldSeePawn(APlayerDiabloCharacter* Other, bool bMaySkipChecks = false) const;

	bool HasLineOfSightTo(const AActor* Other) const;

	FVector GetSensorLocation() const;

	FRotator GetSensorRotation() const;

	AActor* GetSensorActor() const; // Get the actor used as the actual sensor location is derived from this actor.

	bool LineOfSightTo(const AActor* Other, FVector ViewPoint, bool bAlternateChecks) const;

	void GetActorEyesViewPoint( FVector& out_Location, FRotator& out_Rotation ) const;

	APlayerDiabloCharacter* GetPlayer();

	
};
