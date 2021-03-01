#include "EquipManager.h"

#include "DiabloGameInstance.h"
#include "PlayfabManager.h"
#include "Characters/PlayerDiabloCharacter.h"

UDataTable* UEquipManager::GetPlayerSkinDataTable = nullptr;
UDataTable* UEquipManager::GetWeaponDataTable = nullptr;
UDataTable* UEquipManager::GetWingDataTable = nullptr;
UDataTable* UEquipManager::GetPetDataTable = nullptr;
UDataTable* UEquipManager::GetAcceeDataTable = nullptr;

void UEquipManager::ClearSelectedIndex()//ned this for change level,player modify
{
	m_nSelectedSkin = -1;
	m_nSelectedWing = -1;
	m_nSelectedWeapon = -1;
	m_nSelectedPet = -1;
	m_nSelectedAccessory1 = -1;
	m_nSelectedAccessory2 = -1;
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

int UEquipManager::StringSplitEachItem(const FString& equipDatas, TArray<FString>& outStrAry) const
{
	return equipDatas.ParseIntoArray(outStrAry,TEXT("/"));
}

void UEquipManager::SetStringSkinUnlocked(FString skinUnlock) //이 str에 모든 스킨정보가 등록되어있음,근데 순서를 어떻게 보장시킴?
{
	TArray<const FPlayerSkinTable*> ArySkins;

	UEquipManager::GetPlayerSkinDataTable->GetAllRows("", ArySkins);

	m_AryPlayerSkin.Reset();


	TArray<FString> AryEachDatas;

	StringSplitEachItem(skinUnlock, AryEachDatas);

	for (int i = 0; i < ArySkins.Num(); i++)
	{
		FPlayerClassSpec PlSpec;
		PlSpec.m_PlayerData = ArySkins[i];
		PlSpec.ParseFromString(AryEachDatas[i]);
		PlSpec.SetValue();
		m_AryPlayerSkin.Add(PlSpec);
	}
}


void UEquipManager::SetStringWingUnlocked(FString wingUnlock)
{
	TArray<const FWingTable*> AryWings;

	UEquipManager::GetWingDataTable->GetAllRows("", AryWings);

	m_AryWings.Reset();


	TArray<FString> AryEachDatas;

	int Len = StringSplitEachItem(wingUnlock, AryEachDatas);

	for (int i = 0; i < AryWings.Num(); i++)
	{
		FWingSpec EqSpec;
		EqSpec.m_WingData = AryWings[i];
		EqSpec.ParseFromString(AryEachDatas[i]);
		m_AryWings.Add(EqSpec);
	}
}

void UEquipManager::SetStringWeaponUnlocked(FString weaponUnlock)
{
	TArray<const FWeaponTable*> AryWeapon;

	UEquipManager::GetWeaponDataTable->GetAllRows("", AryWeapon);

	m_AryWeapons.Reset();


	TArray<FString> AryEachDatas;

	int Len = StringSplitEachItem(weaponUnlock, AryEachDatas);

	for (int i = 0; i < AryWeapon.Num(); i++)
	{
		FWeaponSpec EqSpec;
		EqSpec.m_EquipData = AryWeapon[i];
		EqSpec.ParseFromString(AryEachDatas[i]);
		EqSpec.SetLevel(EqSpec.m_nLv);
		m_AryWeapons.Add(EqSpec);
	}
}

void UEquipManager::SetStringPetUnlocked(FString petUnlock)
{
	TArray<const FPetTable*> AryPet;

	UEquipManager::GetPetDataTable->GetAllRows("", AryPet);

	m_AryPets.Reset();


	TArray<FString> AryEachDatas;

	StringSplitEachItem(petUnlock, AryEachDatas);

	for (int i = 0; i < AryPet.Num(); i++)
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


	TArray<FString> AryEachDatas;

	int Len = StringSplitEachItem(acceUnlock, AryEachDatas);

	for (int i = 0; i < AryAcces.Num(); i++)
	{
		FAccessorySpec EqSpec;
		EqSpec.m_AccessoryData = AryAcces[i];
		EqSpec.ParseFromString(AryEachDatas[i]);
		EqSpec.SetLevel(EqSpec.m_nLv);

		m_AryAcce.Add(EqSpec);
	}
}

void UEquipManager::EquipAll()
{
	int index = 0;
	for (FAccessorySpec& Spec : m_AryAcce)
	{
		if (Spec.m_nIsEquipped == 1)
		{
			PRINTF("1A:%d,B:%d",Spec.m_nIsEquipped,index);
			TryEquipAccessory1(index);
		}
		else if (Spec.m_nIsEquipped == 2)
		{
			PRINTF("2A:%d,B:%d",Spec.m_nIsEquipped,index);
			TryEquipAccessory2(index);
		}

		index++;
	}

	index = 0;

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
		if (Spec.m_nIsEquipped)
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

void UEquipManager::SetEquipDataFromServer(const FString& classSkin, const FString& weapon, const FString& wing,
                                           const FString& pet, const FString& acce)
{
	SetStringSkinUnlocked(classSkin);
	SetStringWingUnlocked(wing);
	SetStringWeaponUnlocked(weapon);
	SetStringPetUnlocked(pet);
	SetStringAccesoryUnlocked(acce);
	//equip need
	//EquipAll();
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
		m_AryPlayerSkin[m_nSelectedSkin].m_nIsEquipped = false;
	}

	m_AryPlayerSkin[index].m_nIsEquipped = true;

	Cast<APlayerDiabloCharacter>(UGameplayStatics::GetPlayerPawn(UDiabloGameInstance::Get->GetWorld(), 0))->
		PlayerClassDataInject(m_AryPlayerSkin[index]);

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

	Cast<APlayerDiabloCharacter>(UGameplayStatics::GetPlayerPawn(UDiabloGameInstance::Get->GetWorld(), 0))->
		WeaponDataInject(m_AryWeapons[index]);

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

	Cast<APlayerDiabloCharacter>(UGameplayStatics::GetPlayerPawn(UDiabloGameInstance::Get->GetWorld(), 0))->
		WingDataInject(m_AryWings[index]);

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

	Cast<APlayerDiabloCharacter>(UGameplayStatics::GetPlayerPawn(UDiabloGameInstance::Get->GetWorld(), 0))->
		PetDataInject(m_AryPets[index]);

	m_OnPetChanged.Broadcast(m_nSelectedPet, index);

	m_nSelectedPet = index;
}

void UEquipManager::TryEquipAccessory1(int index)
{
	if (m_nSelectedAccessory1 == index)
	{
		PRINTF("The Accessory Is Same");

		return;
	}

	if (m_AryAcce.Num() <= index)
	{
		return;
	}

	UDiabloGameInstance* GameInst = UDiabloGameInstance::Get;

	APlayerDiabloCharacter* DiaChar = Cast<APlayerDiabloCharacter>(
		UGameplayStatics::GetPlayerPawn(GameInst->GetWorld(), 0));

	ADiabloPlayerController* DiaCon = Cast<ADiabloPlayerController>(
		UGameplayStatics::GetPlayerController(GameInst->GetWorld(), 0));

	if (m_nSelectedAccessory2 == index)
	{
		m_AryAcce[m_nSelectedAccessory2].m_nIsEquipped = -1;

		if (m_AcceSpec2) //in first equip theres no acce
		{
			m_AcceSpec2->RemoveAccessory(GameInst, DiaCon, DiaChar);
			m_AcceSpec2 = nullptr;
		}

		m_nSelectedAccessory2 = - 1;
	}

	if (m_nSelectedAccessory1 > -1)
	{
		m_AryAcce[m_nSelectedAccessory1].m_nIsEquipped = -1;

		if (m_AcceSpec1) //for first equip
		{
			m_AcceSpec1->RemoveAccessory(GameInst, DiaCon, DiaChar);
			m_AcceSpec1 = nullptr;
		}
	}

	m_AryAcce[index].m_nIsEquipped = 1;
	m_AcceSpec1 = NewObject<UAccessoryOption>(m_AryAcce[index].m_AccessoryData->m_ClassAccessoryOp);
	m_AcceSpec1->ApplyAccessory(GameInst, DiaCon, DiaChar);

	Cast<APlayerDiabloCharacter>(UGameplayStatics::GetPlayerPawn(UDiabloGameInstance::Get->GetWorld(), 0))->
		AccessoryDataInject(m_AryAcce[index]);

	m_OnAccessoryChanged1.Broadcast(m_nSelectedAccessory1, index);

	m_nSelectedAccessory1 = index;
}

void UEquipManager::TryEquipAccessory2(int index)
{
	if (m_nSelectedAccessory2 == index)
	{
		PRINTF("The Accessory Is Same");

		return;
	}

	if (m_AryAcce.Num() <= index)
	{
		return;
	}

	UDiabloGameInstance* GameInst = UDiabloGameInstance::Get;

	APlayerDiabloCharacter* DiaChar = Cast<APlayerDiabloCharacter>(
		UGameplayStatics::GetPlayerPawn(GameInst->GetWorld(), 0));

	ADiabloPlayerController* DiaCon = Cast<ADiabloPlayerController>(
		UGameplayStatics::GetPlayerController(GameInst->GetWorld(), 0));


	if (m_nSelectedAccessory1 == index)
	{
		m_AryAcce[m_nSelectedAccessory1].m_nIsEquipped = -1;

		if (m_AcceSpec1) //in first equip theres no acce
		{
			m_AcceSpec1->RemoveAccessory(GameInst, DiaCon, DiaChar);
			m_AcceSpec1 = nullptr;
		}

		m_nSelectedAccessory1 = - 1;
	}

	if (m_nSelectedAccessory2 > -1)
	{
		m_AryAcce[m_nSelectedAccessory2].m_nIsEquipped = -1;

		if (m_AcceSpec2) //in first equip theres no acce
		{
			m_AcceSpec2->RemoveAccessory(GameInst, DiaCon, DiaChar);
			m_AcceSpec2 = nullptr;
		}
	}

	m_AryAcce[index].m_nIsEquipped = 2;

	m_AcceSpec2 = NewObject<UAccessoryOption>(m_AryAcce[index].m_AccessoryData->m_ClassAccessoryOp);
	m_AcceSpec2->ApplyAccessory(GameInst, DiaCon, DiaChar);

	Cast<APlayerDiabloCharacter>(UGameplayStatics::GetPlayerPawn(UDiabloGameInstance::Get->GetWorld(), 0))->
		AccessoryDataInject(m_AryAcce[index]);

	m_OnAccessoryChanged1.Broadcast(m_nSelectedAccessory2, index); //??

	m_nSelectedAccessory2 = index;
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

	m_OnAccessoryChanged1.Broadcast(-1, index);
	//m_OnAccessoryChanged2.Broadcast(-1,index);

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
	if (m_AryWeapons[index].m_nLv >= 200)
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
	if (m_AryPets[index].m_nLv >= 200)
	{
		return false;
	}
	m_AryPets[index].m_nLv++;
	m_AryPets[index].SetLevel(m_AryPets[index].m_nLv);

	m_OnPetChanged.Broadcast(-1, index);

	return true;
}
