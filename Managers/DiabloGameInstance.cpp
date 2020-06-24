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

	static ConstructorHelpers::FObjectFinder<UDataTable> FoundItemTable(TEXT("DataTable'/Game/DataTables/TestItemTable.TestItemTable'"));
	if (FoundItemTable.Succeeded())
	{
		m_ItemTable = FoundItemTable.Object;
	}
}

const FBaseStatTable * UDiabloGameInstance::GetBaseUnit(FName id) const
{
	return m_BaseUnitTable->FindRow<FBaseStatTable>(id, "");
}

const FItemData * UDiabloGameInstance::GetItemData(FName id) const
{
	return m_ItemTable->FindRow<FItemData>(id, "");
}

