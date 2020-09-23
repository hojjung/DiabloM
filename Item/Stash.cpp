// Fill out your copyright notice in the Description page of Project Settings.


#include "Stash.h"

 void UStash::RemoveItem(FItemInstance& itemWantErase)
{
}

 void UStash::RemoveItemByIndex(int index)
{
}

 bool UStash::AddItem(int droppedIndex, FItemInstance& itemWantAdd)
{
  return false;
}

 bool UStash::CheckSlotValid(int droppedIndex, FItemInstance& itemWantAdd)
{
  return false;
}

 void UStash::SetItem(int droppedIndex, FItemInstance& itemWantAdd)
{
}

 bool UStash::SwapMove(FItemInstance& Drop, FItemInstance& Drag)
{
  return false;
}

 FOnItemSlotChanged& UStash::GetItemChangeCallback()
{
    return  m_ItemSlotChanged;
}
