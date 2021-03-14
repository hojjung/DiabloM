#include "ChatManager.h"
#include "Http.h"
#include "JsonObject.h"
#include "JsonObjectConverter.h"
#include "JsonSerializer.h"
// Sets default values
UChatManager::UChatManager()
{
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
	

	if(jsonData)
	{
		//Request->SetContentAsString(*jsonData);
	}

	if(Type=="POST")
	{
		//Request->SetHeader("Host", "127.0.0.1:5000");
		//Request->SetHeader("Authorization", Authorization.c_str());
	}
	
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	//Request->SetHeader("Content-Type", TEXT("multipart/form-data"));
	//Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));
	Request->ProcessRequest();

}

void UChatManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		PRINTF("111:Success");
	}
	else
	{
		PRINTF("111:Fail");
	}
	
	PRINTF("222: %s", *Response->GetContentAsString());
}

void UChatManager::Init()
{
	PRINTF("TryGet TextData");
	HttpCall(DocsURL,"GET");
	PRINTF("TrySend TextData");
	ChatPost();
}

void UChatManager::ChatPost()
{
	FString NN = "ASD";
	FString CC = "CCC";
	
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UChatManager::OnResponseReceived);

	Request->SetURL(WebURL);
	Request->SetVerb("POST");
	//
	Request->SetHeader("Content-Type", "application/x-www-form-urlencoded");
        
	Request->SetContentAsString("nickname=123&chat=444");// chat=777&
	//
	Request->ProcessRequest();
}
