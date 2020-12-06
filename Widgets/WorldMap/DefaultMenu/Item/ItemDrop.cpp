#include "ItemDrop.h"

#include "DiaDragDrop.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"


bool UItemDrop::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                             UDragDropOperation* InOperation)
{
    Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

    UDiaDragDrop* DDO =Cast<UDiaDragDrop>( InOperation);
    
    if(!DDO)
    {
        return  false;
    }

    ThrowItem(DDO->m_DraggedItem);

    return  true;
}

void UItemDrop::ThrowItem(const FItemInstance& itemThrow)
{
    PRINTF("ThrowItem");

    itemThrow.m_Holder->RemoveItemByIndex(itemThrow.m_nGridIndex);
    	FItemInstance item =itemThrow;
    	GetGameInstance<UDiabloGameInstance>()->DropItemActor(ADiabloPlayerController::Get->GetPlayerPawn(),250.f,item);
}
