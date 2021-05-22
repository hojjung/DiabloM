// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "UObject/NoExportTypes.h"
#include "InboxData.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UInboxData : public UObject
{
	GENERATED_BODY()
	
};


USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FInboxRewardTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_TextRewardName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* m_RewardIcon;
};