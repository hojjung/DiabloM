#pragma once
#include "DiabloM.h"
#include "InboxManager.h"
#include "OnlineError.h"
#include "OnlineStoreInterface.h"
#include "PlayFab.h"
#include "Core/PlayFabError.h"
#include "Core/PlayFabClientDataModels.h"
#include "Core/PlayFabClientAPI.h"
#include "Managers/GoldManager.h"

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
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayfabError,const FString&);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRankReceived,const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>&);
	
	FOnRankReceived m_OnTotalStageRankReceived;

	FOnRankReceived m_OnPlayerStageRankReceived;

	FOnRankReceived m_OnTotalPVPRankReceived;

	FOnRankReceived m_OnPlayerPVPRankReceived;

	FOnPlayfabError m_OnPlayfabError;

	FString m_SessionTicket;

	TSharedPtr<UPlayFabAuthenticationContext> m_Auth;

	TSharedPtr<UPlayFabAuthenticationContext> CreateAuthCon(const FString* newSessonTicket = nullptr);
public://static
	static const FString MainData;
	static const FString MainDungeon;
	static const FString Quest;
	static const FString Daily;
	static const FString Gold;
	static const FString AdmobTime;
	static const FString PVPStatus;
	//

public:
	UPlayfabManager();
	//
public://user data
	UPROPERTY()
	FString m_PlayfabID;

	PlayFabClientPtr GetClientAPI = nullptr;

	FString m_OrderID;

	FDateTime m_LastLoginTime;

	FDateTime m_LastLogoutTime;

	FDateTime m_CurrentTime;

	FDateTime m_LastAdmobTime;

	bool m_bIsNewCreatePlayer;

	FString m_ServerCloseOpenTime;

	FString m_ServerVersion;

	bool m_bIsCustomID;
	
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
	float m_fDeltaInboxUpdateCooldown;
	UPROPERTY()
	float m_fDeltaCountMinutePlaytime;
	UPROPERTY()
	float m_fDeltaCountTitleData;
	UPROPERTY()
	float m_fDeltaCountRanking;
	UPROPERTY()
	FString m_LoadedNickname;
	UPROPERTY()
	FString m_CurrentVersionName;//RELEASE0408

	TArray<FString> m_AryIAPData;

	TArray<PlayFab::ClientModels::FTitleNewsItem> m_TitleNews;

public:
	void StartPlayfabLogin();
	
protected://rank
	UPROPERTY()
	FSafeInt m_nRanking;

	UPROPERTY()
	FSafeInt m_nPVPRanking;

	TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry> m_TotalStageRanking;

	TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry> m_PlayerStageRanking;

	TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry> m_TotalPVPRanking;

	TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry> m_PlayerPVPRanking;

	TMap<FString,PlayFab::ClientModels::FCatalogItem> m_MapCatalogItems;

protected:
	void RequestUploadNewPlayerData();

	void HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex,
	                           const FOnlineError& error);

	void TryLoginPlayfabGoogle(TSharedPtr<const FUniqueNetId> uniqueId);
	
	void RequestGetAccountInfo();
	
	void OnErrorPlayfabReq(const FFailRslt& ErrorResult);

	void OnSessionLoginErrorPlayfabReq(const FFailRslt& ErrorResult);

	FDateTime DecodePlayfabTimeToUe4Time(FString playfabTime);

	FString GetMainDataJsonStr();

	void OnSuccessPlayfabLogin(const PlayFab::ClientModels::FLoginResult& Result);

	void OnSuccessGetMainData(const FGetUsrDataRslt& result);
	
	void RequestCatalogItems();

	void OnSuccessGetUserData02(const FGetUsrDataRslt& result);

	void OnSuccessGetAccountInfo(const FGetAccntInfoRslt& rslt);
	
	void OnIAPGoogleValidateSuccess( const PlayFab::ClientModels::FValidateGooglePlayPurchaseResult&);

	void OnSuccessGetInven( const PlayFab::ClientModels::FGetUserInventoryResult&);

	void OnSuccessTimeGet(const PlayFab::ClientModels::FGetTimeResult& );

	void OnSuccessGetTotalRanking( const PlayFab::ClientModels::FGetLeaderboardResult&);

	void OnSuccessGetTotalPVPRanking( const PlayFab::ClientModels::FGetLeaderboardResult&);

	void OnNickNameSetSuccess(const  PlayFab::ClientModels::FUpdateUserTitleDisplayNameResult&);

	void OnStageCompleteScriptSuccess(const FExeCScriptRslt& rslt);

	void OnVersionCheckCloudScriptSuccess(const FExeCScriptRslt& rslt);

	void OnServerCloseCheckScriptSuccess(const FExeCScriptRslt& rslt);

	void OnNewPlayerDataInitSuccess(const FExeCScriptRslt& rslt);

	void OnInboxRefreshSuccess(const FExeCScriptRslt& rslt);

	void OnSuccessGetPlayerAroundRanking(const PlayFab::ClientModels::FGetLeaderboardAroundPlayerResult&);

	void OnSuccessGetPVPPlayerAroundRanking(const PlayFab::ClientModels::FGetLeaderboardAroundPlayerResult&);

	void OnSuccessGetTitleNews(const PlayFab::ClientModels::FGetTitleNewsResult&);
	//
	void OnPurchaseWithGemStoneSuccess(const PlayFab::ClientModels::FPurchaseItemResult&);

	void OnPurchaseWithPetTicketSuccess(const PlayFab::ClientModels::FPurchaseItemResult&);

	void OnPurchaseWithWingTicketSuccess(const PlayFab::ClientModels::FPurchaseItemResult&);

	//
	void OnAddGemStone(const PlayFab::ClientModels::FModifyUserVirtualCurrencyResult&);

	void OnAddWeaponStone(const PlayFab::ClientModels::FModifyUserVirtualCurrencyResult&);
	
	void OnAddSkillStone(const PlayFab::ClientModels::FModifyUserVirtualCurrencyResult&);

	void OnAddPetTicket(const PlayFab::ClientModels::FModifyUserVirtualCurrencyResult&);

	void OnAddWingTicket(const PlayFab::ClientModels::FModifyUserVirtualCurrencyResult&);

	void OnAddDgKeys(const PlayFab::ClientModels::FModifyUserVirtualCurrencyResult&);

	void SetMainDataToManagers(const FString& maindataFromServer);

	void OnPVPUploadSuccess(const FExeCScriptRslt& rslt);

	void OnPVPGetSuccess(const FExeCScriptRslt& rslt);

