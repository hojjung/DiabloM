// Fill out your copyright notice in the Description page of Project Settings.


#include "DroppedGold.h"


#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Components/BillboardComponent.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Objs/Containers/QuadtreeNode.h"
#include "Widgets/WorldMap/WorldWidget/ItemNameCard.h"

ADroppedGold::ADroppedGold(const FObjectInitializer& objInit): Super(objInit)
{
    PrimaryActorTick.bCanEverTick=false;
    
    static ConstructorHelpers::FObjectFinder<UTexture2D> FoundImposter(
          TEXT("Texture2D'/Game/Sprite/Imposter/CoinBillboard.CoinBillboard'"));
    
    m_Imposter = CreateDefaultSubobject<UBillboardComponent>("ImposterTexture00");
    m_Imposter->SetupAttachment(RootComponent);
    m_Imposter->SetHiddenInGame(false);
    m_Imposter->ScreenSize=1.f;
    m_Imposter->bIsScreenSizeScaled=true;
    m_Imposter->Sprite=FoundImposter.Object;
    m_Imposter->SetRelativeLocation(FVector(0.f,0.f,30.f));
    m_Imposter->SetRelativeScale3D(FVector(0.55f,0.55f,0.55f));
    m_Imposter->SetReceivesDecals(false);
    m_Imposter->SetCastShadow(false);
    m_Imposter->bReceiveMobileCSMShadows=false;

    m_WidgetNameCard->SetHiddenInGame(true);
    
    static ConstructorHelpers::FClassFinder<UUserWidget> FoundWidgetNameCard(
            TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/WorldWidget/WB_ItemNamecard.WB_ItemNamecard_C'"));
    m_WidgetNameCard->SetWidgetClass(FoundWidgetNameCard.Class);

    
    m_Format = FTextFormat::FromString("{0} Gold");
    m_CollSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    m_WidgetNameCard->SetHiddenInGame(true);
}

void ADroppedGold::SetGoldAmount(float amount)
{
    m_fGoldAmount = amount;
    m_WidgetNameCard->SetHiddenInGame(true);
    SetActorHiddenInGame(false);
}

void ADroppedGold::DropEnd()
{
    UItemNameCard* ItemCard = Cast<UItemNameCard>(m_WidgetNameCard->GetUserWidgetObject());
    FText GoldText = UDiaBlueprintFunctionLibrary::GetAlphabetText(m_fGoldAmount);
    FFormatOrderedArguments Args;
    Args.Add(GoldText);
    ItemCard->SetItemName(FText::Format(m_Format, Args));
    
    m_WidgetNameCard->SetDrawSize(m_WidgetNameCard->GetUserWidgetObject()->GetDesiredSize());
    RegisterToQuadTreeBound();
}

void ADroppedGold::RegisterToQuadTreeBound()
{
    Super::RegisterToQuadTreeBound();

    m_WidgetNameCard->SetHiddenInGame(false);
}

void ADroppedGold::BeginPlay()
{
    Super::BeginPlay();

    if (m_fGoldAmount > 0.f)
    {
        SetGoldAmount(m_fGoldAmount);
        DropEnd();
    }
}

void ADroppedGold::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                             const FHitResult& SweepResult)
{
    APlayerDiabloCharacter* Char = Cast<APlayerDiabloCharacter>(OtherActor);

    if (!Char)
    {
        return;
    }


    float AmountBounus = Cast<UPlayerDiabloAttribute>(Char->GetAttributeSet())->GetGoldBonusPer();

    if (AmountBounus < 1.f)
    {
        AmountBounus = 1.f;
    }

    Char->EarnGold(m_fGoldAmount * AmountBounus);

    HideAll(true);

    if (GetCurrentNode())
    {
        GetCurrentNode()->RemoveElement(this);
    }

    if (m_OnTaskEnd.IsBound()) //Pooled
    {
        m_OnTaskEnd.Broadcast(this);
    }
    else //Not Pooled
    {
        Destroy();
    }
}
