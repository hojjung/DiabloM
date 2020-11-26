// Fill out your copyright notice in the Description page of Project Settings.


#include "DroppedGold.h"


#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Objs/Containers/QuadtreeNode.h"
#include "Widgets/WorldMap/WorldWidget/ItemNameCard.h"

ADroppedGold::ADroppedGold(const FObjectInitializer& objInit): Super(objInit)
{
    m_Format = FTextFormat::FromString("{0} Gold");
    m_MeshComp->SetHiddenInGame(true);
    m_MeshComp->SetVisibility(false);
    m_CollSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    m_BillBoard->SetHiddenInGame(true);
}

void ADroppedGold::SetGoldAmount(float amount)
{
    m_fGoldAmount = amount;
    m_BillBoard->SetHiddenInGame(true);
    SetActorHiddenInGame(false);
}

void ADroppedGold::DropEnd()
{
    UItemNameCard* ItemCard = Cast<UItemNameCard>(m_BillBoard->GetUserWidgetObject());
    FText GoldText = UDiaBlueprintFunctionLibrary::GetAlphabetText(m_fGoldAmount);
    FFormatOrderedArguments Args;
    Args.Add(GoldText);
    ItemCard->SetItemName(FText::Format(m_Format, Args));
    
    m_BillBoard->SetDrawSize(m_BillBoard->GetUserWidgetObject()->GetDesiredSize());
    RegisterToQuadTreeBound();
}

void ADroppedGold::RegisterToQuadTreeBound()
{
    Super::RegisterToQuadTreeBound();

    m_BillBoard->SetHiddenInGame(false);
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
