#include "EquipManager.h"

#include "DiabloGameInstance.h"
#include "JsonObjectConverter.h"
#include "PlayFabJsonObject.h"
#include "PlayFabJsonValue.h"

class UPlayFabJsonValue;
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

void UEquipManager::SetEquipDataFromServer(const TArray<UPlayFabJsonValue*>& classSkin,
                                           const TArray<UPlayFabJsonValue*>& weapon,
                                           const TArray<UPlayFabJsonValue*>& pet)
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

void UEquipManager::SetStringSkinUnlocked(const TArray<UPlayFabJsonValue*>& skinUnlock)
//이 str에 모든 스킨정보가 등록되어있음,근데 순서를 어떻게 보장시킴?
{
	m_MapPlayerSkin.Empty(30);
	m_AryPlayerSkin.Empty(30);
	
	UEquipManager::GetPlayerSkinDataTable->GetAllRows("", m_ArySkinsTable);

	TArray<TSharedPtr<FJsonValue>> AryJsonValue;

	for (auto* playfabJsonV : skinUnlock)
	{
		AryJsonValue.Add(playfabJsonV->GetRootValue());
	}

	if (!FJsonObjectConverter::JsonArrayToUStruct(AryJsonValue, &m_AryPlayerSkin, 0, 0))
	{
		return;
	}


	int IterMax = FMath::Min(m_ArySkinsTable.Num(), m_AryPlayerSkin.Num());

	for (int i = 0; i < IterMax; i++)
	{
		m_AryPlayerSkin[i].m_PlayerData = m_ArySkinsTable[i];
		m_AryPlayerSkin[i].SetValue();
		m_MapPlayerSkin.Add(m_AryPlayerSkin[i].m_PlayerData, i);
	}
}


void UEquipManager::SetStringWeaponUnlocked(const TArray<UPlayFabJsonValue*>& weaponUnlock)
{
	m_MapPlayerWeapon.Empty(30);
	m_AryWeapons.Empty(30);
	
	UEquipManager::GetWeaponDataTable->GetAllRows("", m_AryWeaponTable);

	TArray<TSharedPtr<FJsonValue>> AryJsonValue;

	for (auto* playfabJsonV : weaponUnlock)
	{
		AryJsonValue.Add(playfabJsonV->GetRootValue());
	}

	if (!FJsonObjectConverter::JsonArrayToUStruct(AryJsonValue, &m_AryWeapons, 0, 0))
	{
		return;
	}

	int IterMax = FMath::Min(m_AryWeaponTable.Num(), m_AryWeapons.Num());

	for (int i = 0; i < IterMax; i++)
	{
		m_AryWeapons[i].m_EquipData = m_AryWeaponTable[i];
		m_AryWeapons[i].SetLevel(m_AryWeapons[i].Level);
		m_MapPlayerWeapon.Add(m_AryWeapons[i].m_EquipData, i);
	}
}

void UEquipManager::SetStringPetUnlocked(const TArray<UPlayFabJsonValue*>& petUnlock)
{
	m_MapPlayerPet.Empty(30);
	m_AryPets.Empty(30);
	
	UEquipManager::GetPetDataTable->GetAllRows("", m_AryPetTable);
	//
	TArray<TSharedPtr<FJsonValue>> AryJsonValue;

	for (auto* playfabJsonV : petUnlock)
	{
		AryJsonValue.Add(playfabJsonV->GetRootValue());
	}

	if (!FJsonObjectConverter::JsonArrayToUStruct(AryJsonValue, &m_AryPets, 0, 0))
	{
		return;
	}

	int IterMax = FMath::Min(m_AryPets.Num(), m_AryPetTable.Num());

	for (int i = 0; i < IterMax; i++)
	{
		m_AryPets[i].m_PetData = m_AryPetTable[i];
		m_AryPets[i].SetLevel(m_AryPets[i].Level);
		m_MapPlayerPet.Add(m_AryPets[i].m_PetData, i);
	}
}

