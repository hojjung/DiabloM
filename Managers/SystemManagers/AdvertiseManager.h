// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "UObject/NoExportTypes.h"
#include "AdvertiseManager.generated.h"

class ULoadCustomInterstitialAd;
class ULoadCustomRewardedVideoAd;
/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAdTick,const FString&);
UCLASS()
class DIABLOM_API UAdvertiseManager : public UObject
{
	GENERATED_BODY()
public:
	FOnAdTick m_OnAdTick;
	
protected:
	UPROPERTY()
	ULoadCustomRewardedVideoAd* m_LoadedRewardAds;

	bool m_bLoadRewardAdsProcessing;

	FString m_RewardAdID;

	float m_fRemainAdmobTime;

	bool m_bAdsUpdateLock;

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnShowAdBanner,bool);
	FOnShowAdBanner m_OnShowAdBanner;
	
protected:
	FString GetTimeToStr();
public:
	void Init();

	void SetTimeCooldownFromServer(FDateTime lastAdmobTime,FDateTime currentLoginTime);

	void Tick(float deltaTime);
	
	UFUNCTION()
    void RewardAdsLoadFail(const FString& reason);
	UFUNCTION()
    void RewardAdsLoadSuccess();
	UFUNCTION()
	void OnRewardAdsPlayFail(const FString& reason);

	void ShowBannerAD(bool b);

	void ShowRewardAds();

public:
	UFUNCTION()
	void OnRewardAdsSuccess(FString item, int32 amount);
	UFUNCTION()
	void OnRewardAdsClose();

	
};


