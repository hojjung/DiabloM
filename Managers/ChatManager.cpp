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

	SetBadWordAry();

	m_fDeltaCounter=0.f;
}

void UChatManager::SetBadWordAry()
{
	m_AryBadwordList.Reset();
	m_AryBadwordList.Add("Fuck");
	m_AryBadwordList.Add("Fuk");
	m_AryBadwordList.Add("씨발");
	m_AryBadwordList.Add("시발");
	m_AryBadwordList.Add("씨1발");
	m_AryBadwordList.Add("시1발");
	m_AryBadwordList.Add("병신");
	m_AryBadwordList.Add("병1신");
	m_AryBadwordList.Add("새끼");
	m_AryBadwordList.Add("새1끼");
	m_AryBadwordList.Add("좆");
	m_AryBadwordList.Add("좃");
	m_AryBadwordList.Add("씹");
	m_AryBadwordList.Add("십새");
	
}

void UChatManager::HttpCall(const FString& URL, FString Type,FString* formStrData)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UChatManager::OnResponseReceived);

	Request->SetURL(URL);
	Request->SetVerb(Type);

	if(formStrData)
	{
		FString NewStr =*formStrData;
		Request->SetContentAsString(NewStr);
	}
	
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", "application/x-www-form-urlencoded ; charset=utf-8");
	
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
	m_fDeltaCounter=0.f;
}

void UChatManager::FilterBadWord(FString& outChatWant)
{
	for(FString& BadWord :m_AryBadwordList)
	{
		outChatWant.Replace(*BadWord,TEXT("*"));
	}
}

void UChatManager::Tick(float deltaTime)
{
	m_fDeltaCounter+=deltaTime;

	if(m_fDeltaCounter>3.5f)
	{
		if(m_bIsWaitingGetChatRequest)
		{
			return;
		}
		RequestGetChatFromServer();
	}
}

void UChatManager::ChatPost(const FText& chatWant)
{
	FText CachedText = chatWant;

	CachedText = UKismetTextLibrary::TextTrimPrecedingAndTrailing(CachedText);
	
	FString CachedString = CachedText.ToString(); 
	
	//FilterBadWord(CachedString);

	FString Format = FString::Printf(TEXT("nickname=%s&chat=%s"),*UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname,*CachedString);

	HttpCall(WebURL,"POST",&Format);
}
