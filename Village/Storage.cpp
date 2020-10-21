// Fill out your copyright notice in the Description page of Project Settings.


#include "Storage.h"


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
     return  m_ItemSlotChanged;
 }
