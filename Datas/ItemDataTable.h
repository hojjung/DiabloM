// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
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
enum class EItemType :uint8//아이템 사용을 뜻함
{
	Misc,
	Equipment,
	Consumable,
	Length
};

UENUM(BlueprintType)
enum class EItemThrowType :uint8
{
	CantThrow,
	DefaultThrow,//공격할수 없는 물건을 던질때
	AttackThrow,//공격용 물건을 던짐
	GiveThrow,
	Length
};



USTRUCT(BlueprintType)//난이도,티어
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FItemData()
	{
		m_NameID = NAME_None;
		m_ItemType = EItemType::Misc;
		m_bStackable = true;
		m_nInitStack = 1;
		m_nMaxStack = 99;//(B=242,G=242,R=242,A=255)
		m_Color = FColor(242, 242, 242, 255);
	}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName m_NameID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemType m_ItemType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool m_bStackable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nInitStack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nMaxStack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* m_ItemMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor m_Color;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture* m_ItemIcon;
};



USTRUCT(BlueprintType)//난이도,티어
struct FEquipmentColorTable : public FTableRowBase
{
	GENERATED_BODY()


	public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FColor m_Color;
};


USTRUCT(BlueprintType)
struct FItemInstance
{
	GENERATED_BODY()

public:
	FItemInstance()
	{
		m_ItemData = nullptr;
		m_nCurrentStack = -1;
		m_nGridIndex = -1;
	}
	FItemInstance(const FItemData* itemData, int gridIndex)
	{
		m_ItemData = itemData;
		m_nCurrentStack = m_ItemData->m_nInitStack;
		m_nGridIndex = gridIndex;
	}

public:
	int m_nCurrentStack;
	int m_nGridIndex;
	const FItemData* m_ItemData;

public:
	void SetGridNewIndex(int newIndex)
	{
		m_nGridIndex = newIndex;
	}//드래그 드랍
	bool AddStack()
	{
		if (!CheckCanStack())
		{
			return false;
		}

		m_nCurrentStack++;

		return true;
	}

	bool CheckCanStack() const
	{
		return m_nCurrentStack < m_ItemData->m_nMaxStack;
	}//스텍 되는애가 최대치 이하인지
	bool GetIsStackable() const
	{
		return m_ItemData->m_bStackable;
	}//근본적으로 스택이 되는지
	int RemoveStack()
	{
		return --m_nCurrentStack;
	}//리턴값이 0보다 작으면,밖에서 삭제할것

	void ClearData()
	{
		m_ItemData = nullptr;
		m_nCurrentStack = -1;
	}
};
