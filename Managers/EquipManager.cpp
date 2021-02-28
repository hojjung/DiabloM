#include "EquipManager.h"

#include "DiabloGameInstance.h"
#include "PlayfabManager.h"
#include "Characters/PlayerDiabloCharacter.h"

UDataTable* UEquipManager::GetPlayerSkinDataTable = nullptr;
UDataTable* UEquipManager::GetWeaponDataTable = nullptr;
UDataTable* UEquipManager::GetWingDataTable = nullptr;
UDataTable* UEquipManager::GetPetDataTable = nullptr;
UDataTable* UEquipManager::GetAcceeDataTable = nullptr;

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

	m_nSelectedSkin = -1;
	m_nSelectedWing= -1;
	m_nSelectedWeapon= -1;
	m_nSelectedPet= -1;
	m_nSelectedAccessory1= -1;
	m_nSelectedAccessory2= -1;
}

int UEquipManager::StringSplitEachItem(const FString& equipDatas, TArray<FString>& outStrAry) const
{
	return equipDatas.ParseIntoArray(outStrAry,TEXT("/"));
}

void UEquipManager::SetStringSkinUnlocked(FString skinUnlock)//이 str에 모든 스킨정보가 등록되어있음,근데 순서를 어떻게 보장시킴?
{
	TArray<const FPlayerSkinTable*> ArySkins;
	
	UEquipManager::GetPlayerSkinDataTable->GetAllRows("",ArySkins);
	
	m_AryPlayerSkin.Reset();

	
	TArray<FString> AryEachDatas;
	
	StringSplitEachItem(skinUnlock,AryEachDatas);

	for(int i=0; i< ArySkins.Num();i++)
	{
		FPlayerClassSpec PlSpec;
		PlSpec.m_PlayerData = ArySkins[i];
		PlSpec.ParseFromString(AryEachDatas[i]);
		m_AryPlayerSkin.Add(PlSpec);

		if(m_AryPlayerSkin[i].m_nIsEquipped>0)
		{
			m_nSelectedSkin = i;		
		}
	}
}


void UEquipManager::SetStringWingUnlocked(FString wingUnlock)
{
	TArray<const FWingTable*> AryWings;
	
	UEquipManager::GetWingDataTable->GetAllRows("",AryWings);
	
	m_AryWings.Reset();
	
	
	TArray<FString> AryEachDatas;
	
	int Len = StringSplitEachItem(wingUnlock,AryEachDatas);
	
	for(int i=0; i< AryWings.Num();i++)
	{
		FWingSpec EqSpec;
		EqSpec.m_WingData = AryWings[i];
		EqSpec.ParseFromString(AryEachDatas[i]);
		m_AryWings.Add(EqSpec);

		if(m_AryWings[i].m_nIsEquipped>0)
		{
			m_nSelectedWing = i;		
		}
	}
}

void UEquipManager::SetStringWeaponUnlocked(FString weaponUnlock)
{
	TArray<const FWeaponTable*> AryWeapon;
	
	UEquipManager::GetWeaponDataTable->GetAllRows("",AryWeapon);
	
	m_AryWeapons.Reset();
	
	
	TArray<FString> AryEachDatas;
	
	int Len = StringSplitEachItem(weaponUnlock,AryEachDatas);
	
	for(int i=0; i< AryWeapon.Num();i++)
	{
		FWeaponSpec EqSpec;
		EqSpec.m_EquipData = AryWeapon[i];
		EqSpec.ParseFromString(AryEachDatas[i]);
		m_AryWeapons.Add(EqSpec);

		if(m_AryWeapons[i].m_nIsEquipped>0)
		{
			m_nSelectedWeapon = i;		
		}
	}
}

void UEquipManager::SetStringPetUnlocked(FString petUnlock)
{
	TArray<const FPetTable*> AryPet;
	
	UEquipManager::GetPetDataTable->GetAllRows("",AryPet);
	
	m_AryPets.Reset();

	
	TArray<FString> AryEachDatas;
	
	StringSplitEachItem(petUnlock,AryEachDatas);
	
	for(int i=0; i< AryPet.Num();i++)
	{
		FPetSpec EqSpec;
		EqSpec.m_PetData = AryPet[i];
		EqSpec.ParseFromString(AryEachDatas[i]);

		m_AryPets.Add(EqSpec);

		if(m_AryPets[i].m_nIsEquipped>0)
		{
			m_nSelectedPet = i;		
		}
	}
}

void UEquipManager::SetStringAccesoryUnlocked(FString acceUnlock)
{
	TArray<const FAccessoryTable*> AryAcces;
	
	UEquipManager::GetAcceeDataTable->GetAllRows("",AryAcces);
	
	m_AryAcce.Reset();
	
	
	TArray<FString> AryEachDatas;
	
	int Len = StringSplitEachItem(acceUnlock,AryEachDatas);
	
	for(int i=0; i< AryAcces.Num();i++)
	{
		FAccessorySpec EqSpec;
		EqSpec.m_AccessoryData = AryAcces[i];
		EqSpec.ParseFromString(AryEachDatas[i]);

		m_AryAcce.Add(EqSpec);

		if(m_AryAcce[i].m_nIsEquipped==1)
		{
			m_nSelectedAccessory1 = i;		
		}
		else if(m_AryAcce[i].m_nIsEquipped==2)
		{
			m_nSelectedAccessory2 = i;			
		}
	}
}

