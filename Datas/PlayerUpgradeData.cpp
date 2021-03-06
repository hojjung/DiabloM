#include "PlayerUpgradeData.h"


#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"

// UPlayerUpgradeData::UPlayerUpgradeData()
// {
// 	static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable(
//              TEXT("DataTable'/Game/DataTables/Upgrade/PlayerDefaultUpgradeTable.PlayerDefaultUpgradeTable'"));
// 	GetPlUpgradeTable = FoundEntityTable.Object;
// 	static ConstructorHelpers::FObjectFinder<UDataTable> FoundSkillTable(
//              TEXT("DataTable'/Game/DataTables/Upgrade/PlayerDefaultSkillTable.PlayerDefaultSkillTable'"));
// 	GetSkillUpgradeTable = FoundSkillTable.Object;
// 	//DataTable'/Game/DataTables/Upgrade/PlayerDefaultSkillTable.PlayerDefaultSkillTable'
// }

bool FSkillSpec::IsSkillUseable()
{
	return m_fCurrentCD<=0.f && UDiabloGameInstance::Get->GetPlChar()->GetRage() > m_SkillData->m_fRageCost;
}

float FSkillSpec::UseSkill()
{
	check(m_SkillData->m_AnimSkillMotion);
	
	UDiabloGameInstance::Get->GetPlChar()->PlayAnimMontage(m_SkillData->m_AnimSkillMotion);

	m_fCurrentCD = m_SkillData->m_fSkillCoolTime;

	return m_SkillData->m_AnimSkillMotion->GetPlayLength();
}
