#include "DroppedItem.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/WorldMap/WorldWidget/ItemNameCard.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Components/BillboardComponent.h"
#include "Objs/Containers/QuadtreeNode.h"

ADroppedItem::ADroppedItem(const FObjectInitializer& objInit): Super(objInit)
{
    PrimaryActorTick.bCanEverTick=false;
    
    static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(
            TEXT("StaticMesh'/Game/Models/Props/Containers/MeshSack.MeshSack'"));
    
    m_MeshSack = CreateDefaultSubobject<UStaticMeshComponent>("MeshSack");
    m_MeshSack->SetupAttachment(RootComponent);
    m_MeshSack->SetRelativeLocation(FVector(0.f,0.f,30.f));
    m_MeshSack->SetRelativeScale3D(FVector(2.f,2.f,2.f));
    m_MeshSack->SetReceivesDecals(false);
    m_MeshSack->SetCastShadow(false);
    m_MeshSack->bReceiveMobileCSMShadows=false;
    m_MeshSack->SetStaticMesh(FoundMesh.Object);
    m_MeshSack->SetCollisionProfileName("NoCollision");
    m_MeshSack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    

    static ConstructorHelpers::FClassFinder<UUserWidget> FoundWidgetNameCard(
            TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/WorldWidget/WB_ItemNamecard.WB_ItemNamecard_C'"));
    m_WidgetNameCard->SetWidgetClass(FoundWidgetNameCard.Class);

    m_CollSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    m_CollSphere->SetSphereRadius(140);
    
    m_bIsDroppedInField = false;

    m_bIsDropEnd=false;

    m_WidgetNameCard->SetVisibility(false);

    m_WidgetNameCard->SetRelativeLocation( FVector(0.f, 0.f, 140.f));
}

void ADroppedItem::BeginPlay()
{
    Super::BeginPlay();

    m_WidgetNameCard->InitWidget();

    if (!m_TableID.IsNull() && m_bIsDroppedInField)
    {
        FItemInstance Item = GetGameInstance<UDiabloGameInstance>()->CreateItem(m_TableID.RowName);

        SetItemInstance(Item);

        DropEnd();
    }
}


void ADroppedItem::SetItemInstance(FItemInstance& itemInst)
{
    m_ItemInstance = itemInst;

    SetItemVisual(m_ItemInstance);

    m_TableID.RowName = m_ItemInstance.m_ItemID;

    //
    SetActorHiddenInGame(false);
    m_MeshSack->SetHiddenInGame(false);
    m_WidgetNameCard->SetVisibility(false);

    m_bIsOverlapAble = false;

    m_bIsDropEnd = false;
} 

void ADroppedItem::SetItemVisual(const FItemInstance& ItemData)
{
    //m_BillBoard->SetDrawSize(FVector2D());
    
    UItemNameCard* ItemCard = Cast<UItemNameCard>(m_WidgetNameCard->GetUserWidgetObject());

    ItemCard->SetItemName(ItemData.m_ItemData->m_ShowingName);

    ItemCard->SetItemColor(ItemData.m_ItemTier->m_TierColor);

    ItemCard->ForceLayoutPrepass();

    m_WidgetNameCard->SetDrawSize(m_WidgetNameCard->GetUserWidgetObject()->GetDesiredSize());

    OnItemVisualChange(GetItemColor());
}

void ADroppedItem::Interact(AActor* instigator)
{
    if(!m_bIsDropEnd)
    {
        return;
    }
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

    HideAll();

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
    if(!m_bIsDropEnd)
    {
        return;
    }
    if (m_bIsOverlapAble)
    {
        Interact(OtherActor);
    }
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

    m_bIsDropEnd = true;
    
    RegisterToQuadTreeBound();

    PlaySound();
}

void ADroppedItem::ShowAll()
{
    if(m_bIsVisible)
    {
        return;	
    }
	
    SetActorEnableCollision(true);

    SetActorHiddenInGame(false);
	
    m_WidgetNameCard->SetVisibility(true);

    m_MeshSack->SetHiddenInGame(false);
    
    m_MeshSack->SetComponentTickEnabled(true);
    
    m_bIsVisible=true;
}

void ADroppedItem::HideAll( )
{
    if(!m_bIsVisible)
    {
        return;	
    }
    
    SetActorEnableCollision(false);
    
    SetActorHiddenInGame(true);
	
    m_WidgetNameCard->SetVisibility(false);
	
    m_MeshSack->SetHiddenInGame(true);

    m_MeshSack->SetComponentTickEnabled(false);
    
    m_bIsVisible=false;
}

