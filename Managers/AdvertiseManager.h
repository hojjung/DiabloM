// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "UObject/NoExportTypes.h"
#include "AdvertiseManager.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UAdvertiseManager : public UObject
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnShowAdBanner,bool);
	FOnShowAdBanner m_OnShowAdBanner;
	
	void Init();

public:
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


