// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "Objs/Interfaces/ItemHolder.h"
#include "AbilitySystem/AbilityTypes.h"
#include "ConstructorHelpers.h"
#include "Datas/OptionDataTable.h"
#include "ItemDataTable.generated.h"

/**
 * 
 */

UCLASS()
class DIABLOM_API UItemDataTable : public UObject
{
	GENERATED_BODY()
	
public:
	UItemDataTable(const FObjectInitializer& objInit) ;

static	UDataTable* GetOptionTable;//static?

static	UDataTable* GetTierTable;

static	UDataTable* GetDefaultItemTable;

};


UENUM(BlueprintType)
enum class EItemLocation :uint8//������ �ν��Ͻ��� ��ġ
{
	Void,
	Ground,
	Equipment,
	Inventory,
	Stash,
	Length
};

UENUM(BlueprintType)
enum class EItemType :uint8//������ ����� ����
{
	None,
	Misc,
	OneHandSword,
	TwohandSword,
	Dagger,
	Katana,
	Bow,
	Staff,
	Shield,
	Helmet,
	Necklace,
	BodyArmor,
	Belt,
	Leggins,
	Gauntlets,
	ShoulderArmor,
	Ring, 
	Consumable,
	Length
};


USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FItemTier : public FTableRowBase
{
	GENERATED_BODY()

public:
	FItemTier()
	{
		m_ShowingName = FText::FromString("Normal");
		m_TierColor = FColor(242, 242, 242, 255);
		m_nOptionMaxCount = 0;
	}
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor m_TierColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nOptionMaxCount;
};


USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FItemData()
	{
		m_ItemTier.DataTable = UItemDataTable::GetTierTable;
		m_ItemTier.RowName = "Normal";

		m_ItemType = EItemType::Misc;
		m_bStackable = true;
		m_nInitStack = 1;
		m_nMaxStack = 99;

		m_nSellValue = 100;
		m_bEquipable=true;

		m_ItemID="NeedName";
	}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName m_ItemID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle m_ItemTier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDataTableRowHandle> m_Options;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_FlavorText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemType m_ItemType;

	//무기 같은건 소켓과 액터
	//무기는 스켈레탈 스태틱 둘다 있지 않나? 미리 박아놓으면 소켓이고 뭐고 할게 없다
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* m_SkEquipment;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* m_StEquipment;
	//하지만 방어구 또한 어디로 들어갈지 알아야한다.
	//방어구는 아님
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool m_bEquipable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool m_bStackable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nInitStack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nMaxStack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nSellValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* m_ItemMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture* m_ItemIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> m_DefaultPassive;

public:
	const FItemTier& GetItemTier() const
	{
		return *m_ItemTier.GetRow<FItemTier>("");
	}

	const FOption& GetOption(int index) const
	{
		return *m_Options[index].GetRow<FOption>("");
	}
};



USTRUCT(BlueprintType)
struct FOptionValue
{
	GENERATED_BODY()

public:
	FOptionValue()
	{
		m_nIndex = -1;
		m_fValue = 0;
	}

	FOptionValue(int index , float v)
	{
		m_nIndex = index;
		m_fValue = v;
	}
	UPROPERTY(EditAnywhere)
	int m_nIndex;
	UPROPERTY(EditAnywhere)
	float m_fValue;
};


USTRUCT(BlueprintType)
struct FItemInstance
{
	GENERATED_BODY()

public:
	FItemInstance()
	{
		ClearData();
	}

	FItemInstance(const FItemData* itemData, int gridIndex, IItemHolder* holder, TArray<FOptionValue>* aryUseEffect=nullptr)
	{
		m_ItemData=itemData;
		m_ItemID = m_ItemData->m_ItemID;
		m_nCurrentStack = m_ItemData->m_nInitStack;
		m_nGridIndex = gridIndex;
		m_Holder = holder;
		m_nMaxStack = m_ItemData->m_nMaxStack;
		m_bStackable= m_ItemData->m_bStackable;

		if(aryUseEffect)
			m_AryOptions = *aryUseEffect;
	}

public:
	UPROPERTY(EditAnywhere)
	int m_nCurrentStack;
	UPROPERTY(EditAnywhere)
	int m_nGridIndex;
	UPROPERTY(EditAnywhere)
	TArray<FOptionValue> m_AryOptions;
	UPROPERTY(EditAnywhere)
	FName m_ItemID=NAME_None;

	int m_nMaxStack;
	bool m_bStackable;
	
	IItemHolder* m_Holder;

	const FItemData* m_ItemData;
public:

	bool IsValid()
	{
		return m_ItemID.IsValid();
	}

	void SetGridNewIndex(int newIndex)
	{
		m_nGridIndex = newIndex;
	}//�巡�� ���

	bool CheckCanStack() const
	{
		return m_nCurrentStack < m_nMaxStack;
	}//���� �Ǵ¾ְ� �ִ�ġ ��������
	bool GetIsStackable() const
	{
		return m_bStackable;
	}//�ٺ������� ������ �Ǵ���

	void ClearData()
	{
		m_ItemID = NAME_None;
		m_nCurrentStack = -1;
		m_nGridIndex = -1;
		m_nMaxStack=-1;
		m_bStackable=false;
	}

};
