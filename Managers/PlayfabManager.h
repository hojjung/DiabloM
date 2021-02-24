// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "OnlineError.h"

#include "PlayFab.h"
#include "Core/PlayFabError.h"
#include "Core/PlayFabClientDataModels.h"
#include "Core/PlayFabClientAPI.h"
#include "PlayFabAdminDataModels.h"
#include "PlayFabClientDataModels.h"
#include "PlayFabServerDataModels.h"
#include "PlayFabUtilities.h"
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
typedef PlayFab::ClientModels::FGetUserDataResult FGetUsrDataRSlt;
//
typedef PlayFab::FPlayFabErrorDelegate FFailDele;
typedef PlayFab::FPlayFabCppError FFailRslt;

class UPlayerUpgradeManager;

UCLASS()
class DIABLOM_API UPlayfabManager : public UObject
{
	GENERATED_BODY()
public:
	static const FString PlayfabDungeonIDKey;//현재 스테이지
	static const FString PlayfabPlayerClassIDKey;//현재 클래스 스킨
	//해금된 스테이지와 해금된 클래스,각클래스 업그레이드 레벨 다저장해야함
	//다 숫자로 통일해주자? 테이블에서 어케 가져와
	static const FString PlayerAtkDmg01Key;
	static const FString PlayerAtkCri01Key;
	static const FString PlayerAtkCDmg01Key;
	//
	static const FString PlayerGoldKey;
	
public:
	UPlayfabManager();
	
	~UPlayfabManager();
	
	void LoadLocalDefaultData();

	void Init();
	
	int GetPlAtkDmg01Lv();
	
	int GetPlAtkCri01Lv();
	
	int GetPlAtkCDmg01Lv();

	bool m_bLoginProcessEnd = false;
	
protected:
	void HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex, const FOnlineError& error);
	
	void TryLoginPlayfabGoogle(TSharedPtr<const FUniqueNetId> uniqueId);
	
	void OnSuccessPlayfabLogin(const PlayFab::ClientModels::FLoginResult& Result);
	

	void OnSuccessGetUserData(const FGetUsrDataRSlt& result) ;
public:
	void OnErrorPlayfabReq(const FFailRslt& ErrorResult);

public:
	
	UPROPERTY()
	bool m_bIsLogined = false;	
	
	UPROPERTY()
	FString m_LoadedDgID;
	UPROPERTY()
	FString m_LoadedPlayerClassID;
	UPROPERTY()
	int m_nLoadedPlAtkDmg01;
	UPROPERTY()
	int m_nLoadedPlAtkCri01;
	UPROPERTY()
	int m_nLoadedPlAtkCDmg01;
public:
	UPROPERTY()
	FString m_PlayfabID;

	PlayFabClientPtr GetClientAPI = nullptr;
public:
	
	bool GetIsLogined()
	{
		return m_bIsLogined;
	}

	void ShowBannerAd(bool able);

	FORCEINLINE FString GetLoadedDungeonID() const
	{
		return  m_LoadedDgID;
	}

	FORCEINLINE FString GetLoadedPlayerClassID() const
	{
		return  m_LoadedPlayerClassID;
	}
};
