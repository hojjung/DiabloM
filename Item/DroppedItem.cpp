#include "DroppedItem.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/WorldMap/WorldWidget/ItemNameCard.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"

void ADroppedItem::BeginPlay()
{
	Super::BeginPlay();

	if (!m_TableID.IsNull())
	{
		SetItem(m_TableID.RowName);
		
		auto AA = m_ItemInstance.m_AryOptions;

		//PRINTF("Ground CreatedItem: %s",*m_ItemInstance.m_ItemData->m_ShowingName.ToString());
		
		for(auto BB :AA)
		{
			//PRINTF("Option:%s", *BB.GetOptionText().ToString());
		}
		
	}
}


void ADroppedItem::Interact(AActor * instigator)
{
	if (!Cast<ADiabloPlayerController>(Cast<APlayerDiabloCharacter>(instigator)->GetController())->PickUpItem(this))
	{
		return;
	}
	
	m_ItemInstance.SetGridNewIndex(-2);
	SetActorHiddenInGame(true);
	Destroy();
}

void ADroppedItem::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FName Tier=GetCurrentItem().m_ItemTier->m_TierID;
	
	if(Tier=="Set"||Tier=="Immortal"||Tier=="Legendary"||Tier=="Epic")
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


