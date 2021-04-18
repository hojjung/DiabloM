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
	m_MapPlayerSkin.Reset();
	m_AryPlayerSkin.Reset();
	m_AryPlayerSkin.Reserve(30);
	
	if(!FJsonObjectConverter::JsonArrayStringToUStruct(skinUnlock, &m_AryPlayerSkin, 0, 0))
	{
		return;
	}

	TArray<const FPlayerSkinTable*> ArySkins;

	UEquipManager::GetPlayerSkinDataTable->GetAllRows("", ArySkins);
	
	int IterMax = FMath::Min(ArySkins.Num(),m_AryPlayerSkin.Num());
	
	for(int i=0;i<IterMax; i++)
	{
		m_AryPlayerSkin[i].m_PlayerData = ArySkins[i];
		m_AryPlayerSkin[i].SetValue();
		m_MapPlayerSkin.Add(m_AryPlayerSkin[i].m_PlayerData, i);
	}
}


void UEquipManager::SetStringWeaponUnlocked(FString weaponUnlock)
{
	TArray<const FWeaponTable*> AryWeaponTable;

	UEquipManager::GetWeaponDataTable->GetAllRows("", AryWeaponTable);

	m_MapPlayerWeapon.Reset();
	m_AryWeapons.Reset();
	m_AryWeapons.Reserve(30);

	TArray<FString> AryEachDatas;

	StringSplitEachItem(weaponUnlock, AryEachDatas);
	//
	if(!FJsonObjectConverter::JsonArrayStringToUStruct(weaponUnlock, &m_AryWeapons, 0, 0))
	{
		return;
	}

	int IterMax = FMath::Min(AryWeaponTable.Num(),m_AryWeapons.Num());

	for (int i = 0; i < IterMax; i++)
	{
		m_AryWeapons[i].m_EquipData=AryWeaponTable[i];
		m_AryWeapons[i].SetLevel(m_AryWeapons[i].m_nLv);
		m_MapPlayerWeapon.Add(m_AryWeapons[i].m_EquipData, i);
	}
}

void UEquipManager::SetStringPetUnlocked(FString petUnlock)
{
	TArray<const FPetTable*> AryPet;

	UEquipManager::GetPetDataTable->GetAllRows("", AryPet);

	m_AryPets.Reset();

	TArray<FString> AryEachDatas;

	StringSplitEachItem(petUnlock, AryEachDatas);

	int IterMax = FMath::Min(AryPet.Num(),AryEachDatas.Num());

	for (int i = 0; i < IterMax; i++)
	{
		FPetSpec EqSpec;
		EqSpec.m_PetData = AryPet[i];
		EqSpec.ParseFromString(AryEachDatas[i]);
		EqSpec.SetLevel(EqSpec.m_nLv);

		m_AryPets.Add(EqSpec);
	}
}

void UEquipManager::SetStringAccesoryUnlocked(FString acceUnlock)
{
	TArray<const FAccessoryTable*> AryAcces;

	UEquipManager::GetAcceeDataTable->GetAllRows("", AryAcces);

	m_AryAcce.Reset();
	m_MapAccessory.Reset();

	TArray<FString> AryEachDatas;

	StringSplitEachItem(acceUnlock, AryEachDatas);

	int IterMax = FMath::Min(AryAcces.Num(),AryEachDatas.Num());

	for (int i = 0; i < IterMax; i++)
	{
		FAccessorySpec EqSpec;
		EqSpec.m_AccessoryData = AryAcces[i];
		EqSpec.ParseFromString(AryEachDatas[i]);
		EqSpec.SetLevel(EqSpec.m_nLv);

		int index = m_AryAcce.Add(EqSpec);

		m_MapAccessory.Add(m_AryAcce[index].m_AccessoryData, index);
	}
}

void UEquipManager::EquipAll()
{
	int index = 0;

	for (FPetSpec& Spec : m_AryPets)
	{
		if (Spec.m_nIsEquipped)
		{
			TryEquipPet(index);

			break;
		}

		index++;
	}

	index = 0;

	for (FWeaponSpec& Spec : m_AryWeapons)
	{
		if (Spec.m_nIsEquipped)
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
		if (Spec.m_nIsEquipped)
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

	m_AryPlayerSkin[index].m_nIsEquipped = true;

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
		m_AryWeapons[m_nSelectedWeapon].m_nIsEquipped = false;
	}

	m_AryWeapons[index].m_nIsEquipped = true;

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
		m_AryWings[m_nSelectedWing].m_nIsEquipped = false;
	}

	m_AryWings[index].m_nIsEquipped = true;

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
		m_AryPets[m_nSelectedPet].m_nIsEquipped = false;
	}

	m_AryPets[index].m_nIsEquipped = true;


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

	if (m_AryPlayerSkin[index].m_nStackCount < 5)
	{
		return false;
	}

	while (m_AryPlayerSkin[index].m_nStackCount > 4)
	{
		m_AryPlayerSkin[index].m_nStackCount -= 5;

		if (m_AryPlayerSkin[NextIndex].m_nIsUnlocked < 1)
		{
			m_AryPlayerSkin[NextIndex].m_nIsUnlocked++;

			continue;
		}

		m_AryPlayerSkin[NextIndex].m_nStackCount++;
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

	if (m_AryWings[index].m_nStackCount < 5)
	{
		return false;
	}

	while (m_AryWings[index].m_nStackCount > 4)
	{
		m_AryWings[index].m_nStackCount -= 5;

		if (m_AryWings[NextIndex].m_nIsUnlocked < 1)
		{
			m_AryWings[NextIndex].m_nIsUnlocked++;

			continue;
		}

		m_AryWings[NextIndex].m_nStackCount++;
	}

	m_OnWingChanged.Broadcast(index, NextIndex);

	return true;
}

