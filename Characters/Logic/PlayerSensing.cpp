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
    m_FocusRange=500.f;
    m_SensingInterval = 0.3f;
}

void UPlayerSensing::InitSense(APlayerDiabloCharacter* player)
{
    m_OwnedPlayer = player;
    SetPeripheralVisionAngle(m_PeripheralVisionAngle);
    SetSensingUpdatesEnabled(true);
}

bool UPlayerSensing::TickTryFoundInteraction()
{
    FVector TraceStart = m_OwnedPlayer->GetCapsule()->GetComponentLocation();
    FVector TraceEnd = TraceStart + m_OwnedPlayer->GetCapsule()->GetForwardVector() * m_OwnedPlayer->m_fInteractRange;
    
    FHitResult OutHit;
    
    if (!UKismetSystemLibrary::SphereTraceSingle(
            GetWorld(),
            TraceStart, TraceEnd, 100.f,
            ETraceTypeQuery::TraceTypeQuery3, false, m_OwnedPlayer->m_AryIgnoreActor, EDrawDebugTrace::ForOneFrame, OutHit, true)
        || !OutHit.GetActor())
    {
        if(m_OwnedPlayer->m_FocusedInteractable)
        {
            m_OwnedPlayer->m_FocusedInteractable=nullptr;
        }
        return false;
    }
    
    
    IInteractable* FoundIntract = Cast<IInteractable>(OutHit.GetActor());

    m_OwnedPlayer->m_FocusedInteractable=FoundIntract;

    return true;
}

bool UPlayerSensing::TickTryFoundEnemy()
{
    
    FVector HalfSize = FVector(m_FocusRange, 75, 75);
    FVector InitPos = m_OwnedPlayer->GetBodyMesh()->GetComponentLocation();
    InitPos.Z += m_OwnedPlayer->GetCapsule()->GetScaledCapsuleHalfHeight();

    FVector TraceStart = InitPos + m_OwnedPlayer->GetCapsule()->GetForwardVector() * HalfSize.X;

    FVector TraceEnd = TraceStart;

    TArray <FHitResult> AryOutHit;
    //FHitResult OutHit;

    if (! UKismetSystemLibrary::BoxTraceMultiForObjects(
            GetWorld(),
            TraceStart, TraceEnd, HalfSize, m_OwnedPlayer->GetActorRotation(),
            m_OwnedPlayer->GetAryTarget(), false, m_OwnedPlayer->GetAryIgnoreActor(), EDrawDebugTrace::ForOneFrame,
            AryOutHit, true,
            FLinearColor::Blue)
        )
    {
        return false;
    }

    

    AMonsterPawn* FocusedUnit = GetCloseMonster(AryOutHit);//,프레임저하 심하고 차이가 없음//몬스터 많아지니까 돌진이 계속써짐

    if(!FocusedUnit->IsAlive())
    {
        return false;
    }
    if (!HasLineOfSightTo(FocusedUnit))
    {
        return false;
    }

    if (m_OwnedPlayer->GetFocusedTarget()) //이미 있다면
    {
        if (FocusedUnit == m_OwnedPlayer->GetFocusedTarget()) //찾은애랑 이미 있는애랑 같으면 넘어감
        {
            return true;
        }
    }

    m_OwnedPlayer->FocusTarget(FocusedUnit);

    return true;
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


void UPlayerSensing::OnTimer()
{
    if (!m_OwnedPlayer || !m_OwnedPlayer->GetWorld())
    {
        return;
    }
    
    if (m_OwnedPlayer->GetFocusedTarget())//찾았으면?
    {
        bool CheckMonster= TickTryFoundEnemy();//꼬깔없는 포커싱
        
        if (!CouldSeePawn(m_OwnedPlayer->GetFocusedTarget(),false)&&!CheckMonster)//연산을 줄이려고 한듯
        {
            //즉포커싱으론 잡히는 상태인데
            //꼬깔이 한테 안보이니까 꼬갈한테 보이게 하려고 이러는거?
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
        if (HasLineOfSightTo(&Pawn))
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

    if(!Cast<AUnitPawn>(Other)->IsAlive())
    {
        return false;
    }
    
    FVector const OtherLoc = Other->GetActorLocation();
    FVector const SensorLoc = GetSensorLocation();
    FVector const SelfToOther = OtherLoc - SensorLoc;

    float const SelfToOtherDistSquared = SelfToOther.SizeSquared();
    if (SelfToOtherDistSquared > FMath::Square(m_SightRadius))
    {
        return false;
    }

    if (bMaySkipChecks && (FMath::Square(FMath::FRand()) * SelfToOtherDistSquared > FMath::Square(0.4f * m_SightRadius)))
    {
        return false;
    }
    
    FVector const SelfToOtherDir = SelfToOther.GetSafeNormal();
    FVector const MyFacingDir = GetSensorRotation().Vector();

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

AMonsterPawn* UPlayerSensing::GetCloseMonster(const TArray<FHitResult>& aryMobs)
{
    AActor* MinActor=aryMobs[0].GetActor();
    
    float MinDist=DistSqr(MinActor);
    
    for(int i=1; i<aryMobs.Num();i++)
    {
        float Dist=DistSqr(aryMobs[i].GetActor());
        if(MinDist>Dist)
        {
            MinActor=aryMobs[i].GetActor();
            MinDist=Dist;
        }
    }

    return Cast<AMonsterPawn>( MinActor);
}

float UPlayerSensing::DistSqr(AActor* want)
{
    return FVector::DistSquared2D(m_OwnedPlayer->GetActorLocation(),want->GetActorLocation());
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


AActor* UPlayerSensing::GetSensorActor() const
{
    return m_OwnedPlayer;
}

bool UPlayerSensing::IsSensorActor(const AActor* Actor) const
{
    return (Actor == GetSensorActor());
}