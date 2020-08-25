// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataTable.h"
UDataTable* UItemDataTable::m_OptionTable=nullptr;
UDataTable* UItemDataTable::m_TierTable=nullptr;
UDataTable* UItemDataTable::m_DefaultItemTable=nullptr;
UItemDataTable::UItemDataTable(const FObjectInitializer& objInit):Super(objInit)
{
        static ConstructorHelpers::FObjectFinder<UDataTable> FoundOptionTable(TEXT("DataTable'/Game/DataTables/EquipOptionTable.EquipOptionTable'"));
        UItemDataTable::m_OptionTable = FoundOptionTable.Object;

        static ConstructorHelpers::FObjectFinder<UDataTable> FoundTierTable(TEXT("DataTable'/Game/DataTables/TierTable.TierTable'"));
        UItemDataTable::m_TierTable = FoundTierTable.Object;

        static ConstructorHelpers::FObjectFinder<UDataTable> FoundItemTable(TEXT("DataTable'/Game/DataTables/DefaultItemTable.DefaultItemTable'"));
        UItemDataTable::m_DefaultItemTable = FoundItemTable.Object;


}
