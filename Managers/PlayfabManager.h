// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "OnlineError.h"
#include "PlayFab.h"
#include "Core/PlayFabError.h"
#include "Core/PlayFabClientDataModels.h"
#include "PlayfabManager.generated.h"

UCLASS()
class DIABLOM_API UPlayfabManager : public UObject
{
	GENERATED_BODY()
public:
	UPlayfabManager();
	
	~UPlayfabManager();
	void LoadLocalDefaultData();

	void Init();

	bool m_bLoginProcessEnd = false;
	
protected:
	void HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex, const FOnlineError& error);
	
	void TryLoginPlayfabGoogle(TSharedPtr<const FUniqueNetId> uniqueId);
	
	void OnSuccessPlayfabLogin(const PlayFab::ClientModels::FLoginResult& Result);
	
	void OnLoginErrorPlayfabReq(const PlayFab::FPlayFabCppError& ErrorResult) ;

	void OnErrorPlayfabReq(const PlayFab::FPlayFabCppError& ErrorResult);

	void OnSuccessGetUserData(const PlayFab::ClientModels::FGetUserDataResult& result) ;
	
private:
	PlayFabClientPtr clientAPI = nullptr;

	bool m_bIsLogined = false;

	FString m_PlayfabID;
	
	UPROPERTY()
	FName m_LoadedDgID;
	UPROPERTY()
	FName m_LoadedPlayerClassID;
	UPROPERTY()
	FName m_LoadedPlayerUpgradeID;
	UPROPERTY()
	FName m_LoadedUpgradeID;

public:
	bool GetIsLogined()
	{
		return m_bIsLogined;
	}

	void ShowBannerAd(bool able);

	FORCEINLINE FName GetLoadedDungeonID() const
	{
		return  m_LoadedDgID;
	}

	FORCEINLINE FName GetLoadedPlayerClassID() const
	{
		return  m_LoadedPlayerClassID;
	}
};
