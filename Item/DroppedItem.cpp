#include "DroppedItem.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/WorldMap/WorldWidget/ItemNameCard.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"

ADroppedItem::ADroppedItem(const FObjectInitializer& objInit):Super(objInit)
{
	m_ParticleEffect=CreateDefaultSubobject<UParticleSystemComponent>("ParticleEffect00");
	m_ParticleEffect->SetupAttachment(RootComponent);
	m_BillBoard->SetHiddenInGame(true);
	m_CollSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADroppedItem::BeginPlay()
{
	Super::BeginPlay();

	if (!m_TableID.IsNull())
	{
		SetItem(m_TableID.RowName);
		
		DropEnd();
		
	}
}


void ADroppedItem::Interact(AActor * instigator)
{
	APlayerDiabloCharacter* Player =Cast<APlayerDiabloCharacter>(instigator);

	if(!Player)//Monster Coll
	{
		return;
	}
	
	if (!Cast<ADiabloPlayerController>(Player->GetController())->PickUpItem(this))
	{
		return;
	}
	
	m_ItemInstance.ClearData();//
	m_ItemInstance.SetGridNewIndex(-2);

	m_BillBoard->SetHiddenInGame(true);
	m_CollSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if(m_OnTaskEnd.IsBound())
	{
		m_OnTaskEnd.Broadcast(this);
	}
	else
	{
		Destroy();
	}
}

void ADroppedItem::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FName Tier=GetCurrentItem().m_ItemTier->m_TierID;
	
	if(Tier=="Set"||Tier=="Immortal"||Tier=="Legend"||Tier=="Epic")
	{
		Interact(OtherActor);
	}
}

void ADroppedItem::SetItemVisual(const FItemInstance& ItemData)
{
	UItemNameCard* ItemCard = Cast<UItemNameCard>(m_BillBoard->GetUserWidgetObject());

	ItemCard->SetItemName(ItemData.m_ItemData->m_ShowingName);

	ItemCard->SetItemColor(ItemData.m_ItemTier->m_TierColor);

	m_MeshComp->SetStaticMesh(ItemData.m_ItemData->m_ItemMesh);

	m_BillBoard->SetDrawSize(m_BillBoard->GetUserWidgetObject()->GetDesiredSize());

	OnItemVisualChange(GetItemColor());
}

void ADroppedItem::SetItem(FName itemID)
{
	m_ItemInstance = GetGameInstance<UDiabloGameInstance>()->CreateItem(itemID);

	SetItemVisual(m_ItemInstance);
}

void ADroppedItem::SetItemInstance(FItemInstance& itemInst)
{
	m_ItemInstance=itemInst;

	SetItemVisual(m_ItemInstance);

	m_TableID.RowName=itemInst.m_ItemID;
}


const FItemInstance& ADroppedItem::GetCurrentItem()const
{
	return m_ItemInstance;
}

void ADroppedItem::DropEnd()
{
	SetActorHiddenInGame(false);
	m_ParticleEffect->Activate(true);
	m_BillBoard->SetHiddenInGame(false);
	m_CollSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}


