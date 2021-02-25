#pragma once
#include "DiabloM.h"
#include "OnlineError.h"
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
	static const FString Gold;
	static const FString Dg;
	static const FString Stat;
	static const FString Skill;
	static const FString Class;
	static const FString Weapon;
	static const FString Wing;
	static const FString Pet;
	static const FString Accessory;
	//
public:
	UPlayfabManager();

	~UPlayfabManager();

	void Init();

	bool m_bLoginProcessEnd = false;


protected:
	void HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex,
	                           const FOnlineError& error);

	void TryLoginPlayfabGoogle(TSharedPtr<const FUniqueNetId> uniqueId);

	void OnSuccessPlayfabLogin(const PlayFab::ClientModels::FLoginResult& Result);


	void OnSuccessGetUserData(const FGetUsrDataRSlt& result);
public:
	void OnErrorPlayfabReq(const FFailRslt& ErrorResult);

public:
	UPROPERTY()
	bool m_bIsLogined = false;
	UPROPERTY()
	float m_fDeltaCounter;
	UPROPERTY()
	FString m_LoadedGold;
	UPROPERTY()
	FString m_LoadedDg;
	UPROPERTY()
	FString m_LoadedStat;
	UPROPERTY()
	FString m_LoadedSkill;
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
	//

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

	void TickTryUpdateUserData(float deltaTime);//should split
	//the ui drity should update
	//gold kill count separete need;
};
