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
#include "ItemDataTable.generated.h"

/**
 * 
 */

UCLASS()
class DIABLOM_API UItemDataTable : public UObject
{
	GENERATED_BODY()
	
};
UENUM(BlueprintType)
enum class EItemLocation :uint8//아이템 인스턴스의 위치
{
	Void,
	Ground,
	Equipment,
	Inventory,
	Stash,
	Length
};

UENUM(BlueprintType)
enum class EItemType :uint8//아이템 사용을 뜻함
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


USTRUCT(BlueprintType)//난이도,티어
struct FItemTier : public FTableRowBase
{
	GENERATED_BODY()

public:
	FItemTier()
	{
		m_ShowingName = FText::FromString("Normal");
		m_TierColor = FColor(242, 242, 242, 255);
	}
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor m_TierColor;
};


USTRUCT(BlueprintType)//난이도,티어
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FItemData()
	{
		static ConstructorHelpers::FObjectFinder<UDataTable> FoundItemTable(TEXT("DataTable'/Game/DataTables/TierTable.TierTable'"));
		if (FoundItemTable.Succeeded())
		{
			
			m_ItemTier.DataTable = FoundItemTable.Object;
			m_ItemTier.RowName = "Normal";
		}
		m_ItemType = EItemType::Misc;
		m_bStackable = true;
		m_nInitStack = 1;
		m_nMaxStack = 99;

		m_nSellValue = 100;
	}

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle m_ItemTier;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_FlavorText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemType m_ItemType;
	
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

	FItemInstance(const FItemData* itemData, int gridIndex, IItemHolder* holder, TArray<float>* aryUseEffect=nullptr)
	{
		m_ItemData = itemData;
		m_nCurrentStack = m_ItemData->m_nInitStack;
		m_nGridIndex = gridIndex;
		m_Holder = holder;


		m_AryEffectScale.Init(1, 1.f);

		if(aryUseEffect)
			m_AryEffectScale = *aryUseEffect;
	}

public:
	int m_nCurrentStack;
	int m_nGridIndex;
	const FItemData* m_ItemData;
	TArray<float> m_AryEffectScale;
	IItemHolder* m_Holder;

public:

	bool IsValid()
	{
		return m_ItemData;
	}

	void SetGridNewIndex(int newIndex)
	{
		m_nGridIndex = newIndex;
	}//드래그 드랍

	bool CheckCanStack() const
	{
		return m_nCurrentStack < m_ItemData->m_nMaxStack;
	}//스텍 되는애가 최대치 이하인지
	bool GetIsStackable() const
	{
		return m_ItemData->m_bStackable;
	}//근본적으로 스택이 되는지

	void ClearData()
	{
		m_ItemData = nullptr;
		m_nCurrentStack = -1;
		m_nGridIndex = -1;
	}

};
