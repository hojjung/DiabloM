#include "PlayerDiabloCharacter.h"
#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Managers/DiabloGameInstance.h"
#include "Datas/CharacterDataTable.h"
#include "Characters/DiabloPlayerController.h"
#include "Camera/CameraDissolve.h"

APlayerDiabloCharacter::APlayerDiabloCharacter(const FObjectInitializer& objInit)
	:Super(objInit.SetDefaultSubobjectClass<UPlayerDiabloAttribute>("AttributeSet00"))
{
	
	m_DissolveCam = CreateDefaultSubobject<UCameraDissolve>("CamDissolve00");
	m_DissolveCam->SetupAttachment(RootComponent);
	m_DissolveCam->SetRelativeRotation(FRotator(-50.f, 0.f, 0.f));
	//
	m_TopCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera00");
	m_TopCamera->SetupAttachment(m_DissolveCam);

	m_FocusedInteractable = nullptr;

	m_fInteractRange = 300.f;
}


void APlayerDiabloCharacter::BeginPlay()
{
	Super::BeginPlay();
	m_PlayerCon = Cast<ADiabloPlayerController>( GetController());
}

void APlayerDiabloCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


void APlayerDiabloCharacter::TryCheckInteractable()
{
	FVector TraceStart = m_SkMesh->GetComponentLocation();
	FVector TraceEnd = TraceStart + GetCapsule()->GetForwardVector()*m_fInteractRange;

	FHitResult OutHit;
	TArray<AActor*> IgnoreActors;//like weapon or ally

	if (!UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		TraceStart, TraceEnd, 15.f,
		ETraceTypeQuery::TraceTypeQuery3, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHit, true)
		|| !OutHit.GetActor())
	{
		m_FocusedInteractable = nullptr;
		return;
	}


	IInteractable* FoundIntract = Cast<IInteractable>(OutHit.GetActor());

	if (!FoundIntract)
	{
		if (m_FocusedInteractable)
		{
			m_FocusedInteractable = nullptr;
		}

		return;
	}

	if (!m_FocusedInteractable)
	{
		m_FocusedInteractable = FoundIntract;

		OnInteractFound();

		return;
	}

	if (m_FocusedInteractable != FoundIntract)
	{
		m_FocusedInteractable = FoundIntract;

		OnInteractFound();

		return;
	}


}

void APlayerDiabloCharacter::InteractWithTarget()
{
	if (!m_FocusedInteractable)
		return;

	m_FocusedInteractable->Interact(this);
	m_FocusedInteractable = nullptr;
}

void APlayerDiabloCharacter::OnInteractFound()//npc,item , its not monster
{
	PRINTF("Found Something");
}

void APlayerDiabloCharacter::AutoPlayTick()
{
}

void APlayerDiabloCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TryCheckInteractable();
}


void APlayerDiabloCharacter::AttackInput(float pressed)
{
}


void APlayerDiabloCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	m_PlayerCon = Cast<ADiabloPlayerController>(GetController());
	InputComponent->BindAxis("MoveForward", this, &AUnitPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AUnitPawn::MoveRight);
	PlayerInputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this, &APlayerDiabloCharacter::InteractWithTarget);
}
