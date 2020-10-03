#include "PlayerSensing.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Characters/MonsterPawn.h"
#include "Characters/PlayerDiabloCharacter.h"

UPlayerSensing::UPlayerSensing()
{
    m_SightRadius = 1200.f;
    m_PeripheralVisionAngle = 50.f;
    m_PeripheralVisionCosine = FMath::Cos(FMath::DegreesToRadians(m_PeripheralVisionAngle));

    m_SensingInterval = 0.3f;
}

void UPlayerSensing::InitSense(APlayerDiabloCharacter* player)
{
    m_OwnedPlayer = player;
    SetPeripheralVisionAngle(m_PeripheralVisionAngle);
    SetSensingUpdatesEnabled(true);
}

bool UPlayerSensing::TickTryFoundEnemy()
{
    FVector HalfSize = FVector(500, 75, 75);
    FVector InitPos = m_OwnedPlayer->GetBodyMesh()->GetComponentLocation();
    InitPos.Z += m_OwnedPlayer->GetCapsule()->GetScaledCapsuleHalfHeight();

    FVector TraceStart = InitPos + m_OwnedPlayer->GetCapsule()->GetForwardVector() * HalfSize.X;

    FVector TraceEnd = TraceStart;

    FHitResult OutHit;

    if (! UKismetSystemLibrary::BoxTraceSingleForObjects(
            GetWorld(),
            TraceStart, TraceEnd, HalfSize, m_OwnedPlayer->GetActorRotation(),
            m_OwnedPlayer->GetAryTarget(), false, m_OwnedPlayer->GetAryIgnoreActor(), EDrawDebugTrace::ForOneFrame,
            OutHit, true,
            FLinearColor::Blue)
        || !OutHit.GetActor())
    {
        return false;
    }

    AMonsterPawn* FocusedUnit = Cast<AMonsterPawn>(OutHit.GetActor());

    if (!HasLineOfSightTo(FocusedUnit))
    {
        return false;
    }

    if (m_OwnedPlayer->m_FocusedEnemy) //이미 있다면
    {
        if (FocusedUnit == m_OwnedPlayer->m_FocusedEnemy) //찾은애랑 이미 있는애랑 같으면 넘어감
        {
            return false;
        }
        m_OwnedPlayer->FocusTarget(nullptr);
    }

    m_OwnedPlayer->FocusTarget(FocusedUnit);

    return true;
}

void UPlayerSensing::SetPeripheralVisionAngle(const float NewPeripheralVisionAngle)
{
    m_PeripheralVisionAngle = NewPeripheralVisionAngle;
    m_PeripheralVisionCosine = FMath::Cos(FMath::DegreesToRadians(m_PeripheralVisionAngle));
}

void UPlayerSensing::SetViewRadius(const float radius)
{
    m_SightRadius = radius;
}

void UPlayerSensing::SetFocusRange(const float range)
{
    m_FocusRange=range;
}

float UPlayerSensing::GetPeripheralVisionAngle() const
{
    return m_PeripheralVisionAngle;
}

float UPlayerSensing::GetPeripheralVisionCosine() const
{
    return m_PeripheralVisionCosine;
}

void UPlayerSensing::SetSensingUpdatesEnabled(const bool bEnabled)
{
    if (bEnabled && m_SensingInterval > 0.f)
    {
        const float InitialDelay = (m_SensingInterval * FMath::SRand()) + KINDA_SMALL_NUMBER;
        SetTimer(InitialDelay);
    }
    else
    {
        SetTimer(0.f);
    }
}


void UPlayerSensing::SetTimer(const float TimeInterval)
{
    if (m_OwnedPlayer && GEngine->GetNetMode(GetWorld()) < NM_Client)
    {
        m_OwnedPlayer->GetWorldTimerManager().SetTimer(TimerHandle_OnTimer, this, &UPlayerSensing::OnTimer,
                                                       TimeInterval,
                                                       false);
    }
}


void UPlayerSensing::SetSensingInterval(const float NewSensingInterval)
{
    if (m_SensingInterval != NewSensingInterval)
    {
        m_SensingInterval = NewSensingInterval;

        if (m_OwnedPlayer)
        {
            if (m_SensingInterval <= 0.f)
            {
                SetTimer(0.f);
            }
            else
            {
                float CurrentElapsed = m_OwnedPlayer->GetWorldTimerManager().GetTimerElapsed(TimerHandle_OnTimer);

                CurrentElapsed = FMath::Max(0.f, CurrentElapsed);

                if (CurrentElapsed < m_SensingInterval)
                {
                    // Extend lifetime by remaining time.
                    SetTimer(m_SensingInterval - CurrentElapsed);
                }
                else if (CurrentElapsed > m_SensingInterval)
                {
                    // Basically fire next update, because time has already expired.
                    // Don't want to fire immediately in case an update tries to change the interval, looping endlessly.
                    SetTimer(KINDA_SMALL_NUMBER);
                }
            }
        }
    }
}


