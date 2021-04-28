// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "PlayFabClientDataModels.h"
#include "Characters/OtherPlayerPawn.h"
#include "Datas/ItemTierData.h"
#include "UObject/NoExportTypes.h"
#include "PVPManager.generated.h"

class UPlayFabJsonObject;
/**
 * 
 */
UCLASS()
class DIABLOM_API UPVPManager : public UObject
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnPVPMatched,UPlayFabJsonObject*,UPlayFabJsonObject*,UPlayFabJsonObject*);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnOtherPlayerFound,const FString);
	DECLARE_MULTICAST_DELEGATE(FOnMatchFail);
	DECLARE_MULTICAST_DELEGATE(FOnMatchStart);
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnDmgChanged,float,BigInt,BigInt);

	UPVPManager();

	FOnDmgChanged m_OnDmgChanged;

	FOnPVPMatched m_OnMatchSuccessed;

	FOnOtherPlayerFound m_OnOtherPlayerFound;

	FOnMatchFail m_OnMatchFail;

	FOnMatchStart m_OnMatchStart;

	FTimerHandle m_TimerHandle_OnTimer;

	TWeakObjectPtr<AOtherPlayerPawn> m_PVPOtherPlayer;

	bool m_bIsMatchStarted;

	BigInt m_PlayerTotalDmg;
	
	BigInt m_OtherPlayerTotalDmg;

	BigInt m_TotalDmg;
	
public:
	FString m_OtherPlayerDisplayName;
	UPROPERTY()
	UPlayFabJsonObject* m_StatObj;
	UPROPERTY()
	UPlayFabJsonObject* m_SkillObj;
	UPROPERTY()
	UPlayFabJsonObject* m_EquipObj;

protected:
	void UpdateGauge();
	
public:
	UFUNCTION()
	void MoveStageLevelToPVP();
	
	void RequestPVPMatching();

	void OnRequestComplete(const PlayFab::ClientModels::FGetLeaderboardAroundPlayerResult& rslt);

	void OnGetOtherPlayerSuccess(const PlayFab::ClientModels::FGetUserDataResult&);
	
	void MatchFail();

	void PVPStart();

	void PVPEnd();

	void AddPlayerTotalDamage(const BigInt& v);

	void AddOtherPlayerTotalDamage(const BigInt& v);
};

