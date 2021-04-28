// My First Hack n Slash


#include "FloatingTextWidgetComponent.h"

#include "ConstructorHelpers.h"
#include "FloatingTextWidget.h"

UFloatingTextWidgetComponent::UFloatingTextWidgetComponent()
{
	// static ConstructorHelpers::FClassFinder<UUserWidget> FoundW(
	// 	TEXT("WidgetBlueprint'/Game/Blueprints/Widget/WorldWidget/WB_DamageText.WB_DamageText_C'"));
 //    
	// WidgetClass = FoundW.Class;

	SetDrawAtDesiredSize(true);
    
	SetWidgetSpace(EWidgetSpace::Screen);
}

void UFloatingTextWidgetComponent::SetFloatingText(const FText& ttWant)
{
	Cast<UFloatingTextWidget>( GetUserWidgetObject())->m_TextName->SetText(ttWant);
}
