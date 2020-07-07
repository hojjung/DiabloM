#include "DiabloGameInstance.h"
#include "ConstructorHelpers.h"



UDiabloGameInstance::UDiabloGameInstance(const FObjectInitializer& objInit):Super(objInit)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundBaseTable(TEXT("DataTable'/Game/DataTables/MonsterStatTable.MonsterStatTable'"));
	if (FoundBaseTable.Succeeded())
	{
		m_BaseUnitTable = FoundBaseTable.Object;
	}
	//

	static ConstructorHelpers::FObjectFinder<UDataTable> FoundItemTable(TEXT("DataTable'/Game/DataTables/DefaultItemTable.DefaultItemTable'"));
	if (FoundItemTable.Succeeded())
	{
		m_ItemTable = FoundItemTable.Object;
	}
}

void UDiabloGameInstance::Init()
{
	Super::Init();
	m_ItemManager = new ItemManager();
	m_ItemManager->Init(this);
}

void UDiabloGameInstance::Shutdown()
{
	Super::Shutdown();
	delete m_ItemManager;
}

const FBaseStatTable * UDiabloGameInstance::GetBaseUnit(FName id) const
{
	return m_BaseUnitTable->FindRow<FBaseStatTable>(id, "");
}

const FItemData * UDiabloGameInstance::GetItemData(FName id) const
{
	return m_ItemTable->FindRow<FItemData>(id, "");
}

FItemInstance UDiabloGameInstance::CreateItem(FName id)
{
	return m_ItemManager->CreateItemInstance(id);
}

