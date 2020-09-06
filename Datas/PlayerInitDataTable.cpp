#include "PlayerInitDataTable.h"

#include "OpenGLDrv/Private/Android/AndroidOpenGL.h"

UDataTable* UPlayerInitDataTable::GetPlayerHairTable = nullptr;
UDataTable* UPlayerInitDataTable::GetPlayerFaceTable = nullptr;
UDataTable* UPlayerInitDataTable::GetPlayerArmorTable = nullptr;
UDataTable* UPlayerInitDataTable::GetPlayerItemTable = nullptr;
UDataTable* UPlayerInitDataTable::GetPlayerPerkTable = nullptr;

UPlayerInitDataTable::UPlayerInitDataTable()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable1(
              TEXT("DataTable'/Game/DataTables/PlayerCreation/PlayerHairTable.PlayerHairTable'"));
    UPlayerInitDataTable::GetPlayerHairTable = FoundTable1.Object;
    
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable2(
            TEXT("DataTable'/Game/DataTables/PlayerCreation/PlayerFaceTable.PlayerFaceTable'"));
    UPlayerInitDataTable::GetPlayerFaceTable = FoundTable2.Object;
    
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable3(
            TEXT("DataTable'/Game/DataTables/PlayerCreation/PlayerArmorTable.PlayerArmorTable'"));
    UPlayerInitDataTable::GetPlayerArmorTable = FoundTable3.Object;

    
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable5(
            TEXT("DataTable'/Game/DataTables/PlayerCreation/PlayerItemTable.PlayerItemTable'"));
    UPlayerInitDataTable::GetPlayerItemTable = FoundTable5.Object;
    
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable6(
            TEXT("DataTable'/Game/DataTables/PlayerCreation/PlayerPerkTable.PlayerPerkTable'"));
    UPlayerInitDataTable::GetPlayerPerkTable = FoundTable6.Object;
        
}

const FPlayerHairRow& UPlayerInitDataTable::GetPlayerHair(FName id)
{
    return *GetPlayerHairTable->FindRow<FPlayerHairRow>(id,"");
}

const FPlayerHairRow* UPlayerInitDataTable::GetPlayerHairPtr(FName id)
{
        return GetPlayerHairTable->FindRow<FPlayerHairRow>(id,"");
}

const FPlayerFaceRow& UPlayerInitDataTable::GetPlayerFace(FName id)
{
        return *GetPlayerFaceTable->FindRow<FPlayerFaceRow>(id,"");
}

const FPlayerFaceRow* UPlayerInitDataTable::GetPlayerFacePtr(FName id)
{
        return GetPlayerFaceTable->FindRow<FPlayerFaceRow>(id,"");
}

const FPlayerArmorRow& UPlayerInitDataTable::GetPlayerArmor(FName id)
{
        return *GetPlayerArmorTable->FindRow<FPlayerArmorRow>(id,"");
}

const FPlayerArmorRow* UPlayerInitDataTable::GetPlayerArmorPtr(FName id)
{
        return GetPlayerArmorTable->FindRow<FPlayerArmorRow>(id,"");
}



const FPlayerItemRow& UPlayerInitDataTable::GetPlayerItem(FName id)
{
        return *GetPlayerItemTable->FindRow<FPlayerItemRow>(id,"");
}

const FPlayerItemRow* UPlayerInitDataTable::GetPlayerItemPtr(FName id)
{
        return GetPlayerItemTable->FindRow<FPlayerItemRow>(id,"");
}

const FPlayerPerkRow& UPlayerInitDataTable::GetPlayerPerk(FName id)
{
        return *GetPlayerPerkTable->FindRow<FPlayerPerkRow>(id,"");
}

const FPlayerPerkRow* UPlayerInitDataTable::GetPlayerPerkPtr(FName id)
{
        return GetPlayerPerkTable->FindRow<FPlayerPerkRow>(id,"");
}
