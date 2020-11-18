// My First Hack n Slash


#include "HoverToggle.h"

void UHoverToggle::NativePreConstruct()
{
    Super::NativePreConstruct();

    m_Toggle->WidgetStyle=m_WidgetStyle;
}

void UHoverToggle::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                     UDragDropOperation* InOperation)
{
    Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
    m_Toggle->SetCheckedState(ECheckBoxState::Checked);
}

void UHoverToggle::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragLeave(InDragDropEvent, InOperation);

    //m_Toggle->SetCheckedState(ECheckBoxState::Unchecked);
}
