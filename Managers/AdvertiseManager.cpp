
#include "AdvertiseManager.h"

#include "EasyAdsLibrary.h"
#include "LoadCustomInterstitialAd.h"
#include "PlayVideoProxy.h"
#include "ShowInterstitialProxy.h"

void UAdvertiseManager::Init()
{
	
}

void UAdvertiseManager::ShowBannerAD(bool b)
{
	if(b)
	{
		UEasyAdsLibrary::ShowBanner(false);
		
	}
	else
	{
		UEasyAdsLibrary::HideBanner();
	}
	m_OnShowAdBanner.Broadcast(b);
}

void UAdvertiseManager::ShowRewardAds()
{
	UPlayVideoProxy* PlayVideoProxy = UPlayVideoProxy::PlayRewardedVideo();

	PlayVideoProxy->OnSuccess.AddDynamic(this,&UAdvertiseManager::OnRewardAdsSuccess);
	PlayVideoProxy->OnClosed.AddDynamic(this,&UAdvertiseManager::OnRewardAdsClose);
	PlayVideoProxy->Activate();
}

void UAdvertiseManager::ShowInterstitialAds()
{
	UShowInterstitialProxy* PlayInterProxy =UShowInterstitialProxy::ShowInterstitial();
	PlayInterProxy->OnShow.AddDynamic(this,&UAdvertiseManager::OnInterShow);
	PlayInterProxy->OnClick.AddDynamic(this,&UAdvertiseManager::OnInterClick);
	PlayInterProxy->OnClose.AddDynamic(this,&UAdvertiseManager::OnInterClose);
	PlayInterProxy->Activate();
}

void UAdvertiseManager::OnInterClick()
{
}

void UAdvertiseManager::OnInterShow()
{
}

void UAdvertiseManager::OnInterClose()
{
}

void UAdvertiseManager::OnRewardAdsSuccess(FString item, int32 amount)
{


}
 void UAdvertiseManager::OnRewardAdsClose()
{
}