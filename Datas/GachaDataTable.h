#pragma once

#include "CoreMinimal.h"


#include "ItemTierData.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "GachaDataTable.generated.h"


UCLASS()
class DIABLOM_API UGachaDataTable : public UObject
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FGachaTableRow : public FTableRowBase
{
	GENERATED_BODY()
	//무조건 한개가 뽑힌다
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
   	FText m_ShowingName;
   	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
   	float m_fPriority = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* m_Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
   	FItemTierTableRowHandle m_Handle;

	float GetPercent(float weightTotal) const
	{
		return m_fPriority/weightTotal;
	}
};