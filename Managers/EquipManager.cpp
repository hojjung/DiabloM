#include "EquipManager.h"

#include "DiabloGameInstance.h"
#include "JsonObjectConverter.h"
#include "PlayfabManager.h"
#include "Characters/PlayerDiabloCharacter.h"

UDataTable* UEquipManager::GetPlayerSkinDataTable = nullptr;
UDataTable* UEquipManager::GetWeaponDataTable = nullptr;
UDataTable* UEquipManager::GetWingDataTable = nullptr;
UDataTable* UEquipManager::GetPetDataTable = nullptr;
UDataTable* UEquipManager::GetAcceeDataTable = nullptr;

void UEquipManager::ClearSelectedIndex() //ned this for change level,player modify
{
	m_nSelectedSkin = -1;
	m_nSelectedWing = -1;
	m_nSelectedWeapon = -1;
	m_nSelectedPet = -1;
}

UEquipManager::UEquipManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable(
		TEXT("DataTable'/Game/DataTables/Equipments/WeaponTable.WeaponTable'"));
	GetWeaponDataTable = FoundEntityTable.Object;
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable1(
		TEXT("DataTable'/Game/DataTables/Equipments/WingTable.WingTable'"));
	GetWingDataTable = FoundEntityTable1.Object;
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable2(
		TEXT("DataTable'/Game/DataTables/Equipments/PetTable.PetTable'"));
	GetPetDataTable = FoundEntityTable2.Object;
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable3(
		TEXT("DataTable'/Game/DataTables/Equipments/AccessoryTable.AccessoryTable'"));
	GetAcceeDataTable = FoundEntityTable3.Object;
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable4(
		TEXT("DataTable'/Game/DataTables/Equipments/PlayerSkinTable.PlayerSkinTable'"));
	GetPlayerSkinDataTable = FoundEntityTable4.Object;
	//GetPlayerSkinDataTable
	//DataTable'/Game/DataTables/Equipments/WingDataTable.WingDataTable'
	//DataTable'/Game/DataTables/Equipments/PetDataTable.PetDataTable'
	//DataTable'/Game/DataTables/Equipments/AccessoryDataTable.AccessoryDataTable'

	ClearSelectedIndex();
}

void UEquipManager::SetEquipDataFromServer(const FString& classSkin, const FString& weapon,const FString& pet)
{
	SetStringSkinUnlocked(classSkin);
	SetStringWeaponUnlocked(weapon);
	//SetStringWingUnlocked(wing);
	SetStringPetUnlocked(pet);
	//SetStringAccesoryUnlocked(acce);
}


int UEquipManager::StringSplitEachItem(const FString& equipDatas, TArray<FString>& outStrAry) const
{
	return equipDatas.ParseIntoArray(outStrAry,TEXT("/"));
}

void UEquipManager::SetStringSkinUnlocked(FString skinUnlock) //이 str에 모든 스킨정보가 등록되어있음,근데 순서를 어떻게 보장시킴?
{
	m_MapPlayerSkin.Empty(30);
	m_AryPlayerSkin.Empty(30);
	TArray<const FPlayerSkinTable*> ArySkinsTable;
	UEquipManager::GetPlayerSkinDataTable->GetAllRows("", ArySkinsTable);
	
	if(!FJsonObjectConverter::JsonArrayStringToUStruct(skinUnlock, &m_AryPlayerSkin, 0, 0))
	{
		return;
	}

	
	int IterMax = FMath::Min(ArySkinsTable.Num(),m_AryPlayerSkin.Num());
	
	for(int i=0;i<IterMax; i++)
	{
		m_AryPlayerSkin[i].m_PlayerData = ArySkinsTable[i];
		m_AryPlayerSkin[i].SetValue();
		m_MapPlayerSkin.Add(m_AryPlayerSkin[i].m_PlayerData, i);
	}
}


void UEquipManager::SetStringWeaponUnlocked(FString weaponUnlock)
{
	m_MapPlayerWeapon.Empty(30);
	m_AryWeapons.Empty(30);
	TArray<const FWeaponTable*> AryWeaponTable;
	UEquipManager::GetWeaponDataTable->GetAllRows("", AryWeaponTable);

	if(!FJsonObjectConverter::JsonArrayStringToUStruct(weaponUnlock, &m_AryWeapons, 0, 0))
	{
		return;
	}

	int IterMax = FMath::Min(AryWeaponTable.Num(),m_AryWeapons.Num());

	for (int i = 0; i < IterMax; i++)
	{
		m_AryWeapons[i].m_EquipData=AryWeaponTable[i];
		m_AryWeapons[i].SetLevel(m_AryWeapons[i].Level);
		m_MapPlayerWeapon.Add(m_AryWeapons[i].m_EquipData, i);
	}
}

