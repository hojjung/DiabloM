#pragma once
#include "DiabloM.h"
#include "OnlineError.h"
#include "OnlineStoreInterface.h"
#include "PlayFab.h"
#include "Core/PlayFabError.h"
#include "Core/PlayFabClientDataModels.h"
#include "Core/PlayFabClientAPI.h"
#include "PlayfabManager.generated.h"

typedef PlayFab::UPlayFabClientAPI::FUpdateUserDataDelegate FUpdateDele;
typedef PlayFab::ClientModels::FUpdateUserDataRequest FUpdateReq;
typedef PlayFab::ClientModels::FUpdateUserDataResult FUpdateRslt;
//
typedef PlayFab::UPlayFabClientAPI::FExecuteCloudScriptDelegate FExeCScriptDele;
typedef PlayFab::ClientModels::FExecuteCloudScriptRequest FExeCScriptReq;
typedef PlayFab::ClientModels::FExecuteCloudScriptResult FExeCScriptRslt;
//
typedef PlayFab::UPlayFabClientAPI::FGetUserDataDelegate FGetUsrDataDele;
typedef PlayFab::ClientModels::FGetUserDataRequest FGetUsrDataReq;
typedef PlayFab::ClientModels::FGetUserDataResult FGetUsrDataRslt;
//
typedef PlayFab::FPlayFabErrorDelegate FFailDele;
typedef PlayFab::FPlayFabCppError FFailRslt;
//
typedef PlayFab::UPlayFabClientAPI::FGetAccountInfoDelegate FGetAccntInfoDele;
typedef PlayFab::ClientModels::FGetAccountInfoRequest FGetAccntInfoReq;
typedef PlayFab::ClientModels::FGetAccountInfoResult FGetAccntInfoRslt;
//

UCLASS()
class DIABLOM_API UPlayfabManager : public UObject
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnShowAdBanner,bool);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnVirtualCurrencyChanged,int);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayfabError,FString&);

	FOnShowAdBanner m_OnShowAdBanner;
	//해금된 스테이지와 해금된 클래스,각클래스 업그레이드 레벨 다저장해야함
	//다 숫자로 통일해주자? 테이블에서 어케 가져와
	static const FString Gold;
	static const FString Dg;
	static const FString StatSkill;
	static const FString Quest;
	static const FString SkinClass;
	static const FString Weapon;
	static const FString Wing;
	static const FString Pet;
	static const FString Accessory;
	static const FString IAP;
	//
	FOnVirtualCurrencyChanged m_OnGemstoneChanged;
	
	FOnPlayfabError m_OnPlayfabError;

	FString m_OrderID;

	FDateTime m_LastLoginTime;

	FDateTime m_CurrentTime;

public:
	UPlayfabManager();

	~UPlayfabManager();

	void Init();

protected:
	TMap<FString,PlayFab::ClientModels::FCatalogItem> m_MapCatalogItems;
	

protected:
	void HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex,
	                           const FOnlineError& error);

	void TryLoginPlayfabGoogle(TSharedPtr<const FUniqueNetId> uniqueId);
	


	void OnSuccessPlayfabLogin(const PlayFab::ClientModels::FLoginResult& Result);

	void OnSuccessGetUserData(const FGetUsrDataRslt& result);

	void RequestGetAccountInfo();

	void OnSuccessGetAccountInfo(const FGetAccntInfoRslt& rslt);
	
	
public:
	void OnErrorPlayfabReq(const FFailRslt& ErrorResult);

public:
	UPROPERTY()
	bool m_bVersionChecked = false;
	UPROPERTY()
	bool m_bIsNicknameSet = false;
	UPROPERTY()
	bool m_bShowNicknameSet = false;
	UPROPERTY()
	bool m_bLoginProcessStarted = false;
	UPROPERTY()
	bool m_bIsLoginCompleted = false;
	UPROPERTY()
	float m_fDeltaCounter;
	UPROPERTY()
	FString m_LoadedGold;
	UPROPERTY()
	FString m_LoadedDg;
	UPROPERTY()
	FString m_LoadedStatSkill;
	UPROPERTY()
	FString m_LoadedQuest;
	UPROPERTY()
	FString m_LoadedClass;
	UPROPERTY()
	FString m_LoadedWeapon;
	UPROPERTY()
	FString m_LoadedWing;
	UPROPERTY()
	FString m_LoadedPet;
	UPROPERTY()
	FString m_LoadedAccessory;
	UPROPERTY()
	FString m_LoadedNickname;
	UPROPERTY()
	FString m_CurrentVersionName="TEST0321";
	UPROPERTY()
	bool m_bIsShowAD;
	//
	UPROPERTY()
	int m_nRanking;

public:
	UPROPERTY()
	FString m_PlayfabID;

	PlayFabClientPtr GetClientAPI = nullptr;
public:

	bool GetIsLogined()
	{
		return m_bIsLoginCompleted;
	}

	void ShowBannerAd(bool able);

	void TickTryUpdateUserData(float deltaTime);//should split
	//the ui drity should update
	//gold kill count separete need;
	void RequestGetInventory();

	void RequestSetNickname(FString str);

	void RequestGetUserData();

	void SetOnlineStatus();
	
	void SetOfflineStatus();


protected:
	void OnNickNameSetSuccess(const  PlayFab::ClientModels::FUpdateUserTitleDisplayNameResult&);

	void OnCloudScriptSuccess(const FExeCScriptRslt& rslt);

	void OnVersionCheckCloudScriptSuccess(const FExeCScriptRslt& rslt);

public:
	UFUNCTION()
    void BuyIAP(FString itemId,bool bIsConsumable);

	UFUNCTION()
	void PurchaseVirtualItem(FString itemUniqueId);

	UFUNCTION()
	void PurchaseSuccess(EInAppPurchaseState::Type completionStatus, const FInAppPurchaseProductInfo& inAppPurchaseInformation);

	UFUNCTION()
    void PurchaseFail(EInAppPurchaseState::Type completionStatus, const FInAppPurchaseProductInfo& inAppPurchaseInformation);

	void OnStageComplete();

	void RequestVersionCheck();

	void RequestGetServerTime();
	
protected:
	void OnIAPGoogleValidateSuccess( const PlayFab::ClientModels::FValidateGooglePlayPurchaseResult&);


	void OnSuccessGetInven( const PlayFab::ClientModels::FGetUserInventoryResult&);

	void OnSuccessTimeGet(const PlayFab::ClientModels::FGetTimeResult& );
};


