#include "DroppedItem.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/ItemNameCard.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"


// Sets default values
ADroppedItem::ADroppedItem(const FObjectInitializer& objInit):Super(objInit)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
	//
}

// Called when the game starts or when spawned
void ADroppedItem::BeginPlay()
{
	Super::BeginPlay();

	if (m_TableID != NAME_None)
	{
		SetItem(m_TableID);
	}
}

void ADroppedItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}


void ADroppedItem::PickupItem(AActor * interactCaster)
{
	if (!Cast<ADiabloPlayerController>(Cast<APlayerDiabloCharacter>(interactCaster)->GetController())->PickUpItem(this))
	{
		return;
	}
	m_ItemInstance.SetGridNewIndex(-2);
	SetActorHiddenInGame(true);
	Destroy();
}

void ADroppedItem::Interact(AActor * instigator)
{
	PickupItem(instigator);
}

void ADroppedItem::SetItem(FName itemID)
{
	const FItemData* ItemData = GetGameInstance<UDiabloGameInstance>()->GetItemData(itemID);

	auto* ItemCard = Cast<UItemNameCard>(m_BillBoard->GetUserWidgetObject());

	ItemCard->SetItemName(ItemData->m_ShowingName);

	ItemCard->SetItemColor(ItemData->GetItemTier().m_TierColor);

    m_MeshComp->SetStaticMesh(ItemData->m_ItemMesh);

	m_ItemInstance = GetGameInstance<UDiabloGameInstance>()->CreateItem(itemID);


	m_BillBoard->SetDrawSize(m_BillBoard->GetUserWidgetObject()->GetDesiredSize());
}

FItemInstance  ADroppedItem::GetItemInstance()
{
	return m_ItemInstance;;
}


