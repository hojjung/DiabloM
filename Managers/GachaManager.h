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
	
	UGachaManager();
	//
	static UDataTable* AryWeaponGachaDataTable[10];
	static UDataTable* ArySkinGachaDataTable[10];
	static UDataTable* PetGachaDataTable;
	static UDataTable* WingGachaDataTable;
	static UDataTable* AccessoryGachaDataTable;

protected:
	UPROPERTY()
	int m_nCurrentWeapon;
	UPROPERTY()
	int m_nCurrentSkin;
	
public:
	UPROPERTY()
	float m_AryTotalWeaponGacha[10];
	UPROPERTY()
	float m_AryTotalSkinGacha[10];
	UPROPERTY()
	float m_fTotalPetGacha;
	UPROPERTY()
	float m_fTotalWingGacha;
	UPROPERTY()
	float m_fTotalAccessoryGacha;
	
	TArray<FGachaTableRow*> m_AryAryGachaWeapon[10];//use for gacha info
	TArray<FGachaTableRow*> m_AryAryGachaSkin[10];
	TArray<FGachaTableRow*> m_AryGachaPet;
	TArray<FGachaTableRow*> m_AryGachaWing;
	TArray<FGachaTableRow*> m_AryGachaAccessory;


protected:
	void SetTotalValue(const UDataTable* inTable,TArray<FGachaTableRow*>& outTableRow,float& outTotal);

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
};
