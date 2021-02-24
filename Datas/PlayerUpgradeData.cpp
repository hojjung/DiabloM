#include "PlayerUpgradeData.h"

 UDataTable* UPlayerUpgradeData::GetPlUpgradeTable = nullptr;

UPlayerUpgradeData::UPlayerUpgradeData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable(
             TEXT("DataTable'/Game/DataTables/Upgrade/PlayerDefaultUpgradeTable.PlayerDefaultUpgradeTable'"));
	GetPlUpgradeTable = FoundEntityTable.Object;
}

const FUpgradeDataRow& UPlayerUpgradeData::GetPlUpgradeData(FName id)
{
	return *GetPlUpgradeTable->FindRow<FUpgradeDataRow>(id,"");
}

const FUpgradeDataRow* UPlayerUpgradeData::GetPlUpgradeDataPtr(FName id)
{
	return GetPlUpgradeTable->FindRow<FUpgradeDataRow>(id,"");
}