void UEquipManager::SetStringPetUnlocked(FString petUnlock)
{
	m_MapPlayerPet.Empty(30);
	m_AryPets.Empty(30);
	TArray<const FPetTable*> AryPetTable;
	UEquipManager::GetPetDataTable->GetAllRows("", AryPetTable);
	//
	if(!FJsonObjectConverter::JsonArrayStringToUStruct(petUnlock, &m_AryPets, 0, 0))
	{
		return;
	}

	int IterMax = FMath::Min(m_AryPets.Num(),AryPetTable.Num());

	for (int i = 0; i < IterMax; i++)
	{
		m_AryPets[i].m_PetData=AryPetTable[i];
		m_AryPets[i].SetLevel(m_AryPets[i].Level);
		m_MapPlayerPet.Add(m_AryPets[i].m_PetData, i);
	}
}

void UEquipManager::SetStringAccesoryUnlocked(FString acceUnlock)
{
	m_MapAccessory.Empty(30);
	m_AryAcce.Empty(30);
	TArray<const FAccessoryTable*> AryAccesTable;
	UEquipManager::GetAcceeDataTable->GetAllRows("", AryAccesTable);
	//
	if(!FJsonObjectConverter::JsonArrayStringToUStruct(acceUnlock, &m_AryAcce, 0, 0))
	{
		return;
	}

	int IterMax = FMath::Min(m_AryAcce.Num(),AryAccesTable.Num());

	for (int i = 0; i < IterMax; i++)
	{
		m_AryAcce[i].m_AccessoryData=AryAccesTable[i];
		m_AryAcce[i].SetLevel(m_AryPets[i].Level);
		m_MapAccessory.Add(m_AryAcce[i].m_AccessoryData, i);
	}
}

void UEquipManager::EquipAll()
{
	int index = 0;

	for (FPetSpec& Spec : m_AryPets)
	{
		if (Spec.IsEquipped)
		{
			TryEquipPet(index);

			break;
		}

		index++;
	}

	index = 0;

	for (FWeaponSpec& Spec : m_AryWeapons)
	{
		if (Spec.IsEquipped)
		{
			TryEquipWeapon(index);

			break;
		}

		index++;
	}

	index = 0;

	for (FPlayerClassSpec& Spec : m_AryPlayerSkin)
	{
		if (Spec.IsEquipped)
		{
			TryEquipSkin(index);

			break;
		}

		index++;
	}

	index = 0;

	for (FWingSpec& Spec : m_AryWings)
	{
		if (Spec.IsEquipped)
		{
			TryEquipWing(index);

			break;
		}

		index++;
	}

	index = 0;
}


void UEquipManager::TryEquipSkin(int index)
{
	if (m_nSelectedSkin == index)
	{
		PRINTF("The Skin Is Same ");

		return;
	}

	if (m_AryPlayerSkin.Num() <= index)
	{
		return;
	}

	if (m_nSelectedSkin > -1)
	{
		m_AryPlayerSkin[m_nSelectedSkin].IsEquipped = false;
	}

	m_AryPlayerSkin[index].IsEquipped = true;

	UDiabloGameInstance::Get->GetPlChar()->PlayerClassDataInject(m_AryPlayerSkin[index]);

	UDiabloGameInstance::Get->GetPlCon()->ClientForceGarbageCollection();

	m_OnPlSkinChanged.Broadcast(m_nSelectedSkin, index);

	m_nSelectedSkin = index;
}

void UEquipManager::TryEquipWeapon(int index)
{
	if (m_nSelectedWeapon == index)
	{
		PRINTF("The Weapon Is Same");

		return;
	}

	if (m_AryWeapons.Num() <= index)
	{
		return;
	}

	if (m_nSelectedWeapon > -1)
	{
		m_AryWeapons[m_nSelectedWeapon].IsEquipped = false;
	}

	m_AryWeapons[index].IsEquipped = true;

	UDiabloGameInstance::Get->GetPlChar()->WeaponDataInject(m_AryWeapons[index]);
	
	m_OnWeaponChanged.Broadcast(m_nSelectedWeapon, index);

	m_nSelectedWeapon = index;
}

