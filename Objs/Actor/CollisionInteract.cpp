#include "CollisionInteract.h"

#include "Managers/DiabloGameMode.h"
#include "Widgets/WorldMap/WorldWidget/ItemNameCard.h"


ACollisionInteract::ACollisionInteract(const FObjectInitializer& objInit)
{
	PrimaryActorTick.bCanEverTick = false;
	m_CollSphere = CreateDefaultSubobject<USphereComponent>("Coll00");
	m_CollSphere->SetCollisionProfileName("PickupItem");
	m_CollSphere->SetSphereRadius(70.f);
	RootComponent = m_CollSphere;
	//
	m_WidgetNameCard = CreateDefaultSubobject<UWidgetComponent>("NameCard03");
	m_WidgetNameCard->SetWidgetSpace(EWidgetSpace::Screen);
	m_WidgetNameCard->SetWidgetClass(UItemNameCard::StaticClass());
	m_WidgetNameCard->SetDrawAtDesiredSize(false);
	m_WidgetNameCard->SetGenerateOverlapEvents(false);
	m_WidgetNameCard->SetupAttachment(RootComponent);
	m_WidgetNameCard->SetRelativeLocation( FVector(0.f, 0.f, 70.f));
	m_WidgetNameCard->CastShadow = false;
	m_WidgetNameCard->SetCollisionProfileName("NoCollision");

	m_CurrentNode=nullptr;
	m_bIsVisible=true;
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

	if(!GetCurrentNode())
	{
		return;
	}
	if(GetCurrentNode()->IsPositionInsideNode(GetActorLocation()))
	{
		ShowAll(true);
	}
}

void ACollisionInteract::ShowAll(bool hasBeenShowed)
{
	if(m_bIsVisible)
	{
		return;	
	}
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	m_WidgetNameCard->SetComponentTickEnabled(true);
	m_bIsVisible=true;
}

void ACollisionInteract::HideAll(bool hasBeenShowed)
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	m_WidgetNameCard->SetComponentTickEnabled(false);
	m_bIsVisible=false;
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

