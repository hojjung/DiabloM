// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "Public/Http.h"
#include "UObject/NoExportTypes.h"
#include "ChatManager.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UChatManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnChatReceive, const FString&);
	
	FOnChatReceive m_OnChatReceive;
	
	UChatManager();

protected:
	TArray<FString> m_AryBadwordList;

	bool m_bIsWaitingGetChatRequest;
	
	FString DocsURL;
	
	FString WebURL;
	
	FHttpModule* Http;

	float m_fDeltaCounter;

protected:
	void SetBadWordAry();

    void HttpCall(const FString& URL, FString Type,FString* formStrData=nullptr);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


	void FilterBadWord(FString& outChatWant);

public:
	void Tick(float deltaTime);//should split
	
	void RequestGetChatFromServer();
	
	void ChatPost(const FText& chatWant);

	FString URLEncode(FString url);

	FString URLDecode(FString url);
};