void UPlayerSensing::OnTimer()
{
    if (!m_OwnedPlayer || !m_OwnedPlayer->GetWorld())
    {
        return;
    }
    
    if (m_OwnedPlayer->GetFocusedTarget())
    {
        TickTryFoundEnemy();
        
        if (!CouldSeePawn(m_OwnedPlayer->GetFocusedTarget(),true))
        {
            m_OwnedPlayer->FocusTarget(nullptr);
            UpdateAISensing();
        }
    }
    else
    {
        UpdateAISensing();
    }
    SetTimer(m_SensingInterval);
};


AActor* UPlayerSensing::GetSensorActor() const
{
    return m_OwnedPlayer;
}

bool UPlayerSensing::IsSensorActor(const AActor* Actor) const
{
    return (Actor == GetSensorActor());
}

bool UPlayerSensing::HasLineOfSightTo(const AActor* Other) const
{
    AController* SensorController = m_OwnedPlayer->GetDiaController();

    if (!SensorController)
    {
        return false;
    }

    return SensorController->LineOfSightTo(Other, FVector::ZeroVector, true);
}


void UPlayerSensing::UpdateAISensing()
{
    for (AMonsterPawn* Pawn : TActorRange<AMonsterPawn>(m_OwnedPlayer->GetWorld()))
    {
        SensePawn(*Pawn);
    }
}


void UPlayerSensing::SensePawn(AMonsterPawn& Pawn)
{
    if (CouldSeePawn(&Pawn, true))
    {
        if (m_OwnedPlayer->GetFocusedTarget())
        {
            TickTryFoundEnemy();
        }
        else if (HasLineOfSightTo(&Pawn))
        {
            OnSeePawn.ExecuteIfBound(&Pawn);
        }
        else
        {
            OnSeePawnBlocked.ExecuteIfBound(&Pawn);
        }
    }
    else
    {
        OnCantSeePawn.ExecuteIfBound(&Pawn);
    }
}


bool UPlayerSensing::CouldSeePawn(APawn* Other, bool bMaySkipChecks) const
{
    if(!ShouldCheckVisibilityOf(Other))
    {
        return false;
    }
    
    FVector const OtherLoc = Other->GetActorLocation();
    FVector const SensorLoc = GetSensorLocation();
    FVector const SelfToOther = OtherLoc - SensorLoc;

    // check max sight distance
    float const SelfToOtherDistSquared = SelfToOther.SizeSquared();
    if (SelfToOtherDistSquared > FMath::Square(m_SightRadius))
    {
        return false;
    }

    // may skip if more than some fraction of maxdist away (longer time to acquire)
    if (bMaySkipChecks && (FMath::Square(FMath::FRand()) * SelfToOtherDistSquared > FMath::Square(0.4f * m_SightRadius)))
    {
        return false;
    }

    // 	UE_LOG(LogPath, Warning, TEXT("DistanceToOtherSquared = %f, SightRadiusSquared: %f"), SelfToOtherDistSquared, FMath::Square(SightRadius));

    // check field of view
    FVector const SelfToOtherDir = SelfToOther.GetSafeNormal();
    FVector const MyFacingDir = GetSensorRotation().Vector();

    // 	UE_LOG(LogPath, Warning, TEXT("DotProductFacing: %f, PeripheralVisionCosine: %f"), SelfToOtherDir | MyFacingDir, PeripheralVisionCosine);

    return ((SelfToOtherDir | MyFacingDir) >= m_PeripheralVisionCosine);
}


FVector UPlayerSensing::GetSensorLocation() const
{
    FVector SensorLocation(FVector::ZeroVector);
    const AActor* SensorActor = GetSensorActor();

    if (SensorActor)
    {
        FRotator ViewRotation;
        SensorActor->GetActorEyesViewPoint(SensorLocation, ViewRotation);
    }

    return SensorLocation;
}

FRotator UPlayerSensing::GetSensorRotation() const
{
    FRotator SensorRotation(FRotator::ZeroRotator);

    const AActor* SensorActor = GetSensorActor();

    if (SensorActor)
    {
        SensorRotation = SensorActor->GetActorRotation();
    }

    return SensorRotation;
}


bool UPlayerSensing::ShouldCheckVisibilityOf(APawn* Pawn) const
{
    return !Pawn->IsHidden();
}
