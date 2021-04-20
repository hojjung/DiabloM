// My First Hack n Slash


#include "DailyPrizeManager.h"

#include "DiabloGameInstance.h"
#include "JsonSerializer.h"

UDailyPrizeManager::UDailyPrizeManager()
{
	m_AryClaimGemstone.Reserve(25);
	m_AryClaimGemstone.Add(300);
	m_AryClaimGemstone.Add(1500);
	m_AryClaimGemstone.Add(300);
	m_AryClaimGemstone.Add(300);
	m_AryClaimGemstone.Add(2100);
	m_AryClaimGemstone.Add(300);
	m_AryClaimGemstone.Add(3000);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(3600);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(4500);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(5100);
	m_AryClaimGemstone.Add(600);
	m_AryClaimGemstone.Add(600);
	m_AryClaimGemstone.Add(600);
	m_AryClaimGemstone.Add(600);
	m_AryClaimGemstone.Add(6000);
	//
	m_nDDay=-1;
	m_bIsAbleGetDailyPrize=false;
}

void UDailyPrizeManager::SetPrizeManager(const FString& prizeJsonStr,const FDateTime& currentTime,bool bIsFirst)//json?
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(prizeJsonStr);
	
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return;	
	}
	
	FString ClaimTimeStr = JsonObject->GetStringField(TEXT("ClaimTime"));
	//
	FDateTime ClaimTime;
	
	FDateTime::Parse(ClaimTimeStr,ClaimTime);
	
	m_nDDay=JsonObject->GetIntegerField(TEXT("Dday"));

	if(bIsFirst)
	{
		m_bIsAbleGetDailyPrize = true;

		return;
	}
	
	FTimespan DailyRewardTimeSpen = currentTime - ClaimTime;

	int Hours = DailyRewardTimeSpen.GetTotalHours();

	if(Hours<20)
	{
		m_bIsAbleGetDailyPrize = false;

		return;
	}

	m_nDDay++;
	
	int Max = m_AryClaimGemstone.Num();

	if(m_nDDay>=Max)
	{
		m_nDDay=0;
	}


	m_bIsAbleGetDailyPrize = true;
}

void UDailyPrizeManager::Claim()
{
	if(!m_bIsAbleGetDailyPrize)
	{
		return;
	}
	
	m_bIsAbleGetDailyPrize = false;

	UDiabloGameInstance::Get->m_PlayfabManager->UploadDailyData(GetDailyPrizeJsonStr());
	UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(m_AryClaimGemstone[m_nDDay]);
}

FString UDailyPrizeManager::GetDailyPrizeJsonStr()//시작할때 한번만 받아야함
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetNumberField(TEXT("Dday"),m_nDDay); 
	JsonObject->SetStringField(TEXT("ClaimTime"),FDateTime::UtcNow().ToString());

	return PlayFab::FJsonKeeper(JsonObject).toJSONString();
}
