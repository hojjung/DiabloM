#include "DroppedGold.h"
#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Components/BillboardComponent.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Objs/Containers/QuadtreeNode.h"
#include "Widgets/WorldMap/WorldWidget/ItemNameCard.h"

ADroppedGold::ADroppedGold(const FObjectInitializer& objInit): Super(objInit)
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UTexture2D> FoundImposter(
		TEXT("Texture2D'/Game/Sprite/Imposter/CoinBillboard.CoinBillboard'"));

	m_Imposter = CreateDefaultSubobject<UBillboardComponent>("ImposterTexture00");
	m_Imposter->SetupAttachment(RootComponent);
	m_Imposter->ScreenSize = 1.f;
	m_Imposter->bIsScreenSizeScaled = true;
	m_Imposter->Sprite = FoundImposter.Object;
	m_Imposter->SetRelativeLocation(FVector(0.f, 0.f, 30.f));
	m_Imposter->SetRelativeScale3D(FVector(0.55f, 0.55f, 0.55f));
	m_Imposter->SetReceivesDecals(false);
	m_Imposter->SetCastShadow(false);
	m_Imposter->bReceiveMobileCSMShadows = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> FoundWidgetNameCard(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/WorldWidget/WB_ItemNamecard.WB_ItemNamecard_C'"));
	m_WidgetNameCard->SetWidgetClass(FoundWidgetNameCard.Class);

	m_Format = FTextFormat::FromString("{0} Gold");
	m_CollSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	m_WidgetNameCard->SetVisibility(false);
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

void ADroppedGold::SetGoldAmount(float amount)
{
	m_fGoldAmount = amount;
	
	SetActorHiddenInGame(false);
	m_Imposter->SetHiddenInGame(false);
	m_WidgetNameCard->SetVisibility(false);
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


void ADroppedGold::ShowAll(bool hasBeenShowed)
{
	if (m_bIsVisible)
	{
		return;
	}
	
	SetActorEnableCollision(true);

	SetActorHiddenInGame(false);
	
	m_WidgetNameCard->SetVisibility(true);
	
	m_Imposter->SetHiddenInGame(false);

	m_Imposter->SetComponentTickEnabled(true);
	
	m_bIsVisible = true;
}

void ADroppedGold::HideAll(bool hasBeenShowed)
{
	if(!m_bIsVisible)
	{
		return;	
	}
	
	SetActorEnableCollision(false);
    
	SetActorHiddenInGame(true);
	
	
	m_Imposter->SetHiddenInGame(true);

	//m_WidgetNameCard->SetComponentTickEnabled(false);
	//이유는 모르겠는데 틱을 호출하니까 네임카드가 사라지지않음
	
	m_WidgetNameCard->SetVisibility(false);
	
	m_Imposter->SetComponentTickEnabled(false);
	
	m_bIsVisible=false;
}

void ADroppedGold::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                             const FHitResult& SweepResult)
{
	APlayerDiabloCharacter* Char = Cast<APlayerDiabloCharacter>(OtherActor);
    
	if (Char)
	{
		Interact(Char);
	}

}

void ADroppedGold::Interact(AActor* instigator)
{
	APlayerDiabloCharacter* Char = Cast<APlayerDiabloCharacter>(instigator);

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
