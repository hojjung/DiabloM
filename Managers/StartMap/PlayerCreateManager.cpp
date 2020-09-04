#include "PlayerCreateManager.h"
#include "Managers/DiabloGameInstance.h"



PlayerCreateManager::PlayerCreateManager()
{
    
}

PlayerCreateManager::~PlayerCreateManager()
{
    
}

void PlayerCreateManager::Init(UDiabloGameInstance* gameInst)
{
    UPlayerInitDataTable::GetPlayerHairTable->GetAllRows("FailHair",m_AryHair);
    UPlayerInitDataTable::GetPlayerFaceTable->GetAllRows("FailFace",m_AryFace);
    UPlayerInitDataTable::GetPlayerArmorTable->GetAllRows("FailArmor",m_AryArmor);
    UPlayerInitDataTable::GetPlayerWeaponTable->GetAllRows("FailWeapon",m_AryWeapon);
    UPlayerInitDataTable::GetPlayerItemTable->GetAllRows("FailItem",m_AryItem);
    UPlayerInitDataTable::GetPlayerPerkTable->GetAllRows("FailPerk",m_AryPerk);

    m_CurrentCharData.m_CurrentHair = m_AryHair[0];
    m_CurrentCharData.m_CurrentFace = m_AryFace[0];
    m_CurrentCharData.m_CurrentArmor = m_AryArmor[0];
    m_CurrentCharData.m_CurrentWeapon = m_AryWeapon[0];
    m_CurrentCharData.m_CurrentItem = m_AryItem[0];
    m_CurrentCharData.m_CurrentPerk = m_AryPerk[0];
}

void PlayerCreateManager::OnDataChanged()
{
    
}
