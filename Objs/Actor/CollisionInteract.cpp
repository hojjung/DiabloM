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

	m_CurrentNode=nullptr;
}

void ACollisionInteract::BeginPlay()
{
	Super::BeginPlay();
	m_CollSphere->OnComponentBeginOverlap.AddDynamic(this,&ACollisionInteract::OnOverlap);
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
}

void ACollisionInteract::ShowAll(bool hasBeenShowed)
{
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
}

void ACollisionInteract::HideAll(bool hasBeenShowed)
{
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
}

void ACollisionInteract::SetNode(QuadtreeNode* quadtree_node)
{
	m_CurrentNode=quadtree_node;
}

QuadtreeNode* ACollisionInteract::GetCurrentNode()
{
	return m_CurrentNode;
}


void ACollisionInteract::Interact(AActor* instigator)
{
	
}

