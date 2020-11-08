#include "Storage.h"

AStorage::AStorage()
{
    m_MeshStorage = CreateDefaultSubobject<UStaticMeshComponent>("Mesh00");
    m_MeshStorage->SetupAttachment(RootComponent);
}

void AStorage::RemoveItem(FItemInstance& itemWantErase)
{
    
}

void AStorage::RemoveItemByIndex(int index)
{
    
}

bool AStorage::AddItem(int droppedIndex, FItemInstance& itemWantAdd)
{
    return false;
}

bool AStorage::CheckSlotValid(int droppedIndex, FItemInstance& itemWantAdd)
{
    return false;
}

void AStorage::SetItem(int droppedIndex, FItemInstance& itemWantAdd)
{
}

bool AStorage::SwapMove(FItemInstance& Drop, FItemInstance& Drag)
{
    return false;
}

FOnItemSlotChanged& AStorage::GetItemChangeCallback()
{
    return m_ItemSlotChanged;
}
