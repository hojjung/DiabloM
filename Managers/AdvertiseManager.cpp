#include "AdvertiseManager.h"

#include "DiabloGameInstance.h"
#include "EasyAdsLibrary.h"
#include "LoadCustomInterstitialAd.h"
#include "LoadCustomRewardedVideoAd.h"
#include "PlayCustomRewardedVideoAd.h"
#include "PlayVideoProxy.h"
#include "ShowCustomInterstitialAd.h"
#include "ShowInterstitialProxy.h"

void UAdvertiseManager::Init()
{
	m_RewardAdID = TEXT("ca-app-pub-3940256099942544/5224354917");
	m_InitAdID = TEXT("ca-app-pub-3940256099942544/1033173712");//
	//
	m_bLoadInitAdsProcessing=false;
	
	m_bLoadRewardAdsProcessing=false;
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
	if(m_bLoadRewardAdsProcessing)
	{
		return;
	}
	m_bLoadRewardAdsProcessing=true;
	
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
	UDiabloGameInstance::Get->RequestPopupText(TEXT("광고 로딩 실패"));
	m_bLoadRewardAdsProcessing=false;
}

void UAdvertiseManager::OnRewardAdsSuccess(FString item, int32 amount)
{
	int RandomGemStones = FMath::RandRange(10,50);
	UDiabloGameInstance::Get->RequestPopupText(FString::Printf(TEXT("광고 시청성공,젬스톤%d개 획득"),RandomGemStones));
	PRINTF("RewardAds Success:%s,Amount:%d",*item,amount);
	m_bLoadRewardAdsProcessing=false;
	UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(RandomGemStones);
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

void UAdvertiseManager::ShowInterstitialAds()
{
	if(m_bLoadInitAdsProcessing)
    {
    	return;
    }
    m_bLoadInitAdsProcessing=true;
	
	m_LoadedInitAds=ULoadCustomInterstitialAd::LoadCustomInterstitialAd(m_InitAdID);
	m_LoadedInitAds->OnLoadSuccess.AddDynamic(this,&UAdvertiseManager::InitAdsLoadSuccess);
	m_LoadedInitAds->OnLoadFail.AddDynamic(this,&UAdvertiseManager::InitAdsLoadFail);
	m_LoadedInitAds->Activate();
}

void UAdvertiseManager::InitAdsLoadSuccess()
{
	UShowCustomInterstitialAd* PlayInterProxy = UShowCustomInterstitialAd::ShowCustomInterstitialAd(m_InitAdID);
	PlayInterProxy->OnShow.AddDynamic(this,&UAdvertiseManager::OnInterShow);
	PlayInterProxy->OnClick.AddDynamic(this,&UAdvertiseManager::OnInterClick);
	PlayInterProxy->OnClose.AddDynamic(this,&UAdvertiseManager::OnInterClose);
	PlayInterProxy->Activate();
}

void UAdvertiseManager::InitAdsLoadFail(const FString& reason)
{
	PRINTF("InitAdLoadFail:%s",*reason);
	m_bLoadInitAdsProcessing=false;
}

void UAdvertiseManager::OnInterClick()
{
	m_bLoadInitAdsProcessing=false;
}

void UAdvertiseManager::OnInterShow()
{
	m_bLoadInitAdsProcessing=false;
}

void UAdvertiseManager::OnInterClose()
{
	m_bLoadInitAdsProcessing=false;
}