void UEquipManager::SetEquipDataFromServer(const FString& classSkin,const FString& weapon,const FString& wing,const FString& pet,const FString& acce)
{
	SetStringSkinUnlocked(classSkin);
	SetStringWingUnlocked(wing);
	SetStringWeaponUnlocked(weapon);
	SetStringPetUnlocked(pet);
	SetStringAccesoryUnlocked(acce);
}

void UEquipManager::TryEquipSkin(int index)
{
	if(m_nSelectedSkin == index)
	{
		PRINTF("The Skin Is Same ");
		
		return;
	}

	if(m_AryPlayerSkin.Num() <= index)
	{
		return;
	}

	if(m_nSelectedSkin>-1)
	{
		m_AryPlayerSkin[m_nSelectedSkin].m_nIsEquipped = false;
	}

	m_AryPlayerSkin[index].m_nIsEquipped = true;

	Cast<APlayerDiabloCharacter>( UGameplayStatics::GetPlayerPawn(UDiabloGameInstance::Get->GetWorld(),0))->PlayerClassDataInject(m_AryPlayerSkin[index]);

	m_OnPlSkinChanged.Broadcast(m_nSelectedSkin,index);

	m_nSelectedSkin = index;
}

void UEquipManager::TryEquipWeapon(int index)
{
	if(m_nSelectedWeapon == index)
	{
		PRINTF("The Weapon Is Same");
		
		return;
	}

	if(m_AryWeapons.Num() <= index)
	{
		return;
	}

	if(m_nSelectedWeapon>-1)
	{
		m_AryWeapons[m_nSelectedWeapon].m_nIsEquipped = false;
	}

	m_AryWeapons[index].m_nIsEquipped = true;

	Cast<APlayerDiabloCharacter>( UGameplayStatics::GetPlayerPawn(UDiabloGameInstance::Get->GetWorld(),0))->WeaponDataInject(m_AryWeapons[index]);

	m_OnWeaponChanged.Broadcast(m_nSelectedWeapon,index);

	m_nSelectedWeapon = index;
}

void UEquipManager::TryEquipWing(int index)
{
	if(m_nSelectedWing == index)
	{
		PRINTF("The Wing Is Same");
		
		return;
	}

	if(m_AryWings.Num() <= index)
	{
		return;
	}

	if(m_nSelectedWing>-1)
	{
		m_AryWings[m_nSelectedWing].m_nIsEquipped = false;
	}
	
	m_AryWings[index].m_nIsEquipped = true;

	Cast<APlayerDiabloCharacter>( UGameplayStatics::GetPlayerPawn(UDiabloGameInstance::Get->GetWorld(),0))->WingDataInject(m_AryWings[index]);

	m_OnWingChanged.Broadcast(m_nSelectedWing,index);

	m_nSelectedWing = index;
}

void UEquipManager::TryEquipPet(int index)
{
	if(m_nSelectedPet == index)
	{
		PRINTF("The Pet Is Same");
		
		return;
	}

	if(m_AryPets.Num() <= index)
	{
		return;
	}

	if(m_nSelectedPet>-1)
	{
		m_AryPets[m_nSelectedPet].m_nIsEquipped = false;
	}

	m_AryPets[index].m_nIsEquipped = true;

	Cast<APlayerDiabloCharacter>( UGameplayStatics::GetPlayerPawn(UDiabloGameInstance::Get->GetWorld(),0))->PetDataInject(m_AryPets[index]);

	m_OnPetChanged.Broadcast(m_nSelectedPet,index);

	m_nSelectedPet = index;
}

void UEquipManager::TryEquipAccessory1(int index)
{
	if(m_nSelectedAccessory1 == index)
	{
		PRINTF("The Accessory Is Same");
		
		return;
	}

	if(m_AryAcce.Num() <= index)
	{
		return;
	}

	if(m_nSelectedAccessory1>-1)
	{
		m_AryAcce[m_nSelectedAccessory1].m_nIsEquipped = false;
	}

	m_AryAcce[index].m_nIsEquipped = true;
	

	Cast<APlayerDiabloCharacter>( UGameplayStatics::GetPlayerPawn(UDiabloGameInstance::Get->GetWorld(),0))->AccessoryDataInject(m_AryAcce[index]);

	m_OnAccessoryChanged1.Broadcast(m_nSelectedAccessory1,index);

	m_nSelectedAccessory1 = index;
}

void UEquipManager::TryEquipAccessory2(int index)
{
	if(m_nSelectedAccessory2 == index)
	{
		PRINTF("The Accessory Is Same");
		
		return;
	}

	if(m_AryAcce.Num() <= index)
	{
		return;
	}

	if(m_nSelectedAccessory2>-1)
	{
		m_AryAcce[m_nSelectedAccessory2].m_nIsEquipped = false;
	}

	m_AryAcce[index].m_nIsEquipped = true;
	

	Cast<APlayerDiabloCharacter>( UGameplayStatics::GetPlayerPawn(UDiabloGameInstance::Get->GetWorld(),0))->AccessoryDataInject(m_AryAcce[index]);

	m_OnAccessoryChanged2.Broadcast(m_nSelectedAccessory2,index);

	m_nSelectedAccessory2 = index;
}

