#include "PlayerCreateManager.h"
#include "Managers/DiabloGameInstance.h"



PlayerCreateManager::PlayerCreateManager(): m_IndexHair(0), m_IndexFace(0), m_IndexArmor(0), m_IndexWeapon(0),
                                            m_IndexItem(0),
                                            m_IndexPerk(0)
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
    //
    m_CurrentCharData.m_CurrentHair = m_AryHair[m_IndexHair];
    m_CurrentCharData.m_CurrentFace = m_AryFace[m_IndexFace];
    m_CurrentCharData.m_CurrentArmor = m_AryArmor[m_IndexArmor];
    m_CurrentCharData.m_CurrentWeapon = m_AryWeapon[m_IndexWeapon];
    m_CurrentCharData.m_CurrentItem = m_AryItem[m_IndexItem];
    m_CurrentCharData.m_CurrentPerk = m_AryPerk[m_IndexPerk];
}

void PlayerCreateManager::OnDataChanged()
{
    
    m_OnVisualChange.Broadcast(m_CurrentCharData);
}

void PlayerCreateManager::DecreaseHair()
{
    m_IndexHair--;

    if(m_IndexHair<0)
    {
        m_IndexHair=m_AryHair.Num()-1;
    }
    //text change
    m_CurrentCharData.m_CurrentHair = m_AryHair[m_IndexHair];
    OnDataChanged();
}

void PlayerCreateManager::IncreaseHair()
{
    m_IndexHair++;

    if(m_IndexHair>=m_AryHair.Num())
    {
        m_IndexHair=0;
    }

    m_CurrentCharData.m_CurrentHair = m_AryHair[m_IndexHair];
    OnDataChanged();
}
