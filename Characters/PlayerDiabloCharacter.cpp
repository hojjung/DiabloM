#include "PlayerDiabloCharacter.h"
#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Managers/DiabloGameInstance.h"
#include "Datas/CharacterDataTable.h"
#include "Characters/DiabloPlayerController.h"
#include "Camera/CameraDissolve.h"
#include "Objs/Interfaces/Interactable.h"

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
	//
	CreateSkMeshComponent(&m_SkFace,"SkMesh01");
	CreateSkMeshComponent(&m_SkHair,"SkMesh02");
	CreateSkMeshComponent(&m_SkBelt,"SkMesh03");
	CreateSkMeshComponent(&m_SkGlove,"SkMesh04");
	CreateSkMeshComponent(&m_SkShoe,"SkMesh05");
	CreateSkMeshComponent(&m_SkShoulderPad,"SkMesh06");
	CreateSkMeshComponent(&m_SkHeadGear,"SkMesh07");
}


void APlayerDiabloCharacter::SetUnit(FName unitID)
{
	m_NameUnitID = unitID;

	const FPlayerEntityTable* const UnitData = GetGameInstance<UDiabloGameInstance>()->GetPlayerUnitPtr(m_NameUnitID);
	//	
	m_CachedHairMesh = UnitData->m_SkHair;
	m_CachedBodyMesh = UnitData->m_Mesh;
	//
	SetBodyMeshToCached();
	m_SkMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	m_SkMesh->SetAnimInstanceClass(UnitData->m_AnimBP);
	//
	m_SkFace->SetMasterPoseComponent(m_SkMesh);
	m_SkHair->SetMasterPoseComponent(m_SkMesh);
	m_SkGlove->SetMasterPoseComponent(m_SkMesh);
	m_SkShoe->SetMasterPoseComponent(m_SkMesh);
	m_SkHeadGear->SetMasterPoseComponent(m_SkMesh);
	m_SkShoulderPad->SetMasterPoseComponent(m_SkMesh);
	m_SkBelt->SetMasterPoseComponent(m_SkMesh);
	//
	m_SkFace->SetSkeletalMesh(UnitData->m_SkFace);
	SetHairMeshToCached();
	//
	FGameplayEffectContextHandle EffectContext = m_AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = m_AbilitySystemComponent->MakeOutgoingSpec(UnitData->m_DefaultStatTable, GetLevel(), EffectContext);

	if (!NewHandle.IsValid())
	{
		PRINTF("Invalid Handle");
	}

	FActiveGameplayEffectHandle ActiveGEHandle = m_AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), m_AbilitySystemComponent);

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
		m_FocusedInteractable.SetInterface(nullptr);
		m_FocusedInteractable.SetObject(nullptr);
		return;
	}


	IInteractable* FoundIntract = Cast<IInteractable>(OutHit.GetActor());

	if (!FoundIntract)
	{
		if (m_FocusedInteractable)
		{
			m_FocusedInteractable.SetInterface(nullptr);
			m_FocusedInteractable.SetObject(nullptr);
		}

		return;
	}

	if (!m_FocusedInteractable)
	{

		m_FocusedInteractable.SetInterface(FoundIntract);
		m_FocusedInteractable.SetObject(OutHit.GetActor());

		OnInteractFound();

		return;
	}

	if (m_FocusedInteractable != FoundIntract)
	{
		m_FocusedInteractable.SetInterface(FoundIntract);
		m_FocusedInteractable.SetObject(OutHit.GetActor());

		OnInteractFound();

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
	PRINTF("Attacking");
}

void APlayerDiabloCharacter::SetLoadedData(const USaveCharacterStatus* loadedSaveData)
{
	m_nCharacterLevel=loadedSaveData->m_nLevel;
	
}


void APlayerDiabloCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	m_PlayerCon = Cast<ADiabloPlayerController>(GetController());
	InputComponent->BindAxis("MoveForward", this, &AUnitPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AUnitPawn::MoveRight);
	PlayerInputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this, &APlayerDiabloCharacter::InteractWithTarget);
}

void APlayerDiabloCharacter::SetBodyMeshToCached()
{
	m_SkMesh->SetSkeletalMesh(m_CachedBodyMesh);
}

void APlayerDiabloCharacter::SetHairMeshToCached()
{
	m_SkHair->SetSkeletalMesh(m_CachedHairMesh);
}
