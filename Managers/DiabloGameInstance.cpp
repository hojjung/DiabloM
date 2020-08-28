#include "DiabloGameInstance.h"
#include "ConstructorHelpers.h"


UDiabloGameInstance* UDiabloGameInstance::Get = nullptr;

UDiabloGameInstance::UDiabloGameInstance()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundBaseTable(
        TEXT("DataTable'/Game/DataTables/MonsterTable.MonsterTable'"));
    if (FoundBaseTable.Succeeded())
    {
        m_MonsterUnitTable = FoundBaseTable.Object;
    }
    //
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundBaseTable2(
        TEXT("DataTable'/Game/DataTables/NPCTable.NPCTable'"));
    if (FoundBaseTable2.Succeeded())
    {
        m_NPCUnitTable = FoundBaseTable2.Object;
    }
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundBaseTable3(
        TEXT("DataTable'/Game/DataTables/PlayerTable.PlayerTable'"));
    if (FoundBaseTable3.Succeeded())
    {
        m_PlayerUnitTable = FoundBaseTable3.Object;
    }
    //
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundItemTable(
        TEXT("DataTable'/Game/DataTables/DefaultItemTable.DefaultItemTable'"));
    if (FoundItemTable.Succeeded())
    {
        m_ItemTable = FoundItemTable.Object;
    }

    UDiabloGameInstance::Get = this;

    m_ItemManager=nullptr;

    m_SaveLoadManager=nullptr;
    
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

}

void UDiabloGameInstance::Init()
{
    Super::Init();
  
}

void UDiabloGameInstance::OnStart()
{
    Super::OnStart();
	
}

void UDiabloGameInstance::Shutdown()
{
    Super::Shutdown();

    delete m_ItemManager;
    delete m_SaveLoadManager;
}

const FEntityTable* UDiabloGameInstance::GetMonsterUnit(FName id) const
{
    return m_MonsterUnitTable->FindRow<FEntityTable>(id, "");
}

const FEntityTable* UDiabloGameInstance::GetNPCUnit(FName id) const
{
    return m_NPCUnitTable->FindRow<FEntityTable>(id, "");
}

const FPlayerEntityTable* UDiabloGameInstance::GetPlayerUnit(FName id) const
{
    return m_PlayerUnitTable->FindRow<FPlayerEntityTable>(id, "");
}

const FItemData* UDiabloGameInstance::GetItemData(FName id) const
{
    return m_ItemTable->FindRow<FItemData>(id, "");
}

FItemInstance UDiabloGameInstance::CreateItem(FName id)
{
    return m_ItemManager->CreateItemInstance(id);
}
