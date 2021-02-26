#include "EquipManager.h"

#include "DiabloGameInstance.h"
#include "PlayfabManager.h"
#include "Characters/PlayerDiabloCharacter.h"


UDataTable* UEquipManager::GetWeaponDataTable = nullptr;
UDataTable* UEquipManager::GetWingDataTable = nullptr;
UDataTable* UEquipManager::GetPetDataTable = nullptr;
UDataTable* UEquipManager::GetAcceeDataTable = nullptr;

UEquipManager::UEquipManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable(
          TEXT("DataTable'/Game/DataTables/Equipments/WeaponDataTable.WeaponDataTable'"));
	GetWeaponDataTable = FoundEntityTable.Object;
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable1(
          TEXT("DataTable'/Game/DataTables/Equipments/WingDataTable.WingDataTable'"));
	GetWingDataTable = FoundEntityTable1.Object;
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable2(
          TEXT("DataTable'/Game/DataTables/Equipments/PetDataTable.PetDataTable'"));
	GetPetDataTable = FoundEntityTable2.Object;
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable3(
          TEXT("DataTable'/Game/DataTables/Equipments/AccessoryDataTable.AccessoryDataTable'"));
	GetAcceeDataTable = FoundEntityTable3.Object;
	//DataTable'/Game/DataTables/Equipments/WingDataTable.WingDataTable'
	//DataTable'/Game/DataTables/Equipments/PetDataTable.PetDataTable'
	//DataTable'/Game/DataTables/Equipments/AccessoryDataTable.AccessoryDataTable'
}


void UEquipManager::StringToIntAry(const FString& skinUnlock, TArray<int>& outContent) const
{
	outContent.SetNum(skinUnlock.Len());
	memcpy(outContent.GetData(), TCHAR_TO_ANSI(*skinUnlock), skinUnlock.Len());
}

int UEquipManager::StringSplitEachItem(const FString& equipDatas, TArray<FString>& outStrAry) const
{
	return equipDatas.ParseIntoArray(outStrAry,TEXT("/"));
}

void UEquipManager::SetStringSkinUnlocked(FString skinUnlock)//이 str에 모든 스킨정보가 등록되어있음,근데 순서를 어떻게 보장시킴?
{
	//내가 상수로 업데이트 시켰기때문에 순서자체가 스킨 데이터 순서임.
	//숫자 01234를 데이터 테이블의 키값으로 사용해야하나
	UCharacterDataTable::GetPlayerEntityTable->GetAllRows("",m_AryPlayerClass);
	
	m_AryPlayerSkin.Reset();
	
	TArray<FString> AryEachDatas;
	
	StringSplitEachItem(skinUnlock,AryEachDatas);

	int Len = m_AryPlayerClass.Num();
	
	for(int i=0; i< m_AryPlayerClass.Num();i++)
	{
		FPlayerClassSpec PlSpec;
		PlSpec.m_PlayerData = m_AryPlayerClass[i];
		PlSpec.ParseFromString(AryEachDatas[i]);

		m_AryPlayerSkin.Add(PlSpec);

		if(m_AryPlayerSkin[i].m_nEquippedSlot>0)
		{
			m_CurrentSelectedSkin = &m_AryPlayerSkin[i];		
		}
	}
}


void UEquipManager::SetStringWingUnlocked(FString wingUnlock)
{
	m_AryWings.Reset();
	
	UEquipManager::GetWingDataTable->GetAllRows("",m_AryEquipDatas);
	
	TArray<FString> AryEachDatas;
	
	int Len = StringSplitEachItem(wingUnlock,AryEachDatas);
	
	for(int i=0; i< m_AryEquipDatas.Num();i++)
	{
		FEquipmentSpec EqSpec;
		EqSpec.m_EquipData = m_AryEquipDatas[i];
		EqSpec.ParseFromString(AryEachDatas[i]);
		m_AryWings.Add(EqSpec);
	}
}

void UEquipManager::SetStringWeaponUnlocked(FString weaponUnlock)
{
	m_AryWeapons.Reset();
	
	UEquipManager::GetWeaponDataTable->GetAllRows("",m_AryWeaponDatas);
	
	TArray<FString> AryEachDatas;
	
	int Len = StringSplitEachItem(weaponUnlock,AryEachDatas);
	
	for(int i=0; i< m_AryWeaponDatas.Num();i++)
	{
		FEquipmentSpec EqSpec;
		EqSpec.m_EquipData = m_AryWeaponDatas[i];
		EqSpec.ParseFromString(AryEachDatas[i]);

		m_AryWeapons.Add(EqSpec);
	}
}

void UEquipManager::SetStringPetUnlocked(FString petUnlock)
{
	m_AryPets.Reset();
	
	UEquipManager::GetPetDataTable->GetAllRows("",m_AryPetDatas);
	
	TArray<FString> AryEachDatas;
	
	int Len = StringSplitEachItem(petUnlock,AryEachDatas);
	
	for(int i=0; i< m_AryPetDatas.Num();i++)
	{
		FEquipmentSpec EqSpec;
		EqSpec.m_EquipData = m_AryPetDatas[i];
		EqSpec.ParseFromString(AryEachDatas[i]);

		m_AryPets.Add(EqSpec);
	}
}

void UEquipManager::SetStringAccesoryUnlocked(FString acceUnlock)
{
	m_AryAcce.Reset();
	
	UEquipManager::GetAcceeDataTable->GetAllRows("",m_AryAccessDatas);
	
	TArray<FString> AryEachDatas;
	
	int Len = StringSplitEachItem(acceUnlock,AryEachDatas);
	
	for(int i=0; i< m_AryAccessDatas.Num();i++)
	{
		FEquipmentSpec EqSpec;
		EqSpec.m_EquipData = m_AryAccessDatas[i];
		EqSpec.ParseFromString(AryEachDatas[i]);

		m_AryAcce.Add(EqSpec);
	}
}

void UEquipManager::SetEquipDataFromServer(const FString& classSkin,const FString& weapon,const FString& wing,const FString& pet,const FString& acce)
{
	//	
	SetStringSkinUnlocked(classSkin);
	SetStringWingUnlocked(wing);
	SetStringWeaponUnlocked(weapon);
	SetStringPetUnlocked(pet);
	SetStringAccesoryUnlocked(acce);
}

const FPlayerClassSpec* UEquipManager::TryEquipSkin(const FPlayerClassSpec* player_class_spec)
{
	if(m_CurrentSelectedSkin == player_class_spec)
	{
		PRINTF("The Skin Is Same");
		
		return nullptr;
	}

	const FPlayerClassSpec* Removed = m_CurrentSelectedSkin;

	m_CurrentSelectedSkin = player_class_spec;
	
	Cast<APlayerDiabloCharacter>( UGameplayStatics::GetPlayerPawn(UDiabloGameInstance::Get->GetWorld(),0))->PlayerClassDataInject(this);

	return Removed;
}

void UEquipManager::TryEquipEquipment(const FEquipmentSpec* equipment_spec)
{
	
}