void UEquipManager::TryEquipWing(int index)
{
	if (m_nSelectedWing == index)
	{
		PRINTF("The Wing Is Same");

		return;
	}

	if (m_AryWings.Num() <= index)
	{
		return;
	}

	if (m_nSelectedWing > -1)
	{
		m_AryWings[m_nSelectedWing].IsEquipped = false;
	}

	m_AryWings[index].IsEquipped = true;

	UDiabloGameInstance::Get->GetPlChar()->WingDataInject(m_AryWings[index]);

	m_OnWingChanged.Broadcast(m_nSelectedWing, index);

	m_nSelectedWing = index;
}

void UEquipManager::TryEquipPet(int index)
{
	if (m_nSelectedPet == index)
	{
		PRINTF("The Pet Is Same");

		return;
	}

	if (m_AryPets.Num() <= index)
	{
		return;
	}

	if (m_nSelectedPet > -1)
	{
		m_AryPets[m_nSelectedPet].IsEquipped = false;
	}

	m_AryPets[index].IsEquipped = true;


	UDiabloGameInstance::Get->GetPlChar()->PetDataInject(m_AryPets[index]);

	m_OnPetChanged.Broadcast(m_nSelectedPet, index);

	m_nSelectedPet = index;
}


bool UEquipManager::TryCombineSkin(int index)
{
	int NextIndex = index + 1;

	if (NextIndex >= m_AryPlayerSkin.Num())
	{
		return false; //full
	}

	if (m_AryPlayerSkin[index].StackCount < 5)
	{
		return false;
	}

	while (m_AryPlayerSkin[index].StackCount > 4)
	{
		m_AryPlayerSkin[index].StackCount -= 5;

		if (!m_AryPlayerSkin[NextIndex].IsUnlocked)//해금
		{
			m_AryPlayerSkin[NextIndex].IsUnlocked=true;

			continue;
		}

		m_AryPlayerSkin[NextIndex].StackCount++;//해금완료됐으면 스택늘려줌
	}

	m_OnPlSkinChanged.Broadcast(index, NextIndex);

	return true;
}

bool UEquipManager::TryCombineWing(int index)
{
	int NextIndex = index + 1;

	if (NextIndex >= m_AryWings.Num())
	{
		return false; //full
	}

	if (m_AryWings[index].StackCount < 5)
	{
		return false;
	}

	while (m_AryWings[index].StackCount > 4)
	{
		m_AryWings[index].StackCount -= 5;

		if (!m_AryWings[NextIndex].IsUnlocked)
		{
			m_AryWings[NextIndex].IsUnlocked=true;

			continue;
		}

		m_AryWings[NextIndex].StackCount++;
	}

	m_OnWingChanged.Broadcast(index, NextIndex);

	return true;
}

bool UEquipManager::TryCombineLevelUpAccessory(int index)
{
	if (m_AryAcce[index].StackCount < m_AryAcce[index].m_LvlUpCost)
	{
		return false;
	}

	m_AryAcce[index].StackCount -= m_AryAcce[index].m_LvlUpCost;

	m_AryAcce[index].Level++;

	m_AryAcce[index].SetLevel(m_AryAcce[index].Level);

	m_OnAccessoryChanged.Broadcast(-1, index);

	return true;
}

bool UEquipManager::TryCombineWeapon(int index)
{
	int NextIndex = index + 1;

	if (NextIndex >= m_AryWeapons.Num())
	{
		return false; //full
	}

	if (m_AryWeapons[index].StackCount < 5)
	{
		return false;
	}

	while (m_AryWeapons[index].StackCount > 4)
	{
		m_AryWeapons[index].StackCount -= 5;

		if (m_AryWeapons[NextIndex].Level < 1)
		{
			m_AryWeapons[NextIndex].Level++;
			m_AryWeapons[NextIndex].SetLevel(m_AryWeapons[NextIndex].Level);

			continue;
		}

		m_AryWeapons[NextIndex].StackCount++;
	}

	m_OnWeaponChanged.Broadcast(NextIndex, index);

	return true;
}

bool UEquipManager::TryCombinePet(int index)
{
	int NextIndex = index + 1;

	if (NextIndex >= m_AryPets.Num())
	{
		return false; //full
	}

	if (m_AryPets[index].StackCount < 5)
	{
		return false;
	}

	while (m_AryPets[index].StackCount > 4)
	{
		m_AryPets[index].StackCount -= 5;

		if (m_AryPets[NextIndex].Level < 1)
		{
			m_AryPets[NextIndex].Level++;
			m_AryPets[NextIndex].SetLevel(m_AryPets[NextIndex].Level);

			continue;
		}

		m_AryPets[NextIndex].StackCount++;
	}

	m_OnPetChanged.Broadcast(NextIndex, index);

	return true;
}

