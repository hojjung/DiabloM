#include "DroppedItem.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/WorldMap/WorldWidget/ItemNameCard.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Components/BillboardComponent.h"
#include "Objs/Containers/QuadtreeNode.h"

ADroppedItem::ADroppedItem(const FObjectInitializer& objInit): Super(objInit)
{
    m_Imposter = CreateDefaultSubobject<UBillboardComponent>("ImposterTexture00");
    m_Imposter->SetupAttachment(RootComponent);
    m_Imposter->SetVisibility(true);
    m_Imposter->SetHiddenInGame(false);

    m_BillBoard->SetHiddenInGame(true);

    m_MeshComp->SetVisibility(false);
    m_MeshComp->SetHiddenInGame(true);

    m_CollSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    m_bIsOverlapAble = false;
    m_bIsDroppedInField = false;

}

void ADroppedItem::BeginPlay()
{
    Super::BeginPlay();

    m_BillBoard->InitWidget();

    if (!m_TableID.IsNull() && m_bIsDroppedInField)
    {
        FItemInstance Item = GetGameInstance<UDiabloGameInstance>()->CreateItem(m_TableID.RowName);

        SetItemInstance(Item);

        DropEnd();
    }
}


void ADroppedItem::Interact(AActor* instigator)
{
    if(m_ItemInstance.IsEmpty())
    {
        return;
    }
    
    APlayerDiabloCharacter* Player = Cast<APlayerDiabloCharacter>(instigator);

    if (!Player) //Monster Coll
    {
        return;
    }

    if (!Cast<ADiabloPlayerController>(Player->GetController())->PickUpItem(this))
    {
        return;
    }

    m_ItemInstance.ClearData(); //
    m_ItemInstance.SetGridNewIndex(-2);

    HideAll(true);

    if (GetCurrentNode())
    {
        GetCurrentNode()->RemoveElement(this);
    }

    if (m_OnTaskEnd.IsBound())
    {
        m_OnTaskEnd.Broadcast(this);
    }
    else
    {
        Destroy();
    }
}

void ADroppedItem::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                             const FHitResult& SweepResult)
{
    if (m_bIsOverlapAble)
    {
        Interact(OtherActor);
    }
}

void ADroppedItem::SetItemVisual(const FItemInstance& ItemData)
{
    UItemNameCard* ItemCard = Cast<UItemNameCard>(m_BillBoard->GetUserWidgetObject());

    ItemCard->SetItemName(ItemData.m_ItemData->m_ShowingName);

    ItemCard->SetItemColor(ItemData.m_ItemTier->m_TierColor);

    m_BillBoard->SetDrawSize(m_BillBoard->GetUserWidgetObject()->GetDesiredSize());

    OnItemVisualChange(GetItemColor());
}

void ADroppedItem::SetItemInstance(FItemInstance& itemInst)
{
    m_ItemInstance = itemInst;

    SetItemVisual(m_ItemInstance);

    m_TableID.RowName = m_ItemInstance.m_ItemID;

    m_BillBoard->SetHiddenInGame(true);
    SetActorHiddenInGame(false);
} 


const FItemInstance& ADroppedItem::GetCurrentItem() const
{
    return m_ItemInstance;
}

void ADroppedItem::DropEnd()
{
    if(GetCurrentItem().IsEmpty())
    {
        return;
    }

    m_bIsOverlapAble = GetCurrentItem().IsHighValue();

    RegisterToQuadTreeBound();
}

void ADroppedItem::RegisterToQuadTreeBound()
{
    Super::RegisterToQuadTreeBound();

    m_BillBoard->SetHiddenInGame(false);
}
