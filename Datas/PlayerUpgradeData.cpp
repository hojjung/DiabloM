#include "PlayerUpgradeData.h"

 UDataTable* UPlayerUpgradeData::GetPlUpgradeTable = nullptr;
UDataTable* UPlayerUpgradeData::GetSkillUpgradeTable = nullptr;

UPlayerUpgradeData::UPlayerUpgradeData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable(
             TEXT("DataTable'/Game/DataTables/Upgrade/PlayerDefaultUpgradeTable.PlayerDefaultUpgradeTable'"));
	GetPlUpgradeTable = FoundEntityTable.Object;
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundSkillTable(
             TEXT("DataTable'/Game/DataTables/Upgrade/PlayerDefaultSkillTable.PlayerDefaultSkillTable'"));
	GetSkillUpgradeTable = FoundSkillTable.Object;
	//DataTable'/Game/DataTables/Upgrade/PlayerDefaultSkillTable.PlayerDefaultSkillTable'
}

const FUpgradeDataRow& UPlayerUpgradeData::GetPlUpgradeData(FName id)
{
	return *GetPlUpgradeTable->FindRow<FUpgradeDataRow>(id,"");
}

const FUpgradeDataRow* UPlayerUpgradeData::GetPlUpgradeDataPtr(FName id)
{
	return GetPlUpgradeTable->FindRow<FUpgradeDataRow>(id,"");
}

const FUpgradeDataRow& UPlayerUpgradeData::GetSkillUpgradeData(FName id)
{
	return *GetSkillUpgradeTable->FindRow<FUpgradeDataRow>(id,"");
}

const FUpgradeDataRow* UPlayerUpgradeData::GetSkillUpgradeDataPtr(FName id)
{
	return GetSkillUpgradeTable->FindRow<FUpgradeDataRow>(id,"");
}