bool UEquipManager::TryCombineLevelUpAccessory(int index)
{
	if (m_AryAcce[index].m_nStackCount < m_AryAcce[index].m_LvlUpCost)
	{
		return false;
	}

	m_AryAcce[index].m_nStackCount -= m_AryAcce[index].m_LvlUpCost;

	m_AryAcce[index].m_nLv++;

	m_AryAcce[index].SetLevel(m_AryAcce[index].m_nLv);

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

	if (m_AryWeapons[index].m_nStackCount < 5)
	{
		return false;
	}

	while (m_AryWeapons[index].m_nStackCount > 4)
	{
		m_AryWeapons[index].m_nStackCount -= 5;

		if (m_AryWeapons[NextIndex].m_nLv < 1)
		{
			m_AryWeapons[NextIndex].m_nLv++;
			m_AryWeapons[NextIndex].SetLevel(m_AryWeapons[NextIndex].m_nLv);

			continue;
		}

		m_AryWeapons[NextIndex].m_nStackCount++;
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

	if (m_AryPets[index].m_nStackCount < 5)
	{
		return false;
	}

	while (m_AryPets[index].m_nStackCount > 4)
	{
		m_AryPets[index].m_nStackCount -= 5;

		if (m_AryPets[NextIndex].m_nLv < 1)
		{
			m_AryPets[NextIndex].m_nLv++;
			m_AryPets[NextIndex].SetLevel(m_AryPets[NextIndex].m_nLv);

			continue;
		}

		m_AryPets[NextIndex].m_nStackCount++;
	}

	m_OnPetChanged.Broadcast(NextIndex, index);

	return true;
}

bool UEquipManager::TryLvUpWeapon(int index)
{
	if (m_AryWeapons[index].m_nLv >= m_AryWeapons[index].GetMaxLv())
	{
		return false;
	}

	if (!UDiabloGameInstance::Get->m_GoldManager->SubtractGold(m_AryWeapons[index].m_LvlUpCost))
	{
		return false;
	}

	m_AryWeapons[index].m_nLv++;
	m_AryWeapons[index].SetLevel(m_AryWeapons[index].m_nLv);

	m_OnWeaponChanged.Broadcast(-1, index);

	return true;
}

bool UEquipManager::TryLvUpPet(int index)
{
	if (m_AryPets[index].m_nLv >= 100)
	{
		return false;
	}

	if (!UDiabloGameInstance::Get->m_GoldManager->SubtractGold(m_AryPets[index].m_LvlUpCost))
	{
		return false;
	}

	m_AryPets[index].m_nLv++;
	m_AryPets[index].SetLevel(m_AryPets[index].m_nLv);

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
	if (m_AryWeapons[index].m_nLv < 1)
	{
		TryLvUpWeapon(index);
	}
	else
	{
		m_AryWeapons[index].m_nStackCount++;
	}

	m_OnWeaponChanged.Broadcast(-1, index);
}

void UEquipManager::AddSkinStack(int index)
{
	if (m_AryPlayerSkin[index].m_nIsUnlocked < 1)
	{
		m_AryPlayerSkin[index].m_nIsUnlocked = 1;
	}
	else
	{
		m_AryPlayerSkin[index].m_nStackCount++;
	}

	m_OnPlSkinChanged.Broadcast(-1, index);
}

void UEquipManager::AddPetStack(int index)
{
	if (m_AryPets[index].m_nLv < 1)
	{
		TryLvUpPet(index);
	}
	else
	{
		m_AryPets[index].m_nStackCount++;
	}

	m_OnPetChanged.Broadcast(-1, index);
}

void UEquipManager::AddWingStack(int index)
{
	if (m_AryWings[index].m_nIsUnlocked < 1)
	{
		m_AryWings[index].m_nIsUnlocked = 1;
	}
	else
	{
		m_AryWings[index].m_nStackCount++;
	}

	m_OnWingChanged.Broadcast(-1, index);
}

void UEquipManager::AddAccessoryStack(int index)
{
	if (m_AryAcce[index].m_nLv < 1)
	{
		m_AryAcce[index].SetLevel(1);
	}
	else
	{
		m_AryAcce[index].m_nStackCount++;
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
		WeaponStr.Append(WeaponSpec.ParseToString());
		WeaponStr.AppendChar(TEXT('/'));
	}

	return WeaponStr;
}

FString UEquipManager::GetSkinDataStr()
{
	FString SkinStr;

	for (auto& SkinSpec : m_AryPlayerSkin)
	{
		SkinStr.Append(SkinSpec.ParseToString());
		SkinStr.AppendChar(TEXT('/'));
	}

	return SkinStr;
}

FString UEquipManager::GetPetDataStr()
{
	FString PetStr;

	for (auto& PetSpec : m_AryPets)
	{
		PetStr.Append(PetSpec.ParseToString());
		PetStr.AppendChar(TEXT('/'));
	}

	return PetStr;
}

FString UEquipManager::GetAccessoryDataStr()
{
	FString AccessoryStr;

	for (auto& AcceSpec : m_AryAcce)
	{
		AccessoryStr.Append(AcceSpec.ParseToString());
		AccessoryStr.AppendChar(TEXT('/'));
	}

	return AccessoryStr;
}

FString UEquipManager::GetWingDataStr()
{
	FString WingStr;

	for (auto& WingSpecs : m_AryWings)
	{
		WingStr.Append(WingSpecs.ParseToString());
		WingStr.AppendChar(TEXT('/'));
	}

	return WingStr;
}