void UEquipManager::SetStringAccesoryUnlocked(const TArray<UPlayFabJsonValue*>& acceUnlock)
{
	m_MapAccessory.Empty(30);
	m_AryAcce.Empty(30);
	
	UEquipManager::GetAcceeDataTable->GetAllRows("", m_AryAccesTable);
	//
	TArray<TSharedPtr<FJsonValue>> AryJsonValue;

	for (auto* playfabJsonV : acceUnlock)
	{
		AryJsonValue.Add(playfabJsonV->GetRootValue());
	}

	if (!FJsonObjectConverter::JsonArrayToUStruct(AryJsonValue, &m_AryAcce, 0, 0))
	{
		return;
	}

	int IterMax = FMath::Min(m_AryAcce.Num(), m_AryAccesTable.Num());

	for (int i = 0; i < IterMax; i++)
	{
		m_AryAcce[i].m_AccessoryData = m_AryAccesTable[i];
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

		if (!m_AryPlayerSkin[NextIndex].IsUnlocked) //해금
		{
			m_AryPlayerSkin[NextIndex].IsUnlocked = true;

			continue;
		}

		m_AryPlayerSkin[NextIndex].StackCount++; //해금완료됐으면 스택늘려줌
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
			m_AryWings[NextIndex].IsUnlocked = true;

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

void UEquipManager::SetWeaponDataToJson(UPlayFabJsonObject* obj)
{
	TArray<UPlayFabJsonObject*> AryWeaponObj;

	AryWeaponObj.Empty(30);

	for (const FWeaponSpec& WeaponSpec : m_AryWeapons)
	{
		UPlayFabJsonObject* WeaponSpecObj = UPlayFabJsonObject::ConstructJsonObject(this);

		WeaponSpecObj->SetNumberField(TEXT("Level"), WeaponSpec.Level);

		WeaponSpecObj->SetNumberField(TEXT("IsEquipped"), WeaponSpec.IsEquipped);

		WeaponSpecObj->SetNumberField(TEXT("StackCount"), WeaponSpec.StackCount);

		AryWeaponObj.Add(WeaponSpecObj);
	}

	obj->SetObjectArrayField(TEXT("Weapon"), AryWeaponObj);
}

void UEquipManager::SetSkinDataToJson(UPlayFabJsonObject* obj)
{
	TArray<UPlayFabJsonObject*> ArySkinObj;

	ArySkinObj.Empty(30);

	for (const FPlayerClassSpec& SkinSpec : m_AryPlayerSkin)
	{
		UPlayFabJsonObject* SkinSpecObj = UPlayFabJsonObject::ConstructJsonObject(this);

		SkinSpecObj->SetNumberField(TEXT("IsUnlocked"), SkinSpec.IsUnlocked);

		SkinSpecObj->SetNumberField(TEXT("IsEquipped"), SkinSpec.IsEquipped);

		SkinSpecObj->SetNumberField(TEXT("StackCount"), SkinSpec.StackCount);

		ArySkinObj.Add(SkinSpecObj);
	}

	obj->SetObjectArrayField(TEXT("Skin"), ArySkinObj);
}

void UEquipManager::SetPetDataToJson(UPlayFabJsonObject* obj)
{
	TArray<UPlayFabJsonObject*> AryPetObj;

	AryPetObj.Empty(30);

	for (const FPetSpec& PetSpec : m_AryPets)
	{
		UPlayFabJsonObject* SkinSpecObj = UPlayFabJsonObject::ConstructJsonObject(this);

		SkinSpecObj->SetNumberField(TEXT("Level"), PetSpec.Level);

		SkinSpecObj->SetNumberField(TEXT("IsEquipped"), PetSpec.IsEquipped);

		SkinSpecObj->SetNumberField(TEXT("StackCount"), PetSpec.StackCount);

		AryPetObj.Add(SkinSpecObj);
	}

	obj->SetObjectArrayField(TEXT("Pet"), AryPetObj);
}

void UEquipManager::SetAccessoryDataToJson(UPlayFabJsonObject* obj)
{
	TArray<UPlayFabJsonObject*> AryPetObj;

	AryPetObj.Empty(30);

	for (const FPetSpec& PetSpec : m_AryPets)
	{
		UPlayFabJsonObject* SkinSpecObj = UPlayFabJsonObject::ConstructJsonObject(this);

		SkinSpecObj->SetNumberField(TEXT("Level"), PetSpec.Level);

		SkinSpecObj->SetNumberField(TEXT("IsEquipped"), PetSpec.IsEquipped);

		SkinSpecObj->SetNumberField(TEXT("StackCount"), PetSpec.StackCount);

		AryPetObj.Add(SkinSpecObj);
	}

	obj->SetObjectArrayField(TEXT("Pet"), AryPetObj);
}

void UEquipManager::SetWingDataToJson(UPlayFabJsonObject* obj)
{
	TArray<UPlayFabJsonObject*> AryPetObj;

	AryPetObj.Empty(30);

	for (const FPetSpec& PetSpec : m_AryPets)
	{
		UPlayFabJsonObject* SkinSpecObj = UPlayFabJsonObject::ConstructJsonObject(this);

		SkinSpecObj->SetNumberField(TEXT("Level"), PetSpec.Level);

		SkinSpecObj->SetNumberField(TEXT("IsEquipped"), PetSpec.IsEquipped);

		SkinSpecObj->SetNumberField(TEXT("StackCount"), PetSpec.StackCount);

		AryPetObj.Add(SkinSpecObj);
	}

	obj->SetObjectArrayField(TEXT("Pet"), AryPetObj);
}

int UEquipManager::GetWeaponUnlockCount()
{
	int Count = 0;

	for(auto& WW : m_AryWeapons)
	{
		if(WW.Level>0)
		{
			Count++;
		}
	}

	return Count;
}

int UEquipManager::GetSkinUnlockCount()
{
	int Count = 0;

	for(auto& WW : m_AryPlayerSkin)
	{
		if(WW.IsUnlocked)
		{
			Count++;
		}
	}

	return Count;
}

int UEquipManager::GetPetUnlockCount()
{
	int Count = 0;

	for(auto& WW : m_AryPets)
	{
		if(WW.Level>0)
		{
			Count++;
		}
	}

	return Count;
}

FString UEquipManager::GetWeaponUnlockStr()
{
	return FString::Printf(TEXT("%d/%d"),GetWeaponUnlockCount(),m_AryWeaponTable.Num());	
}

FString UEquipManager::GetSkinUnlockStr()
{
	return FString::Printf(TEXT("%d/%d"),GetSkinUnlockCount(),m_ArySkinsTable.Num());
}

FString UEquipManager::GetPetUnlockStr()
{
	return FString::Printf(TEXT("%d/%d"),GetPetUnlockCount(),m_AryPetTable.Num());
}
