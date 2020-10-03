#include "ItemDrop.h"
#include "Widgets/WorldMap/DefaultMenu/DiaDragDrop.h"
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
    //드랍한순간
    //아이템을 액터로 생성
    //플레이어 주위에 랜덤 위치
    //해당 아이템 인벤에서 삭제
    PRINTF("ThrowItem");

    itemThrow.m_Holder->RemoveItemByIndex(itemThrow.m_nGridIndex);
    	FItemInstance item =itemThrow;
    	GetGameInstance<UDiabloGameInstance>()->DropItemActor(item);
}
