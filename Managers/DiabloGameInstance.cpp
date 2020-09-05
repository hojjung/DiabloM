#include "DiabloGameInstance.h"
#include "ConstructorHelpers.h"
#include "StartMap/PlayerCreateManager.h"


UDiabloGameInstance* UDiabloGameInstance::Get = nullptr;

UDiabloGameInstance::UDiabloGameInstance()
{
    UDiabloGameInstance::Get = this;

    m_ItemManager=nullptr;

    m_SaveLoadManager=nullptr;
    
    m_PlCreateManager=nullptr;

}

void UDiabloGameInstance::Init()
{
    Super::Init();
    
    if (!m_ItemManager)
    {
        m_ItemManager = new  ItemManager();
        m_ItemManager->Init(this);

    }
    if (!m_SaveLoadManager)
    {
        m_SaveLoadManager = new SaveLoadManager();
        m_SaveLoadManager->CreateSaveLoadInstance();

    }

    if(!m_PlCreateManager)
    {
        m_PlCreateManager = new PlayerCreateManager();
        m_PlCreateManager->Init(this);
        //m_PlCreateManager->CreateSaveLoadInstance();
    }
}

void UDiabloGameInstance::Shutdown()
{
    Super::Shutdown();

    delete m_ItemManager;
    delete m_SaveLoadManager;
}

#pragma region  DataGetter


const FEntityTable* UDiabloGameInstance::GetMonsterUnitPtr(FName id) const
{
    return UCharacterDataTable::GetMonsterPtr(id);
}



const FNPCEntityTable* UDiabloGameInstance::GetNPCUnitPtr(FName id) const
{
    return UCharacterDataTable::GetNPCPtr(id);
}

const FPlayerEntityTable* UDiabloGameInstance::GetPlayerUnitPtr(FName id) const
{
    return UCharacterDataTable::GetPlayerEntityPtr(id);
}

const FItemTier* UDiabloGameInstance::GetItemTierPtr(FName id) const
{
    return UItemDataTable::GetItemTierPtr(id);
}

const FItemData* UDiabloGameInstance::GetItemDataPtr(FName id) const
{
    return UItemDataTable::GetItemDataPtr(id);
}

const FOption* UDiabloGameInstance::GetOptionPtr(FName id) const
{
    return UOptionDataTable::GetOptionPtr(id);
}

const FPlayerHairRow* UDiabloGameInstance::GetPlayerHairPtr(FName id) const
{
    return UPlayerInitDataTable::GetPlayerHairPtr(id);
}

const FPlayerFaceRow* UDiabloGameInstance::GetPlayerFacePtr(FName id) const
{
    return UPlayerInitDataTable::GetPlayerFacePtr(id);
}

const FPlayerArmorRow* UDiabloGameInstance::GetPlayerArmorPtr(FName id) const
{
    return UPlayerInitDataTable::GetPlayerArmorPtr(id);
}

const FPlayerWeaponRow* UDiabloGameInstance::GetPlayerWeaponPtr(FName id) const
{
    return UPlayerInitDataTable::GetPlayerWeaponPtr(id);
}

const FPlayerItemRow* UDiabloGameInstance::GetPlayerItemPtr(FName id) const
{
    return UPlayerInitDataTable::GetPlayerItemPtr(id);
}

const FPlayerPerkRow* UDiabloGameInstance::GetPlayerPerkPtr(FName id) const
{
    return UPlayerInitDataTable::GetPlayerPerkPtr(id);
}

const FEntityTable& UDiabloGameInstance::GetMonsterUnit(FName id) const
{
    return UCharacterDataTable::GetMonster(id);
}

const FNPCEntityTable& UDiabloGameInstance::GetNPCUnit(FName id) const
{
    return UCharacterDataTable::GetNPC(id);
}

const FPlayerEntityTable& UDiabloGameInstance::GetPlayerUnit(FName id) const
{
    return UCharacterDataTable::GetPlayerEntity(id);
}

const FItemTier& UDiabloGameInstance::GetItemTier(FName id) const
{
    return UItemDataTable::GetItemTier(id);
}

const FItemData& UDiabloGameInstance::GetItemData(FName id) const
{
    return UItemDataTable::GetItemData(id);
}

const FOption& UDiabloGameInstance::GetOption(FName id) const
{
    return UOptionDataTable::GetOption(id);
}

const FPlayerHairRow& UDiabloGameInstance::GetPlayerHair(FName id) const
{
    return UPlayerInitDataTable::GetPlayerHair(id);
}

const FPlayerFaceRow& UDiabloGameInstance::GetPlayerFace(FName id) const
{
    return UPlayerInitDataTable::GetPlayerFace(id);
}

const FPlayerArmorRow& UDiabloGameInstance::GetPlayerArmor(FName id) const
{
    return UPlayerInitDataTable::GetPlayerArmor(id);
}

const FPlayerWeaponRow& UDiabloGameInstance::GetPlayerWeapon(FName id) const
{
    return UPlayerInitDataTable::GetPlayerWeapon(id);
}

const FPlayerItemRow& UDiabloGameInstance::GetPlayerItem(FName id) const
{
    return UPlayerInitDataTable::GetPlayerItem(id);
}

const FPlayerPerkRow& UDiabloGameInstance::GetPlayerPerk(FName id) const
{
    return UPlayerInitDataTable::GetPlayerPerk(id);
}
#pragma endregion

FItemInstance UDiabloGameInstance::CreateItem(FName id)
{
    return m_ItemManager->CreateItemInstance(id);
}