bool UEquipManager::TryLvUpWeapon(int index)
{
	if (m_AryWeapons[index].Level >= m_AryWeapons[index].GetMaxLv())
	{
		return false;
	}

	if (!UDiabloGameInstance::Get->m_GoldManager->SubtractGold(m_AryWeapons[index].m_LvlUpCost))
	{
		return false;
	}

	m_AryWeapons[index].Level++;
	m_AryWeapons[index].SetLevel(m_AryWeapons[index].Level);

	m_OnWeaponChanged.Broadcast(-1, index);

	return true;
}

bool UEquipManager::TryLvUpPet(int index)
{
	if (m_AryPets[index].Level >= 100)
	{
		return false;
	}

	if (!UDiabloGameInstance::Get->m_GoldManager->SubtractGold(m_AryPets[index].m_LvlUpCost))
	{
		return false;
	}

	m_AryPets[index].Level++;
	m_AryPets[index].SetLevel(m_AryPets[index].Level);

	m_OnPetChanged.Broadcast(-1, index);

	return true;
}

void UEquipManager::AddWeaponStack(const FGachaAbleRow* weaponData)
{
	AddWeaponStack(m_MapPlayerWeapon[weaponData]);
}

void UEquipManager::AddSkinStack(const FGachaAbleRow* skinData)
{
	AddSkinStack(m_MapPlayerSkin[skinData]);
}

void UEquipManager::AddAccessoryStack(const FGachaAbleRow* acceData)
{
	AddAccessoryStack(m_MapAccessory[acceData]);
}

void UEquipManager::AddWeaponStack(int index)
{
	if (m_AryWeapons[index].Level < 1)
	{
		TryLvUpWeapon(index);
	}
	else
	{
		m_AryWeapons[index].StackCount++;
	}

	m_OnWeaponChanged.Broadcast(-1, index);
}

void UEquipManager::AddSkinStack(int index)
{
	if (!m_AryPlayerSkin[index].IsUnlocked)
	{
		m_AryPlayerSkin[index].IsUnlocked = true;
	}
	else
	{
		m_AryPlayerSkin[index].StackCount++;
	}

	m_OnPlSkinChanged.Broadcast(-1, index);
}

void UEquipManager::AddPetStack(int index)
{
	if (m_AryPets[index].Level < 1)
	{
		TryLvUpPet(index);
	}
	else
	{
		m_AryPets[index].StackCount++;
	}

	m_OnPetChanged.Broadcast(-1, index);
}

void UEquipManager::AddWingStack(int index)
{
	if (!m_AryWings[index].IsUnlocked)
	{
		m_AryWings[index].IsUnlocked = true;
	}
	else
	{
		m_AryWings[index].StackCount++;
	}

	m_OnWingChanged.Broadcast(-1, index);
}

void UEquipManager::AddAccessoryStack(int index)
{
	if (m_AryAcce[index].Level < 1)
	{
		m_AryAcce[index].SetLevel(1);
	}
	else
	{
		m_AryAcce[index].StackCount++;
	}

	m_OnAccessoryChanged.Broadcast(-1, index);
}

FAccessorySpec& UEquipManager::GetAccessory(EAccessory acces)
{
	return m_AryAcce[(int)acces];
}


FString UEquipManager::GetWeaponDataStr()
{
	FString WeaponStr;

	for (auto& WeaponSpec : m_AryWeapons)
	{
	}

	return WeaponStr;
}

FString UEquipManager::GetSkinDataStr()
{
	FString SkinStr;

	for (auto& SkinSpec : m_AryPlayerSkin)
	{
	}

	return SkinStr;
}

FString UEquipManager::GetPetDataStr()
{
	FString PetStr;

	for (auto& PetSpec : m_AryPets)
	{
	}

	return PetStr;
}

FString UEquipManager::GetAccessoryDataStr()
{
	FString AccessoryStr;

	for (auto& AcceSpec : m_AryAcce)
	{
	}

	return AccessoryStr;
}

FString UEquipManager::GetWingDataStr()
{
	FString WingStr;

	for (auto& WingSpecs : m_AryWings)
	{
	}

	return WingStr;
}
