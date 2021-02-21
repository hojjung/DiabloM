#include "PlayerClassManager.h"

#include "Datas/CharacterDataTable.h"

void UPlayerClassManager::LoadPlayerClass(FName classData)
{
	m_IDClassData = classData;
}

const FPlayerEntityTable* UPlayerClassManager::GetPlayerEntity()
{
	return UCharacterDataTable::GetPlayerEntityPtr(m_IDClassData);
}

