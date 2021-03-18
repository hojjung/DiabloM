#include "ChatManager.h"

#include "DiabloGameInstance.h"
#include "Http.h"
#include "JsonObject.h"
#include "JsonObjectConverter.h"
#include "JsonSerializer.h"
// Sets default values
UChatManager::UChatManager()
{
	m_bIsWaitingGetChatRequest = false;

	Http = &FHttpModule::Get();

	DocsURL =
		"https://docs.google.com/spreadsheets/d/18XUGwRb4DYjyJEpBZL0H3FqvlcDedE8icg7WJ_JNdD0/export?format=tsv&range=B:B";

	WebURL =
		"https://script.google.com/macros/s/AKfycbxwh-4ylp9wH6lHTU73NDPSD2ls0asPdzcHuuVpXmIRtigYYtAeq6Bx0CWp0UKldH9X/exec";

	SetBadWordAry();

	m_fDeltaCounter = 0.f;
}

void UChatManager::SetBadWordAry()
{
	m_AryBadwordList.Reset();
	m_AryBadwordList.Add(TEXT("Fuck"));
	m_AryBadwordList.Add(TEXT("Fuk"));
	m_AryBadwordList.Add(TEXT("씨발"));
	m_AryBadwordList.Add(TEXT("시발"));
	m_AryBadwordList.Add(TEXT("씨1발"));
	m_AryBadwordList.Add(TEXT("시1발"));
	m_AryBadwordList.Add(TEXT("병신"));
	m_AryBadwordList.Add(TEXT("병1신"));
	m_AryBadwordList.Add(TEXT("새끼"));
	m_AryBadwordList.Add(TEXT("새1끼"));
	m_AryBadwordList.Add(TEXT("좆"));
	m_AryBadwordList.Add(TEXT("좃"));
	m_AryBadwordList.Add(TEXT("씹"));
	m_AryBadwordList.Add(TEXT("십새"));
}

void UChatManager::HttpCall(const FString& URL, FString Type, FString* formStrData)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UChatManager::OnResponseReceived);

	Request->SetURL(URL);
	Request->SetVerb(Type);

	if (formStrData)
	{
		FString NewStr = *formStrData;

		NewStr = URLEncode(NewStr);

		Request->SetContentAsString(NewStr);
	}


	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");
	//application/x-www-form-urlencoded 
	//"Content-Type" content = "text / html; charset = utf-8"
	//text/plain
	Request->ProcessRequest();
}

void UChatManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (Request->GetVerb()=="GET") //for split from chat post
	{
		m_bIsWaitingGetChatRequest = false;

		if (bWasSuccessful)
			m_OnChatReceive.Broadcast(Response->GetContentAsString());
	}
	else if (Request->GetVerb()=="POST")
	{
		
	}
}

void UChatManager::RequestGetChatFromServer()
{
	HttpCall(DocsURL, "GET");
	m_bIsWaitingGetChatRequest = true;
	m_fDeltaCounter = 0.f;
}

void UChatManager::FilterBadWord(FString& outChatWant)
{
	for (FString& BadWord : m_AryBadwordList)
	{
		outChatWant = outChatWant.Replace(*BadWord,TEXT("*"));
	}
}

void UChatManager::Tick(float deltaTime)
{
	m_fDeltaCounter += deltaTime;

	if (m_fDeltaCounter > 0.7f)
	{
		if (m_bIsWaitingGetChatRequest)
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

	FilterBadWord(CachedString);

	FString FormatStr = FString::Printf(
		TEXT("ranking=%d&nickname=%s&chat=%s"), UDiabloGameInstance::Get->m_PlayfabManager->m_nRanking,
		*UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname, *CachedString);

	HttpCall(WebURL, "POST", &FormatStr);

}

FString UChatManager::URLEncode(FString url)
{
	FString dst;
	char hex[] = "0123456789ABCDEF";
	TArray<ANSICHAR> ANSIArray;

	int32 SrcLen = TCString<ANSICHAR>::Strlen(TCHAR_TO_UTF8(*url)) + 1;
	ANSIArray.AddUninitialized(SrcLen);

	FPlatformString::Convert(ANSIArray.GetData(), SrcLen, TCHAR_TO_UTF8(*url), SrcLen);

	int32 index = 0;

	for (size_t i = 0; i < ANSIArray.Num(); ++i)
	{
		if (isascii(ANSIArray[i])
			&& ANSIArray[i] != '[' && ANSIArray[i] != ']'
			&& ANSIArray[i] != '{' && ANSIArray[i] != '}'
			&& ANSIArray[i] != '\"')
		{
			dst.AppendChar(ANSIArray[i]);
		}
		else
		{
			uint8 c(ANSIArray[i]);
			dst.AppendChar('%');
			dst.AppendChar(hex[c / 16]);
			dst.AppendChar(hex[c % 16]);
		}
	}
	return (dst);
}

FString UChatManager::URLDecode(FString url)
{
	TArray<ANSICHAR> returnStr1;
	//char* s = new char[url.Len()];
	//FMemory::Memset(s, 0, url.Len());
	//int32 index2 = 0;
	for (int32 index = 0; index < url.Len(); index++)
	{
		FString tempStr("");
		if (url[index] != '%')
		{
			returnStr1.Add(url[index]);
			//s[index2] = url[index];
		}
		else if (isxdigit(url[index + 1]) && isxdigit(url[index + 2]))
		{
			tempStr += url[index + 1];
			tempStr += url[index + 2];
			int32 hex = FCString::Strtoi(*tempStr, 0, 16);


			returnStr1.Add(ANSICHAR(hex));
			//s[index2] = char(hex);

			index += 2;
		}
		else
		{
			returnStr1.Add('%');
			//s[index2] = '%';
		}
		//index2++;
	}
	returnStr1.Add('\0');
	FString str = UTF8_TO_TCHAR(returnStr1.GetData());
	//delete[]s;
	return str;
}