public:
	void Init();
	
	void OnBossBattleStart();
	
	bool GetIsLogined()
	{
		return m_bIsLoginCompleted;
	}

	void TickTryUpdateUserData(float deltaTime);//should split
	//the ui drity should update
	//gold kill count separete need;
	void RequestSetNickname(FString str);
	
	void RequestGetInventory();

	void RequestGetMainData();

	void RequestGetOtherPlayerMainData(const FString& playfabID,FGetUsrDataDele onSuccess);

	bool RequestInboxList();

	void RequestClaimInbox(int index);

	void RequestClaimAllInbox();
	
	UFUNCTION()
    void BuyIAP(FString itemId,bool bIsConsumable);

	UFUNCTION()
	void PurchaseVirtualItem(FString itemUniqueId);

	UFUNCTION()
	void PurchaseSuccess(EInAppPurchaseState::Type completionStatus, const FInAppPurchaseProductInfo& inAppPurchaseInformation);

	UFUNCTION()
    void PurchaseFail(EInAppPurchaseState::Type completionStatus, const FInAppPurchaseProductInfo& inAppPurchaseInformation);

	void RequestVersionCheck();

	void RequestServerOpenCheck();

	void RequestGetServerTime();

	void RequestTitleNews();
	
	void UpdateInboxListToClient(FString InboxListStr);

	void RequestRetrieveTotalRanking();

	void RequestRetrievePlayerAroundRanking();

	void RequestRetrieveTotalPVPRanking();

	void RequestRetrievePVPPlayerAroundRanking();
	
	int GetRanking();

	void RequestCheatAlert();

	FORCEINLINE const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& GetTotalStageRank() const
	{
		return m_TotalStageRanking;
	}

	FORCEINLINE const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& GetStagePlayerRank() const
	{
		return m_PlayerStageRanking;
	}

	FORCEINLINE const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& GetTotalPVPRank() const
	{
		return m_TotalPVPRanking;
	}

	FORCEINLINE const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& GetPVPPlayerRank() const
	{
		return m_PlayerPVPRanking;
	}

	void PurchaseWithGemStone(int amount,FString itemName);

	void PurchaseWithPetTicket(int amount);

	void PurchaseWithWingTicket(int amount,FString itemName);

	void SubtractWeaponStone(int amount);

	void SubtractSkillStone(int amount);

	void SubtractDgKey(int amount);
	//
	void AddGemStone(int amount);
	//
	void AddPetTicket(int amount);
	
	void AddWingTicket(int amount);

	void AddDgKey(int amount);

	void AddWeaponStones(int amount);

	void AddSkillStones(int amount);

	//
	void UploadNormalDungeon();
	
	void UploadMainData();

	void UploadDailyData(const FString dailyJsonStr);

	void UploadQuestData(const FString& data);

	void UploadGold(BigInt gold);

	void UploadAdmobTime(const FDateTime& date_time);

	void RequestPVPMatching(int aroundCount,PlayFab::UPlayFabClientAPI::FGetLeaderboardAroundPlayerDelegate completeDele);

	void OnPvPComplete();

	void RequestGetPVPData();

	friend UGoldManager;
};



