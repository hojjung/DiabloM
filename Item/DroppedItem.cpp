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
    
    static ConstructorHelpers::FObjectFinder<UTexture2D> FoundImposter(
            TEXT("Texture2D'/Game/Sprite/Imposter/BagImposter.BagImposter'"));
    
    m_Imposter = CreateDefaultSubobject<UBillboardComponent>("ImposterTexture00");
    m_Imposter->SetupAttachment(RootComponent);
    m_Imposter->SetHiddenInGame(false);
    m_Imposter->ScreenSize=1.f;
    m_Imposter->bIsScreenSizeScaled=true;
    m_Imposter->Sprite=FoundImposter.Object;
    m_Imposter->SetRelativeLocation(FVector(0.f,0.f,30.f));
    m_Imposter->SetRelativeScale3D(FVector(0.45f,0.45f,0.45f));
    m_Imposter->SetReceivesDecals(false);
    m_Imposter->SetCastShadow(false);
    m_Imposter->bReceiveMobileCSMShadows=false;

    m_WidgetNameCard->SetHiddenInGame(true);
    
    static ConstructorHelpers::FClassFinder<UUserWidget> FoundWidgetNameCard(
            TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/WorldWidget/WB_ItemNamecard.WB_ItemNamecard_C'"));
    m_WidgetNameCard->SetWidgetClass(FoundWidgetNameCard.Class);

    m_CollSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    m_bIsOverlapAble = false;
    m_bIsDroppedInField = false;

}

void ADroppedItem::BeginPlay()
{
    Super::BeginPlay();

    m_WidgetNameCard->InitWidget();
    //m_BillBoard->SetHiddenInGame(true);
    //m_BillBoard->SetComponentTickEnabled(false);

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
    //m_BillBoard->SetDrawSize(FVector2D());
    
    UItemNameCard* ItemCard = Cast<UItemNameCard>(m_WidgetNameCard->GetUserWidgetObject());

    ItemCard->SetItemName(ItemData.m_ItemData->m_ShowingName);

    ItemCard->SetItemColor(ItemData.m_ItemTier->m_TierColor);

    ItemCard->ForceLayoutPrepass();

    m_WidgetNameCard->SetDrawSize(m_WidgetNameCard->GetUserWidgetObject()->GetDesiredSize());

    OnItemVisualChange(GetItemColor());
}

void ADroppedItem::SetItemInstance(FItemInstance& itemInst)
{
    m_ItemInstance = itemInst;

    SetItemVisual(m_ItemInstance);

    m_TableID.RowName = m_ItemInstance.m_ItemID;

    m_WidgetNameCard->SetHiddenInGame(true);
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

    m_WidgetNameCard->SetHiddenInGame(false);
}

void ADroppedItem::ShowAll(bool hasBeenShowed)
{
    if(m_bIsVisible)
    {
        return;	
    }
	
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    m_WidgetNameCard->SetComponentTickEnabled(true);
    m_bIsVisible=true;
    m_Imposter->SetComponentTickEnabled(true);
}

void ADroppedItem::HideAll(bool hasBeenShowed)
{
    if(!m_bIsVisible)
    {
        return;	
    }
    
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    m_WidgetNameCard->SetComponentTickEnabled(false);
    m_bIsVisible=false;
    m_Imposter->SetComponentTickEnabled(false);
}

