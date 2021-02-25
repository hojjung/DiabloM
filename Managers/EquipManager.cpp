#include "EquipManager.h"
#include "PlayfabManager.h"



UDataTable* UEquipManager::GetWeaponDataTable = nullptr;
UDataTable* UEquipManager::GetWingDataTable = nullptr;
UDataTable* UEquipManager::GetPetDataTable = nullptr;
UDataTable* UEquipManager::GetAcceeDataTable = nullptr;

UEquipManager::UEquipManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable(
          TEXT("DataTable'/Game/DataTables/Equipments/WeaponDataTable.WeaponDataTable'"));
	GetWeaponDataTable = FoundEntityTable.Object;
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
	TArray<const FPlayerEntityTable*> AryPlayerClass;
	
	UCharacterDataTable::GetPlayerEntityTable->GetAllRows("",AryPlayerClass);
	
	m_AryPlayer.Reset();
	
	TArray<FString> AryEachDatas;
	
	int Len = StringSplitEachItem(skinUnlock,AryEachDatas);
	
	for(int i=0; i< Len;i++)
	{
		FPlayerClassSpec PlSpec;
		PlSpec.m_PlayerData = AryPlayerClass[i];
		PlSpec.ParseFromString(AryEachDatas[i]);

		m_AryPlayer.Add(PlSpec);

		if(m_AryPlayer[i].m_nLv>1)
		{
			m_CurrentSelectedSkin = &m_AryPlayer[i];		
		}
	}
}


void UEquipManager::SetStringWingUnlocked(FString wingUnlock)
{
	m_AryWings.Reset();
	
	TArray<const FEquipmentDataRow*> AryEquipDatas;
	
	UEquipManager::GetWingDataTable->GetAllRows("",AryEquipDatas);
	
	TArray<FString> AryEachDatas;
	
	int Len = StringSplitEachItem(wingUnlock,AryEachDatas);
	
	for(int i=0; i< Len;i++)
	{
		FEquipmentSpec EqSpec;
		EqSpec.m_EquipData = AryEquipDatas[i];
		EqSpec.ParseFromString(AryEachDatas[i]);
		m_AryWings.Add(EqSpec);
	}
}

void UEquipManager::SetStringWeaponUnlocked(FString weaponUnlock)
{
	m_AryWeapons.Reset();
	
	TArray<const FEquipmentDataRow*> AryWeaponDatas;
	
	UEquipManager::GetWeaponDataTable->GetAllRows("",AryWeaponDatas);
	
	TArray<FString> AryEachDatas;
	
	int Len = StringSplitEachItem(weaponUnlock,AryEachDatas);
	
	for(int i=0; i< Len;i++)
	{
		FEquipmentSpec EqSpec;
		EqSpec.m_EquipData = AryWeaponDatas[i];
		EqSpec.ParseFromString(AryEachDatas[i]);

		m_AryWeapons.Add(EqSpec);
	}
}

void UEquipManager::SetStringPetUnlocked(FString petUnlock)
{
	m_AryPets.Reset();
	TArray<const FEquipmentDataRow*> AryPetDatas;
	
	UEquipManager::GetPetDataTable->GetAllRows("",AryPetDatas);
	
	TArray<FString> AryEachDatas;
	
	int Len = StringSplitEachItem(petUnlock,AryEachDatas);
	
	for(int i=0; i< Len;i++)
	{
		FEquipmentSpec EqSpec;
		EqSpec.m_EquipData = AryPetDatas[i];
		EqSpec.ParseFromString(AryEachDatas[i]);

		m_AryPets.Add(EqSpec);
	}
}

void UEquipManager::SetStringAccesoryUnlocked(FString acceUnlock)
{
	m_AryAcce.Reset();
	TArray<const FEquipmentDataRow*> AryAccessDatas;
	
	UEquipManager::GetAcceeDataTable->GetAllRows("",AryAccessDatas);
	
	TArray<FString> AryEachDatas;
	
	int Len = StringSplitEachItem(acceUnlock,AryEachDatas);
	
	for(int i=0; i< Len;i++)
	{
		FEquipmentSpec EqSpec;
		EqSpec.m_EquipData = AryAccessDatas[i];
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
