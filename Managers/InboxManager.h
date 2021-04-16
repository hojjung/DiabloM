// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InboxManager.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FInboxSpec 
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	FString MailName;
	UPROPERTY(EditAnywhere)
    FString MailDesc;
    UPROPERTY(EditAnywhere)
    FString ItemID;
	UPROPERTY(EditAnywhere)
	FString ExpireTime;
};

UCLASS()
class DIABLOM_API UInboxManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnInboxUpdated,const TArray<FInboxSpec>&);
	FOnInboxUpdated m_OnInboxUpdated;
protected:
	TArray<FInboxSpec> m_AryInbox;
	
public:
	void SetInboxManager(TArray<FInboxSpec>& inboxSpec);

	void RefreshInbox();
};
