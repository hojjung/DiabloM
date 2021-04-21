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
UCLASS()
class DIABLOM_API UAdvertiseManager : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	ULoadCustomInterstitialAd* m_LoadedInitAds;
	UPROPERTY()
	ULoadCustomRewardedVideoAd* m_LoadedRewardAds;

	bool m_bLoadInitAdsProcessing;

	bool m_bLoadRewardAdsProcessing;

	FString m_RewardAdID;

	FString m_InitAdID;
	
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnShowAdBanner,bool);
	FOnShowAdBanner m_OnShowAdBanner;
	

public:
	void Init();
	
	UFUNCTION()
	void InitAdsLoadFail(const FString& reason);
	UFUNCTION()
	void InitAdsLoadSuccess();
	UFUNCTION()
    void RewardAdsLoadFail(const FString& reason);
	UFUNCTION()
    void RewardAdsLoadSuccess();
	UFUNCTION()
	void OnRewardAdsPlayFail(const FString& reason);

	void ShowBannerAD(bool b);

	void ShowRewardAds();

	void ShowInterstitialAds();

public:
	UFUNCTION()
	void OnInterClick();
	UFUNCTION()
    void OnInterShow();
	UFUNCTION()
    void OnInterClose();
	UFUNCTION()
	void OnRewardAdsSuccess(FString item, int32 amount);
	UFUNCTION()
	void OnRewardAdsClose();
};


