#include "AdvertiseManager.h"
#include "EasyAdsLibrary.h"
#include "LoadCustomInterstitialAd.h"
#include "LoadCustomRewardedVideoAd.h"
#include "PlayCustomRewardedVideoAd.h"
#include "PlayVideoProxy.h"
#include "ShowCustomInterstitialAd.h"
#include "ShowInterstitialProxy.h"
#include "Managers/DiabloGameInstance.h"

#define ADSCOOL 200.f

FString UAdvertiseManager::GetTimeToStr()
{
	int Minutes = m_fRemainAdmobTime / 60.f;

	int Secs = m_fRemainAdmobTime - (Minutes*60.f);
	
	return FString::Printf(TEXT("쿨다운:%.2i:%.2i"),Minutes,Secs);
}

void UAdvertiseManager::Init()
{
	m_RewardAdID = TEXT("ca-app-pub-3940256099942544/5224354917");
	//
	m_bLoadRewardAdsProcessing=false;

	m_bAdsUpdateLock=false;
}

void UAdvertiseManager::SetTimeCooldownFromServer(FDateTime lastAdmobTime, FDateTime currentLoginTime)
{
	m_fRemainAdmobTime = 0.f;
	//로그인후 클레임시,양수,이후 로그아웃 로그인하면 음수가 되버림
	FTimespan AdmobRemainTime =  lastAdmobTime-currentLoginTime;//이시간은 얼마든지 달라질수있음

	m_fRemainAdmobTime=ADSCOOL-AdmobRemainTime.GetDuration().GetTotalSeconds();

	m_fRemainAdmobTime = FMath::Clamp(m_fRemainAdmobTime,0.f,ADSCOOL);
}

void UAdvertiseManager::Tick(float deltaTime)
{
	m_fRemainAdmobTime-=deltaTime;

	if(m_fRemainAdmobTime>0)
	{
		m_OnAdTick.Broadcast(GetTimeToStr());
	}
	else if(!m_bAdsUpdateLock)
	{
		m_bAdsUpdateLock=true;

		m_OnAdTick.Broadcast(FString());
	}
}

void UAdvertiseManager::ShowBannerAD(bool b)
{
	bool IsPurchased = UDiabloGameInstance::Get->m_ShopManager->GetPackagePurchased(0);
	//
	if (b && (!GetDefault<UPlayFabRuntimeSettings>()->bIsVIPGameVersion) && !IsPurchased)
	{
		UEasyAdsLibrary::ShowBanner(false);
		m_OnShowAdBanner.Broadcast(true);
	}
	else
	{
		UEasyAdsLibrary::HideBanner();

		m_OnShowAdBanner.Broadcast(false);
	}
}

void UAdvertiseManager::ShowRewardAds()
{
	
	if(m_fRemainAdmobTime>0)
	{
		return;
	}
	
	if(m_bLoadRewardAdsProcessing)
	{
	 	return;
	}

	UDiabloGameInstance::Get->GetHud()->ShowTouchBan(1.2f);
	
	m_bLoadRewardAdsProcessing=true;
	
	m_bAdsUpdateLock=false;
	
	m_LoadedRewardAds=ULoadCustomRewardedVideoAd::LoadCustomRewardedVideoAd(m_RewardAdID);
	m_LoadedRewardAds->OnLoadSuccess.AddDynamic(this,&UAdvertiseManager::RewardAdsLoadSuccess);
	m_LoadedRewardAds->OnLoadFail.AddDynamic(this,&UAdvertiseManager::RewardAdsLoadFail);
	m_LoadedRewardAds->Activate();
}

void UAdvertiseManager::RewardAdsLoadSuccess()
{
	
	UPlayCustomRewardedVideoAd* PlayVideoProxy = UPlayCustomRewardedVideoAd::PlayCustomRewardedVideoAd(m_RewardAdID);
	PlayVideoProxy->OnComplete.AddDynamic(this,&UAdvertiseManager::OnRewardAdsSuccess);
	PlayVideoProxy->OnFail.AddDynamic(this,&UAdvertiseManager::OnRewardAdsPlayFail);
	PlayVideoProxy->OnClose.AddDynamic(this,&UAdvertiseManager::OnRewardAdsClose);
	PlayVideoProxy->Activate();
}

void UAdvertiseManager::RewardAdsLoadFail(const FString& reason)
{
	UDiabloGameInstance::Get->RequestPopupText(TEXT("광고 로딩 실패,그냥 받으세요"));
	
	m_bLoadRewardAdsProcessing=false;

	OnRewardAdsSuccess(FString(),0);
}

void UAdvertiseManager::OnRewardAdsSuccess(FString item, int32 amount)
{
	m_bLoadRewardAdsProcessing=false;
	
	int RandomGemStones = FMath::RandRange(30,150);
	UDiabloGameInstance::Get->RequestPopupText(FString::Printf(TEXT("광고 시청성공,젬스톤%d개 획득"),RandomGemStones));
	
	UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(RandomGemStones);
	//
	UDiabloGameInstance::Get->m_PlayfabManager->UploadAdmobTime(FDateTime::UtcNow());
	
	m_fRemainAdmobTime=ADSCOOL;
	
	m_bAdsUpdateLock=false;
}

void UAdvertiseManager::OnRewardAdsClose()
{
	UDiabloGameInstance::Get->RequestPopupText(TEXT("광고 창 닫힘"));
	m_bLoadRewardAdsProcessing=false;
}

void UAdvertiseManager::OnRewardAdsPlayFail(const FString& reason)
{
	UDiabloGameInstance::Get->RequestPopupText(TEXT("광고 재생 실패"));
	m_bLoadRewardAdsProcessing=false;
}

