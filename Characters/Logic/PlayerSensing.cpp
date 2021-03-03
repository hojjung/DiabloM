#include "PlayerSensing.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "WeakInterfacePtr.h"
#include "Characters/MonsterPawn.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/MonsterSpawnManager.h"

UPlayerSensing::UPlayerSensing()
{
	m_SightRadius = 1200.f;
	m_PeripheralVisionAngle = 50.f;
	m_PeripheralVisionCosine = FMath::Cos(FMath::DegreesToRadians(m_PeripheralVisionAngle));
	m_FocusRange = 500.f;
	m_SensingInterval = 0.3f;
}

void UPlayerSensing::InitSense(APlayerDiabloCharacter* player)
{
	m_OwnedPlayer = player;
	SetPeripheralVisionAngle(m_PeripheralVisionAngle);
	SetSensingUpdatesEnabled(true);
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

	UpdateAISensing();

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
	if(m_OwnedPlayer->GetFocusedTarget())
	{
		if(!m_OwnedPlayer->GetFocusedTarget()->IsAlive())
		{
			return;
		}
	}
	
	if (m_OwnedPlayer->GetIsManualMove())
	{
		return;
	}
	
	auto* Pawn = UDiabloGameInstance::Get->m_MonsterSpawn->GetNearestMonster(m_OwnedPlayer->GetActorLocation());

	if (m_OwnedPlayer->GetFocusedTarget() == Pawn)
	{
		return;
	}

	if (Pawn)
	{
		m_OwnedPlayer->FocusTarget(Pawn);
	}
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


float UPlayerSensing::DistSqr(AActor* want)
{
	return FVector::DistSquared2D(m_OwnedPlayer->GetActorLocation(), want->GetActorLocation());
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
	m_FocusRange = range;
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
