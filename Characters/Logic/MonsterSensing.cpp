#include "MonsterSensing.h"
#include "NetworkingDistanceConstants.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/Pawns/MonsterPawn.h"
#include "Characters/Pawns/PlayerDiabloCharacter.h"

//플레이어가 그냥 사정거리에 들어오고
//가리는 벽이 없다면
//추적상태가 됨
//너무 오래 추적하면 포기하고 돌아감

UMonsterSensing::UMonsterSensing()
{
    m_OwnedMob=nullptr;
    m_SightRadius = 500.f;
    m_SensingInterval = 0.3f;
}

void UMonsterSensing::InitSense(AMonsterPawn* mobs)
{
    m_OwnedMob = mobs;
    m_CurrentPlayer = Cast<APlayerDiabloCharacter>( UGameplayStatics::GetPlayerPawn(this,0));
    SetSensingUpdatesEnabled(true);
}

void UMonsterSensing::SetSensingUpdatesEnabled(const bool bEnabled)
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

void UMonsterSensing::SetTimer(const float TimeInterval)
{
    if (m_OwnedMob && GEngine->GetNetMode(GetWorld()) < NM_Client)
    {
        m_OwnedMob->GetWorldTimerManager().SetTimer(m_TimerHandle_OnTimer, this, &UMonsterSensing::OnTimer,
                                                       TimeInterval,
                                                       false);
    }
}

void UMonsterSensing::SetSensingInterval(const float newSensingInterval)
{
    if (m_SensingInterval != newSensingInterval)
    {
        m_SensingInterval = newSensingInterval;

        if (m_OwnedMob)
        {
            if (m_SensingInterval <= 0.f)
            {
                SetTimer(0.f);
            }
            else
            {
                float CurrentElapsed = m_OwnedMob->GetWorldTimerManager().GetTimerElapsed(m_TimerHandle_OnTimer);

                CurrentElapsed = FMath::Max(0.f, CurrentElapsed);

                if (CurrentElapsed < m_SensingInterval)
                {
                    SetTimer(m_SensingInterval - CurrentElapsed);
                }
                else if (CurrentElapsed > m_SensingInterval)
                {
                    SetTimer(KINDA_SMALL_NUMBER);
                }
            }
        }
    }
}

void UMonsterSensing::OnTimer()
{
    if (!m_OwnedMob || !m_OwnedMob->GetWorld())
    {
        return;
    }
    
    if (!m_OwnedMob->GetFocusedTarget())
    {
        UpdateAISensing();
    }
    else//찾았으면?
    {
        if(!GetPlayer()->IsAlive())
        {
            m_OwnedMob->FocusTarget(nullptr);
        }
        if (HasLineOfSightTo(GetPlayer()))
        {
            m_LastPlayerSeen=GetPlayer()->GetActorLocation();//보임 ㅇㅇ
        }
    }
          
    SetTimer(m_SensingInterval);
};

bool UMonsterSensing::HasLineOfSightTo(const AActor* Other) const
{
    return LineOfSightTo(Other, FVector::ZeroVector, true);
}


void UMonsterSensing::UpdateAISensing()
{
    SensePawn(*GetPlayer());
}

void UMonsterSensing::SensePawn(APlayerDiabloCharacter& player)
{
    if (CouldSeePawn(&player, true))
    {
        if (HasLineOfSightTo(&player))
        {
            m_OwnedMob->FocusTarget(&player);
        }
    }
    else
    {
        
    }
}

bool UMonsterSensing::CouldSeePawn(APlayerDiabloCharacter* Other, bool bMaySkipChecks) const
{
    if(!ShouldCheckVisibilityOf(Other))
    {
        return false;
    }

    if(!Cast<AUnitPawn>(Other)->IsAlive())
    {
        return false;
    }
    
    FVector const OtherLoc = Other->GetActorLocation();
    FVector const SensorLoc = GetSensorLocation();
    FVector const SelfToOther = OtherLoc - SensorLoc;

    float const SelfToOtherDistSquared = SelfToOther.SizeSquared();
    float SS = FMath::Square(m_SightRadius);
    
    if (SelfToOtherDistSquared > SS)
    {
        return false;
    }

    if (bMaySkipChecks && (FMath::Square(FMath::FRand()) * SelfToOtherDistSquared > FMath::Square(0.4f * m_SightRadius)))
    {
        return false;
    }
    
    return true;
}


FVector UMonsterSensing::GetSensorLocation() const
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

FRotator UMonsterSensing::GetSensorRotation() const
{
    FRotator SensorRotation(FRotator::ZeroRotator);

    const AActor* SensorActor = GetSensorActor();

    if (SensorActor)
    {
        SensorRotation = SensorActor->GetActorRotation();
    }

    return SensorRotation;
}


bool UMonsterSensing::ShouldCheckVisibilityOf(APlayerDiabloCharacter* Pawn) const
{
    return Pawn&&(!Pawn->IsHidden() || !Pawn->IsAlive());
}

float UMonsterSensing::DistSqr(AActor* want)
{
    return FVector::DistSquared2D(m_OwnedMob->GetActorLocation(),want->GetActorLocation());
}


void UMonsterSensing::SetViewRadius(const float radius)
{
    m_SightRadius = radius;
}

AActor* UMonsterSensing::GetSensorActor() const
{
    return m_OwnedMob;
}

bool UMonsterSensing::IsSensorActor(const AActor* Actor) const
{
    return (Actor == GetSensorActor());
}


bool UMonsterSensing::LineOfSightTo(const AActor* Other, FVector ViewPoint, bool bAlternateChecks) const
{
    if( !Other )
    {
        return false;
    }

    if ( ViewPoint.IsZero() )
    {
        FRotator ViewRotation;
        GetActorEyesViewPoint(ViewPoint, ViewRotation);
    }

    FCollisionQueryParams CollisionParms(SCENE_QUERY_STAT(LineOfSight), true, Other);
    CollisionParms.AddIgnoredActor(m_OwnedMob);
    FVector TargetLocation = Other->GetTargetLocation(m_OwnedMob);
    bool bHit = GetWorld()->LineTraceTestByChannel(ViewPoint, TargetLocation, ECC_Visibility, CollisionParms);
    if( !bHit )
    {
        return true;
    }

    if (!Cast<const APawn>(Other) && Cast<UCapsuleComponent>(Other->GetRootComponent()) == NULL)
    {
        return false;
    }
    float distSq = (Other->GetActorLocation() - ViewPoint).SizeSquared();
    if ( distSq > FARSIGHTTHRESHOLDSQUARED )
    {
        return false;
    }
    if ( !Cast<const APawn>(Other) && (distSq > NEARSIGHTTHRESHOLDSQUARED) ) 
    {
        return false;
    }

    float OtherRadius, OtherHeight;
    Other->GetSimpleCollisionCylinder(OtherRadius, OtherHeight);
	
    //try viewpoint to head
    bHit = GetWorld()->LineTraceTestByChannel(ViewPoint,  Other->GetActorLocation() + FVector(0.f,0.f,OtherHeight), ECC_Visibility, CollisionParms);
    return !bHit;
}

void UMonsterSensing::GetActorEyesViewPoint( FVector& out_Location, FRotator& out_Rotation ) const
{
    if (m_OwnedMob)
    {
        m_OwnedMob->GetActorEyesViewPoint( out_Location, out_Rotation );
    }
}

APlayerDiabloCharacter* UMonsterSensing::GetPlayer()
{
    return m_CurrentPlayer;
}

