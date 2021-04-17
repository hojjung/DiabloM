#pragma once
#include "DiabloM.h"
#include "InboxManager.h"
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
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayfabError,const FString&);
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
	static const FString Daily;
	static const FString Inbox;
	static const FString Gacha;
	//
public://user data
	UPROPERTY()
	FString m_PlayfabID;

	PlayFabClientPtr GetClientAPI = nullptr;

	FString m_OrderID;

	FDateTime m_LastLoginTime;

	FDateTime m_LastLogoutTime;

	FDateTime m_CurrentTime;

	bool m_bIsNewCreatePlayer;

	FString m_ServerCloseOpenTime;

	FString m_ServerVersion;

	bool m_bIsCustomID;
	
public://loaded data
	UPROPERTY()
	bool m_bIsNicknameSet = false;
	UPROPERTY()
	bool m_bShowNicknameSet = false;
	UPROPERTY()
	bool m_bLoginProcessStarted = false;
	UPROPERTY()
	bool m_bIsLoginCompleted = false;
	UPROPERTY()
	bool m_bIsServerClosed = false;
	UPROPERTY()
	bool m_bIsVersionWrong = false;
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
	FString m_CurrentVersionName;//RELEASE0408

	TArray<FString> m_AryIAPData;

	int m_nLocalGemStone;

	TArray<PlayFab::ClientModels::FTitleNewsItem> m_TitleNews;
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
	
	void RequestUploadNewPlayerData();

protected:
	TMap<FString,PlayFab::ClientModels::FCatalogItem> m_MapCatalogItems;
	

protected:
	void HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex,
	                           const FOnlineError& error);

	void TryLoginPlayfabGoogle(TSharedPtr<const FUniqueNetId> uniqueId);
	
	void RequestGetAccountInfo();
	
	void OnErrorPlayfabReq(const FFailRslt& ErrorResult);

	FDateTime DecodePlayfabTimeToUe4Time(FString playfabTime);

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

	void RequestGetUserData01();

	void RequestGetUserData02();

	void RequestInboxList();

	void RequestClaimInbox(int index);
	
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

	void RequestServerOpenCheck();

	void RequestGetServerTime();

	void RequestTitleNews();
	
	void UpdateInboxListToClient(FString InboxListStr);

protected:
	
	void OnSuccessPlayfabLogin(const PlayFab::ClientModels::FLoginResult& Result);

	void OnSuccessGetUserData01(const FGetUsrDataRslt& result);

	void OnSuccessGetUserData02(const FGetUsrDataRslt& result);

	void OnSuccessGetAccountInfo(const FGetAccntInfoRslt& rslt);
	
	void OnIAPGoogleValidateSuccess( const PlayFab::ClientModels::FValidateGooglePlayPurchaseResult&);

	void OnSuccessGetInven( const PlayFab::ClientModels::FGetUserInventoryResult&);

	void OnSuccessTimeGet(const PlayFab::ClientModels::FGetTimeResult& );

	void OnSuccessGetTotalRanking( const PlayFab::ClientModels::FGetLeaderboardResult&);

	void OnNickNameSetSuccess(const  PlayFab::ClientModels::FUpdateUserTitleDisplayNameResult&);

	void OnStageCompleteScriptSuccess(const FExeCScriptRslt& rslt);

	void OnVersionCheckCloudScriptSuccess(const FExeCScriptRslt& rslt);

	void OnServerCloseCheckScriptSuccess(const FExeCScriptRslt& rslt);

	void OnInboxRefreshSuccess(const FExeCScriptRslt& rslt);

	void OnSuccessGetPlayerAroundRanking(const PlayFab::ClientModels::FGetLeaderboardAroundPlayerResult&);

	void OnSuccessGetTitleNews(const PlayFab::ClientModels::FGetTitleNewsResult&);

public:
	void RequestRetrieveTotalRanking();

	void RequestRetrievePlayerAroundRanking();
	
	int GetSafeRanking();

	int GetRanking();

	void SetRanking(int rank);
	
	FString GetIAPDataStr();

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

	void UploadEquipData(const FString& weaponData,const FString& skinData,const FString& petData,const FString& accessoryData,const FString& wingData);

	void UploadIAPData();

	void UploadGold(BigInt gold);

	void UploadDungeonData(int currentDungeon,int maxDungeon);

	void OnBossBattleStart();
	//
	void RequestItemTest();

	void UploadDailyData(int dday,const FDateTime claimTime);

	void UploadQuestData(const FString& data);
};


