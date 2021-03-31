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
public://delegate
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnVirtualCurrencyChanged,int);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayfabError,FString&);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRankReceived,const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>&);
	
	FOnRankReceived m_OnTotalRankReceived;

	FOnRankReceived m_OnPlayerRankReceived;
	
	

	FOnVirtualCurrencyChanged m_OnGemstoneChanged;
	
	FOnPlayfabError m_OnPlayfabError;

public://static
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
public://user data
	UPROPERTY()
	FString m_PlayfabID;

	PlayFabClientPtr GetClientAPI = nullptr;

	FString m_OrderID;

	FDateTime m_LastLoginTime;

	FDateTime m_CurrentTime;

public://loaded data
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
	float m_fDeltaCountMinutePlaytime;
	UPROPERTY()
	float m_fDeltaCountTitleData;
	UPROPERTY()
	float m_fDeltaCountRanking;
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
	FString m_LoadedIAP;
	UPROPERTY()
	FString m_LoadedNickname;
	UPROPERTY()
	FString m_CurrentVersionName="TEST0321";
	
protected://rank
	UPROPERTY()
	int m_nRanking;

	int m_nSafeRanking;

	TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry> m_TotalRanking;

	TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry> m_PlayerRanking;

public://init
	UPlayfabManager();

	~UPlayfabManager();

	void Init();
	

protected:
	TMap<FString,PlayFab::ClientModels::FCatalogItem> m_MapCatalogItems;
	

protected:
	void HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex,
	                           const FOnlineError& error);

	void TryLoginPlayfabGoogle(TSharedPtr<const FUniqueNetId> uniqueId);
	
	void RequestGetAccountInfo();
	
	void OnErrorPlayfabReq(const FFailRslt& ErrorResult);

public:

	bool GetIsLogined()
	{
		return m_bIsLoginCompleted;
	}

	void TickTryUpdateUserData(float deltaTime);//should split
	//the ui drity should update
	//gold kill count separete need;
	void RequestGetInventory();

	void RequestSetNickname(FString str);

	void RequestGetUserData();

	void SetOnlineStatus();
	
	void SetOfflineStatus();

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
	void OnSuccessPlayfabLogin(const PlayFab::ClientModels::FLoginResult& Result);

	void OnSuccessGetUserData(const FGetUsrDataRslt& result);

	void OnSuccessGetAccountInfo(const FGetAccntInfoRslt& rslt);
	
	void OnIAPGoogleValidateSuccess( const PlayFab::ClientModels::FValidateGooglePlayPurchaseResult&);

	void OnSuccessGetInven( const PlayFab::ClientModels::FGetUserInventoryResult&);

	void OnSuccessTimeGet(const PlayFab::ClientModels::FGetTimeResult& );

	void OnSuccessGetTotalRanking( const PlayFab::ClientModels::FGetLeaderboardResult&);

	void OnNickNameSetSuccess(const  PlayFab::ClientModels::FUpdateUserTitleDisplayNameResult&);

	void OnCloudScriptSuccess(const FExeCScriptRslt& rslt);

	void OnVersionCheckCloudScriptSuccess(const FExeCScriptRslt& rslt);

	void OnSuccessGetPlayerAroundRanking(const PlayFab::ClientModels::FGetLeaderboardAroundPlayerResult&);

public:
	void RequestRetrieveTotalRanking();

	void RequestRetrievePlayerAroundRanking();
	
	int GetSafeRanking();

	int GetRanking();

	void SetRanking(int rank);
	void UploadIAPData();

	void UploadUserTitleData();

	void RequestCheatAlert();

	FORCEINLINE const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& GetTotalRank() const
	{
		return m_TotalRanking;
	}

	FORCEINLINE const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& GetPlayerRank() const
	{
		return m_PlayerRanking;
	}

	void PurchaseWithGemStone(int amount,FString itemName);

	void OnPurchaseWithGemStoneSuccess(const PlayFab::ClientModels::FPurchaseItemResult&);

	void AddGemStone(int amount);

	void OnAddGemStone(const PlayFab::ClientModels::FModifyUserVirtualCurrencyResult&);

	void UploadQuestData(const FString& data);

	void UploadUpgradeData(const FString& data);

	void UploadEquipData(const FString& weaponData,const FString& skinData,const FString& petData,const FString& accessoryData,const FString& wingData);

	void UploadIAPData(const FString& data);


	void UploadGold(BigInt gold);

	void UploadDungeonData(int currentDungeon,int maxDungeon);
};

