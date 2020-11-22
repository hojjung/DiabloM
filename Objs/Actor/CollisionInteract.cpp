#include "CollisionInteract.h"

#include "Managers/DiabloGameMode.h"
#include "Widgets/WorldMap/WorldWidget/ItemNameCard.h"


ACollisionInteract::ACollisionInteract(const FObjectInitializer& objInit)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bCanEverTick = false;
	m_CollSphere = CreateDefaultSubobject<USphereComponent>("Coll00");
	m_CollSphere->SetCollisionProfileName("PickupItem");
	m_CollSphere->SetupAttachment(m_MeshComp);
	m_CollSphere->SetSphereRadius(70.f);
	RootComponent = m_CollSphere;
	//
	m_MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh02");
	m_MeshComp->SetCollisionProfileName("NoCollision");
	m_MeshComp->SetGenerateOverlapEvents(true);
	m_MeshComp->CastShadow = false;
	m_MeshComp->SetupAttachment(RootComponent);
	//
	m_BillBoard = CreateDefaultSubobject<UWidgetComponent>("Billboard03");
	m_BillBoard->SetWidgetSpace(EWidgetSpace::Screen);
	m_BillBoard->SetWidgetClass(UItemNameCard::StaticClass());
	m_BillBoard->SetDrawAtDesiredSize(false);
	m_BillBoard->SetGenerateOverlapEvents(false);
	m_BillBoard->SetupAttachment(RootComponent);
	m_BillBoard->SetRelativeLocation( FVector(0.f, 0.f, 70.f));
	m_BillBoard->CastShadow = false;
}

void ACollisionInteract::BeginPlay()
{
	Super::BeginPlay();
	m_CollSphere->OnComponentBeginOverlap.AddDynamic(this,&ACollisionInteract::OnOverlap);
	RegisterToQuadTreeBound();
}
void ACollisionInteract::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

FVector ACollisionInteract::GetActorLocation()
{
	return AActor::GetActorLocation();
}

void ACollisionInteract::RegisterToQuadTreeBound()
{
	ADiabloGameMode::Get->RegisterQuadElement(this);
	HideAll(false);

}

void ACollisionInteract::ShowAll(bool hasBeenShowed)
{
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
}

void ACollisionInteract::HideAll(bool hasBeenShowed)
{
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
}


void ACollisionInteract::Interact(AActor* instigator)
{
	
}

