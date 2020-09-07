#include "PlayerCreateManager.h"
#include "Managers/DiabloGameInstance.h"



PlayerCreateManager::PlayerCreateManager(): m_IndexHair(0), m_IndexFace(0), m_IndexArmor(0), m_IndexWeapon(0),
                                            m_IndexItem(0),
                                            m_IndexPerk(0)
{
    
}

PlayerCreateManager::~PlayerCreateManager()
{
    m_AryHair.Empty();
    m_AryFace.Empty();
    m_AryArmor.Empty();
    m_AryItem.Empty();
    m_AryPerk.Empty();
    m_CurrentCharData.Clear();
}

void PlayerCreateManager::Init(UDiabloGameInstance* gameInst)
{
    UPlayerInitDataTable::GetPlayerHairTable->GetAllRows("FailHair",m_AryHair);
    UPlayerInitDataTable::GetPlayerFaceTable->GetAllRows("FailFace",m_AryFace);
    UPlayerInitDataTable::GetPlayerArmorTable->GetAllRows("FailArmor",m_AryArmor);
    UPlayerInitDataTable::GetPlayerItemTable->GetAllRows("FailItem",m_AryItem);
    UPlayerInitDataTable::GetPlayerPerkTable->GetAllRows("FailPerk",m_AryPerk);
    //
    m_CurrentCharData.m_CurrentHair = m_AryHair[m_IndexHair];
    m_CurrentCharData.m_CurrentFace = m_AryFace[m_IndexFace];
    m_CurrentCharData.m_CurrentArmor = m_AryArmor[m_IndexArmor];
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

void PlayerCreateManager::DecreaseFace()
{
    m_IndexFace--;

    if(m_IndexFace<0)
    {
        m_IndexFace=m_AryFace.Num()-1;
    }
    //text change
    m_CurrentCharData.m_CurrentFace = m_AryFace[m_IndexFace];
    OnDataChanged();
}

void PlayerCreateManager::IncreaseFace()
{
    m_IndexFace++;

    if(m_IndexFace>=m_AryFace.Num())
    {
        m_IndexFace=0;
    }

    m_CurrentCharData.m_CurrentFace = m_AryFace[m_IndexFace];
    OnDataChanged();
}

void PlayerCreateManager::DecreaseArmor()
{
    m_IndexArmor--;

    if(m_IndexArmor<0)
    {
        m_IndexArmor=m_AryArmor.Num()-1;
    }
    //text change
    m_CurrentCharData.m_CurrentArmor = m_AryArmor[m_IndexArmor];
    OnDataChanged();
}

void PlayerCreateManager::IncreaseArmor()
{
    m_IndexArmor++;

    if(m_IndexArmor>=m_AryArmor.Num())
    {
        m_IndexArmor=0;
    }

    m_CurrentCharData.m_CurrentArmor = m_AryArmor[m_IndexArmor];
    OnDataChanged();
}


void PlayerCreateManager::DecreaseItem()
{
    m_IndexItem--;

    if(m_IndexItem<0)
    {
        m_IndexItem=m_AryItem.Num()-1;
    }
    //text change
    m_CurrentCharData.m_CurrentItem = m_AryItem[m_IndexItem];
    OnDataChanged();
}

void PlayerCreateManager::IncreaseItem()
{
    m_IndexItem++;
    
    if(m_IndexItem>=m_AryItem.Num())
    {
        m_IndexItem=0;
    }

    m_CurrentCharData.m_CurrentItem = m_AryItem[m_IndexItem];
    OnDataChanged();
}

void PlayerCreateManager::DecreasePerk()
{
    m_IndexPerk--;
    
    if(m_IndexPerk<0)
    {
        m_IndexPerk=m_AryPerk.Num()-1;
    }
    //text change
    m_CurrentCharData.m_CurrentPerk = m_AryPerk[m_IndexPerk];
    OnDataChanged();
}

void PlayerCreateManager::IncreasePerk()
{
    m_IndexPerk++;
    
    if(m_IndexPerk>=m_AryPerk.Num())
    {
        m_IndexPerk=0;
    }
    
    m_CurrentCharData.m_CurrentPerk = m_AryPerk[m_IndexPerk];
    OnDataChanged();
}

void PlayerCreateManager::CreateCharcter()
{
    SaveLoadManager::Get->CreateNewCharacter(this);
}
