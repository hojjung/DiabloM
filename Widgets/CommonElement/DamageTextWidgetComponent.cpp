#include "DamageTextWidgetComponent.h"
#include "DiabloM.h"


UDamageTextWidgetComponent::UDamageTextWidgetComponent()
{
    //WidgetClass
    static ConstructorHelpers::FClassFinder<UUserWidget> FoundW(
    TEXT("WidgetBlueprint'/Game/Blueprints/Widget/WorldWidget/WB_DamageText.WB_DamageText_C'"));
    
    WidgetClass = FoundW.Class;

    SetDrawAtDesiredSize(true);
    
    SetWidgetSpace(EWidgetSpace::Screen);
}

void UDamageTextWidgetComponent::EndAnimation()
{
    SetHiddenInGame(true);
    SetRelativeLocation(FVector(0,0,0));
}
