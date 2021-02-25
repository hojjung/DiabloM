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

UCLASS()
class DIABLOM_API UPlayfabManager : public UObject
{
	GENERATED_BODY()
public:
	//해금된 스테이지와 해금된 클래스,각클래스 업그레이드 레벨 다저장해야함
	//다 숫자로 통일해주자? 테이블에서 어케 가져와
	static const FString PlayerAtkDmg01Key;
	static const FString PlayerAtkCri01Key;
	static const FString PlayerAtkCDmg01Key;
	//
	static const FString PlayerSkill01Key;
	static const FString PlayerSkill02Key;
	static const FString PlayerSkill03Key;
	//
	static const FString PlayerClassKey;
	static const FString PlayerWeaponKey;
	static const FString PlayerWingKey;
	static const FString PlayerPetKey;
	static const FString PlayerAccessoryKey;
	//
	static const FString DungeonUnlockKey;
	//
	static const FString PlayerGoldKey;
	
public:
	UPlayfabManager();
	
	~UPlayfabManager();
	
	void Init();
	
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
	FString m_LoadedDgUnlockedID;
	UPROPERTY()
	int m_nLoadedPlAtkDmg01;
	UPROPERTY()
	int m_nLoadedPlAtkCri01;
	UPROPERTY()
	int m_nLoadedPlAtkCDmg01;
	//
	UPROPERTY()
	int m_nLoadedPlSkill01;
	UPROPERTY()
	int m_nLoadedPlSkill02;
	UPROPERTY()
	int m_nLoadedPlSkill03;
	
	UPROPERTY()
	float m_fDeltaCounter;
	UPROPERTY()
	FString m_LoadedPlayerClass;
	UPROPERTY()
	FString m_LoadedWing;
	UPROPERTY()
	FString m_LoadedWeapon;
	UPROPERTY()
	FString m_LoadedAccessory;
	UPROPERTY()
	FString m_LoadedPet;
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
		return  m_LoadedDgUnlockedID;
	}

	void TickTryUpdateUserData(float deltaTime);
};

