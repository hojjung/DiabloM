// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "UObject/NoExportTypes.h"
#include "InboxManager.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FInboxSpec 
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString MailName;
	UPROPERTY(EditAnywhere)
    FString MailDesc;
    UPROPERTY(EditAnywhere)
    FString ItemID;
	UPROPERTY(EditAnywhere)
	FString ItemAmount;
	UPROPERTY(EditAnywhere)
	FString ExpireTime;
	
	FString m_StrRemainTime;

	bool m_bIsExpired = false;
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

protected:
	void ClaimItem(FString itemID,int amount);

	void ClaimAllItems();
	
public:
	void SetInboxManager(TArray<FInboxSpec>& inboxSpec);

	void ClaimInbox(int index);
	

	void ClaimAllInbox();

	FORCEINLINE const TArray<FInboxSpec>& GetAryInbox()
	{
		return m_AryInbox;
	}
};
