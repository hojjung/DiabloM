#pragma once

#include "DiabloM.h"
#include "UObject/NoExportTypes.h"
#include "ItemTierData.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UItemTierData : public UObject
{
	GENERATED_BODY()
public:
	static UDataTable* GetTierDataTable;

	UItemTierData()
	{
		static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/DataTables/ItemTierData.ItemTierData'"));
	
		GetTierDataTable = Found.Object;
	}
};
USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FItemTierTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor m_TierColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_TextTierName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool m_bHasSpecialEffect = false;
};

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FItemTierTableRowHandle : public FDataTableRowHandle
{
	GENERATED_BODY()

public:
	FItemTierTableRowHandle()
	{
		DataTable = UItemTierData::GetTierDataTable;
	}
};