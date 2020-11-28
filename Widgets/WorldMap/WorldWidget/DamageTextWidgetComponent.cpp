#include "DamageTextWidgetComponent.h"
#include "DiabloM.h"


void UDamageTextWidgetComponent::EndAnimation()
{
    SetHiddenInGame(true);
    SetRelativeLocation(FVector(0,0,0));
}
