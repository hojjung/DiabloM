#include "GachaManager.h"

#include "DiabloGameInstance.h"
#include "Datas/GachaDataTable.h"

UDataTable* UGachaManager::AryWeaponGachaDataTable[8];
UDataTable* UGachaManager::ArySkinGachaDataTable[8];
UDataTable* UGachaManager::PetGachaDataTable = nullptr;
UDataTable* UGachaManager::WingGachaDataTable = nullptr;
UDataTable* UGachaManager::AccessoryGachaDataTable = nullptr;


UGachaManager::UGachaManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundGacha01(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaWeapon01.GachaWeapon01'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundGacha02(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaWeapon02.GachaWeapon02'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundGacha03(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaWeapon03.GachaWeapon03'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundGacha04(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaWeapon04.GachaWeapon04'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundGacha05(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaWeapon05.GachaWeapon05'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundGacha06(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaWeapon06.GachaWeapon06'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundGacha07(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaWeapon07.GachaWeapon07'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundGacha08(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaWeapon08.GachaWeapon08'"));
	//
	static ConstructorHelpers::FObjectFinder<UDataTable> Found2Gacha01(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaSkin01.GachaSkin01'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Found2Gacha02(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaSkin02.GachaSkin02'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Found2Gacha03(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaSkin03.GachaSkin03'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Found2Gacha04(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaSkin04.GachaSkin04'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Found2Gacha05(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaSkin05.GachaSkin05'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Found2Gacha06(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaSkin06.GachaSkin06'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Found2Gacha07(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaSkin07.GachaSkin07'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Found2Gacha08(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaSkin08.GachaSkin08'"));
	//
	static ConstructorHelpers::FObjectFinder<UDataTable> Found3Gacha01(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaPet01.GachaPet01'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Found4Gacha01(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaWing01.GachaWing01'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> Found5Gacha01(
		TEXT("DataTable'/Game/DataTables/Gacha/GachaAccessory01.GachaAccessory01'"));
	//
	AryWeaponGachaDataTable[0] = (FoundGacha01.Object);
	AryWeaponGachaDataTable[1] = (FoundGacha02.Object);
	AryWeaponGachaDataTable[2] = (FoundGacha03.Object);
	AryWeaponGachaDataTable[3] = (FoundGacha04.Object);
	AryWeaponGachaDataTable[4] = (FoundGacha05.Object);
	AryWeaponGachaDataTable[5] = (FoundGacha06.Object);
	AryWeaponGachaDataTable[6] = (FoundGacha07.Object);
	AryWeaponGachaDataTable[7] = (FoundGacha08.Object);
	//
	ArySkinGachaDataTable[0] = (Found2Gacha01.Object);
	ArySkinGachaDataTable[1] = (Found2Gacha02.Object);
	ArySkinGachaDataTable[2] = (Found2Gacha03.Object);
	ArySkinGachaDataTable[3] = (Found2Gacha04.Object);
	ArySkinGachaDataTable[4] = (Found2Gacha05.Object);
	ArySkinGachaDataTable[5] = (Found2Gacha06.Object);
	ArySkinGachaDataTable[6] = (Found2Gacha07.Object);
	ArySkinGachaDataTable[7] = (Found2Gacha08.Object);
	//
	PetGachaDataTable = Found3Gacha01.Object;
	WingGachaDataTable = Found4Gacha01.Object;
	AccessoryGachaDataTable = Found5Gacha01.Object;
	//

	for (int i = 0; i < 8; i++)
	{
		SetTotalValue(AryWeaponGachaDataTable[i], m_AryAryGachaWeapon[i], m_AryTotalWeaponGacha[i]);
	}
	//
	for (int i = 0; i < 8; i++)
	{
		SetTotalValue(ArySkinGachaDataTable[i], m_AryAryGachaSkin[i], m_AryTotalSkinGacha[i]);
	}

	SetTotalValue(PetGachaDataTable, m_AryGachaPet, m_fTotalPetGacha);
	//SetTotalValue(WingGachaDataTable, m_AryGachaWing, m_fTotalWingGacha);
	SetTotalValue(AccessoryGachaDataTable, m_AryGachaAccessory, m_fTotalAccessoryGacha);
	//
	m_nGachaWeaponMaxCount[0] = 100;
	m_nGachaWeaponMaxCount[1] = 500;
	m_nGachaWeaponMaxCount[2] = 1000;
	m_nGachaWeaponMaxCount[3] = 1500;
	m_nGachaWeaponMaxCount[4] = 2500;
	m_nGachaWeaponMaxCount[5] = 3000;
	m_nGachaWeaponMaxCount[6] = 3500;
	m_nGachaWeaponMaxCount[7] = 4000;
	//
	m_nGachaSkinMaxCount[0] = 100;
	m_nGachaSkinMaxCount[1] = 500;
	m_nGachaSkinMaxCount[2] = 1000;
	m_nGachaSkinMaxCount[3] = 1500;
	m_nGachaSkinMaxCount[4] = 2500;
	m_nGachaSkinMaxCount[5] = 3000;
	m_nGachaSkinMaxCount[6] = 3500;
	m_nGachaSkinMaxCount[7] = 4000;
}

void UGachaManager::SetGachaLevel(const FString weaponGacha, const FString playerGacha)
{
	TArray<FString> WeaponGachaSpec;

	weaponGacha.ParseIntoArray(WeaponGachaSpec,TEXT(":"));

	TArray<FString> SkinGachaSpec;

	playerGacha.ParseIntoArray(SkinGachaSpec,TEXT(":"));

	m_nCurrentWeaponIndex = FCString::Atoi(*WeaponGachaSpec[0]);
	m_nGachaWeaponCount = FCString::Atoi(*WeaponGachaSpec[1]);
	
	m_nCurrentSkinIndex = FCString::Atoi(*SkinGachaSpec[0]);
	m_nGachaSkinCount = FCString::Atoi(*SkinGachaSpec[1]);
}

FString UGachaManager::GetGachaLevelStr()
{
	return FString::Printf(TEXT("%d:%d/%d:%d"),m_nCurrentWeaponIndex,m_nGachaWeaponCount,m_nCurrentSkinIndex,m_nGachaSkinCount);
}

void UGachaManager::SetTotalValue(const UDataTable* inTable, TArray<FGachaTableRow*>& outTableRow, float& outTotal)
{
	if (!inTable)
	{
		return;
	}
	inTable->GetAllRows<FGachaTableRow>("", outTableRow);

	for (FGachaTableRow* TableRow : outTableRow)
	{
		outTotal += TableRow->m_fPriority;
	}
}

void UGachaManager::AddGachaWeaponCount()
{
	m_nGachaWeaponCount++;

	if (m_nGachaWeaponCount >= m_nGachaWeaponMaxCount[m_nCurrentWeaponIndex])
	{
		if (m_nCurrentWeaponIndex + 1 >= 8)
		{
			return;
		}

		m_nCurrentWeaponIndex++;
		m_nGachaWeaponCount = 0;
	}

	m_OnGachaRollWeapon.Broadcast(m_nGachaWeaponCount, m_nGachaWeaponMaxCount[m_nCurrentWeaponIndex],
	                              m_nCurrentWeaponIndex);
}

void UGachaManager::AddGachaSkinCount()
{
	m_nGachaSkinCount++;

	if (m_nGachaSkinCount >= m_nGachaSkinMaxCount[m_nCurrentSkinIndex])
	{
		if (m_nCurrentSkinIndex + 1 >= 8)
		{
			return;
		}

		m_nCurrentSkinIndex++;
		m_nGachaSkinCount = 0;
	}

	m_OnGachaRollSkin.Broadcast(m_nGachaSkinCount, m_nGachaSkinMaxCount[m_nCurrentSkinIndex], m_nCurrentSkinIndex);
}

TArray<FGachaTableRow*>& UGachaManager::GetCurrentLevelWeaponTable()
{
	return m_AryAryGachaWeapon[m_nCurrentWeaponIndex];
}

TArray<FGachaTableRow*>& UGachaManager::GetCurrentLevelSkinTable()
{
	return m_AryAryGachaSkin[m_nCurrentSkinIndex];
}

float UGachaManager::GetCurrentLevelWeaponTotalPercent()
{
	return m_AryTotalWeaponGacha[m_nCurrentWeaponIndex];
}

float UGachaManager::GetCurrentLevelSkinTotalPercent()
{
	return m_AryTotalSkinGacha[m_nCurrentSkinIndex];
}

const FGachaAbleRow* UGachaManager::RollWeapon()
{
	float WeightTotal = GetCurrentLevelWeaponTotalPercent();
	//0.4
	float RollPercent = FMath::RandRange(0.f, 1.f);

	float CurrentPercent = 0;
	//0.1
	int i = 0;

	for (FGachaTableRow* TableRow : GetCurrentLevelWeaponTable())
	{
		CurrentPercent += TableRow->GetPercent(WeightTotal);

		if (CurrentPercent >= RollPercent)
		{
			const FGachaAbleRow* GachaData = &TableRow->GetGachaData();

			UDiabloGameInstance::Get->m_EquipManager->AddWeaponStack(GachaData);

			AddGachaWeaponCount();

			return GachaData;
		}

		i++;
	}
	

	UDiabloGameInstance::Get->m_EquipManager->AddWeaponStack(i - 1);


	return &GetCurrentLevelWeaponTable()[i - 1]->GetGachaData();
}

const FGachaAbleRow* UGachaManager::RollSkin()
{
	float WeightTotal = GetCurrentLevelSkinTotalPercent();
	//0.4
	float RollPercent = FMath::RandRange(0.f, 1.f);

	float CurrentPercent = 0;
	//0.1
	int i = 0;
	for (FGachaTableRow* TableRow : GetCurrentLevelSkinTable())
	{
		CurrentPercent += TableRow->GetPercent(WeightTotal);

		if (CurrentPercent >= RollPercent)
		{
			const FGachaAbleRow* GachaData = &TableRow->GetGachaData();

			UDiabloGameInstance::Get->m_EquipManager->AddSkinStack(GachaData);

			AddGachaSkinCount();
			//select
			return GachaData;
		}
		i++;
	}


	UDiabloGameInstance::Get->m_EquipManager->AddSkinStack(i - 1);

	return &GetCurrentLevelSkinTable()[i - 1]->GetGachaData();
}

const FGachaAbleRow* UGachaManager::RollPet()
{
	float WeightTotal = m_fTotalPetGacha;
	//0.4
	float RollPercent = FMath::RandRange(0.f, 1.f);

	float CurrentPercent = 0;

	int i = 0;
	//0.1
	for (FGachaTableRow* TableRow : m_AryGachaPet)
	{
		CurrentPercent += TableRow->GetPercent(WeightTotal);

		if (CurrentPercent >= RollPercent)
		{
			UDiabloGameInstance::Get->m_EquipManager->AddPetStack(i);

			//select
			return &TableRow->GetGachaData();
		}

		i++;
	}

	UDiabloGameInstance::Get->m_EquipManager->AddPetStack(i - 1);

	return &m_AryGachaPet[i - 1]->GetGachaData();
}

const FGachaAbleRow* UGachaManager::RollWing()
{
	float WeightTotal = m_fTotalWingGacha;
	//0.4
	float RollPercent = FMath::RandRange(0.f, 1.f);

	float CurrentPercent = 0;

	int i = 0;
	//0.1
	for (FGachaTableRow* TableRow : m_AryGachaWing)
	{
		CurrentPercent += TableRow->GetPercent(WeightTotal);

		if (CurrentPercent >= RollPercent)
		{
			UDiabloGameInstance::Get->m_EquipManager->AddWingStack(i);

			//select
			return &TableRow->GetGachaData();
		}

		i++;
	}

	UDiabloGameInstance::Get->m_EquipManager->AddWingStack(i - 1);

	return &m_AryGachaWing[i - 1]->GetGachaData();
}

const FGachaAbleRow* UGachaManager::RollAccessory()
{
	float WeightTotal = m_fTotalAccessoryGacha;

	float RollPercent = FMath::RandRange(0.f, 1.f);

	float CurrentPercent = 0;

	int i = 0;

	for (FGachaTableRow* TableRow : m_AryGachaAccessory)
	{
		CurrentPercent += TableRow->GetPercent(WeightTotal);

		if (CurrentPercent >= RollPercent)
		{
			const FGachaAbleRow* Data = &TableRow->GetGachaData();
			
			UDiabloGameInstance::Get->m_EquipManager->AddAccessoryStack(Data);

			return Data;
		}

		i++;
	}

	UDiabloGameInstance::Get->m_EquipManager->AddAccessoryStack(i - 1);

	return &m_AryGachaAccessory[i - 1]->GetGachaData();
}

const FGachaAbleRow* UGachaManager::RollItem(ERollItemType type)
{
	UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::GachaCount);
	switch (type)
	{
	case ERollItemType::RollWeapon: return RollWeapon();
	case ERollItemType::RollSkin: return RollSkin();
	case ERollItemType::RollPet: return RollPet();
	case ERollItemType::RollWing: return RollWing();
	case ERollItemType::RollAccessory: return RollAccessory();
	}

	return nullptr;
}

