// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Characters/DiabloPlayerController.h"
#include "Engine/EngineTypes.h"
#include "UObject/NoExportTypes.h"
#include "PlayerSensing.generated.h"

/**
 * 
 */
class AMonsterPawn;
UCLASS()
class DIABLOM_API UPlayerSensing : public UObject
{
    GENERATED_BODY()
    friend AUnitPawn;
    friend APlayerDiabloCharacter;
    friend ADiabloPlayerController;

public:
    DECLARE_DELEGATE_OneParam(FSeePawnDelegate, APawn*);
    DECLARE_DELEGATE_ThreeParams(FHearNoiseDelegate, APawn*, const FVector&, float);

public:
    UPlayerSensing();
protected:
    float m_PeripheralVisionAngle;

    float m_PeripheralVisionCosine;

    float m_SightRadius;

    float m_FocusRange;

    float m_SensingInterval;

    UPROPERTY()
    APlayerDiabloCharacter* m_OwnedPlayer;

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

public:
    void InitSense(APlayerDiabloCharacter* player);

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
