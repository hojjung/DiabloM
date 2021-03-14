#include "ChatManager.h"

#include "DiabloGameInstance.h"
#include "Http.h"
#include "JsonObject.h"
#include "JsonObjectConverter.h"
#include "JsonSerializer.h"
// Sets default values
UChatManager::UChatManager()
{
	m_bIsWaitingGetChatRequest= false;
	
	Http = &FHttpModule::Get();
	
	DocsURL= "https://docs.google.com/spreadsheets/d/18XUGwRb4DYjyJEpBZL0H3FqvlcDedE8icg7WJ_JNdD0/export?format=tsv&range=B:B";
	
	WebURL = "https://script.google.com/macros/s/AKfycbz9wNohotr_mFU9aQrDdguybBzwojleMe-55lzcVQHvlrfRvNJTNS-z8N9p8ae2mjk/exec";
}

void UChatManager::HttpCall(const FString& URL, FString Type,FString* jsonData)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UChatManager::OnResponseReceived);

	Request->SetURL(URL);
	Request->SetVerb(Type);
	
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", "application/x-www-form-urlencoded");
	
	Request->ProcessRequest();
}

void UChatManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		if(m_bIsWaitingGetChatRequest)//for split from chat post
		{
			m_bIsWaitingGetChatRequest=false;

			m_OnChatReceive.Broadcast(Response->GetContentAsString());
		}
	}
	
}

void UChatManager::RequestGetChatFromServer()
{
	HttpCall(DocsURL,"GET");
	m_bIsWaitingGetChatRequest = true;
}


void UChatManager::ChatPost(FString& chatWant)
{
	FString NewStr = FString::Printf(TEXT("nickname=%s&chat=%s"),*UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname,*chatWant);

	HttpCall(WebURL,"POST",&NewStr);
}
