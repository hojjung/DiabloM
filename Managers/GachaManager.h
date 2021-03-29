#pragma once

#include "DiabloM.h"
#include "Datas/GachaDataTable.h"
#include "UObject/NoExportTypes.h"
#include "GachaManager.generated.h"

UENUM()
enum ERollItemType
{
	RollWeapon,
	RollSkin,
	RollPet,
	RollWing,
	RollAccessory,
	Length
};

UCLASS()
class DIABLOM_API UGachaManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnGachaUpdated,int,int,int);//c,m,level

	FOnGachaUpdated m_OnGachaRollWeapon;

	FOnGachaUpdated m_OnGachaRollSkin;
	
	UGachaManager();
	//
	static UDataTable* AryWeaponGachaDataTable[8];
	static UDataTable* ArySkinGachaDataTable[8];
	static UDataTable* PetGachaDataTable;
	static UDataTable* WingGachaDataTable;
	static UDataTable* AccessoryGachaDataTable;

public:
	UPROPERTY()
	int m_nCurrentWeaponIndex;
	UPROPERTY()
	int m_nCurrentSkinIndex;
	UPROPERTY()
	int m_nGachaWeaponCount;
	UPROPERTY()
	int m_nGachaSkinCount;

	int m_nGachaWeaponMaxCount[8];

	int m_nGachaSkinMaxCount[8];
	
	UPROPERTY()
	float m_AryTotalWeaponGacha[8];
	UPROPERTY()
	float m_AryTotalSkinGacha[8];
	UPROPERTY()
	float m_fTotalPetGacha;
	UPROPERTY()
	float m_fTotalWingGacha;
	UPROPERTY()
	float m_fTotalAccessoryGacha;
	
	TArray<FGachaTableRow*> m_AryAryGachaWeapon[8];//use for gacha info
	TArray<FGachaTableRow*> m_AryAryGachaSkin[8];
	TArray<FGachaTableRow*> m_AryGachaPet;
	TArray<FGachaTableRow*> m_AryGachaWing;
	TArray<FGachaTableRow*> m_AryGachaAccessory;


protected:
	void SetTotalValue(const UDataTable* inTable,TArray<FGachaTableRow*>& outTableRow,float& outTotal);

	void AddGachaWeaponCount();
	
	void AddGachaSkinCount();
	
public:
	TArray<FGachaTableRow*>& GetCurrentLevelWeaponTable();
	
	TArray<FGachaTableRow*>& GetCurrentLevelSkinTable();
	//void
	float GetCurrentLevelWeaponTotalPercent();

	float GetCurrentLevelSkinTotalPercent();

public:
	const FGachaAbleRow* RollWeapon();

	const FGachaAbleRow* RollSkin();

	const FGachaAbleRow* RollPet();

	const FGachaAbleRow* RollWing();

	const FGachaAbleRow* RollAccessory();
	
	const FGachaAbleRow* RollItem(ERollItemType type);
	

	int GetGachaSkinMaxCount()
	{
		return m_nGachaSkinMaxCount[m_nCurrentSkinIndex];
	}

	int GetGachaWeaponMaxCount()
	{
		return m_nGachaWeaponMaxCount[m_nCurrentWeaponIndex];
	}
};
