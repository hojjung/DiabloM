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
	FString DocsURL;
	
	FString WebURL;
	
	FHttpModule* Http;
	// Sets default values for this actor's properties
	UChatManager();

    void HttpCall(const FString& URL, FString Type,FString* jsonData=nullptr);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void Init();

	void ChatPost();
	
};


