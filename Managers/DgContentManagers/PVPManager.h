// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "MonsterSpawnManager.h"
#include "PlayFabClientDataModels.h"
#include "Characters/Pawns/OtherPlayerPawn.h"
#include "Datas/ItemTierData.h"
#include "UObject/NoExportTypes.h"
#include "PVPManager.generated.h"


#define PVPTIME 30

class UPlayFabJsonObject;
/**
 * 
 */
UCLASS()
class DIABLOM_API UPVPManager : public UMonsterSpawnManager
{
	GENERATED_BODY()
public:
	UPVPManager();
	
	DECLARE_DELEGATE_ThreeParams(FOnPVPMatched,UPlayFabJsonObject*,UPlayFabJsonObject*,UPlayFabJsonObject*);
	DECLARE_DELEGATE_OneParam(FOnOtherPlayerSpawned,AOtherPlayerPawn*);
	DECLARE_DELEGATE_ThreeParams(FOnDmgChanged,float,BigInt,BigInt);
	DECLARE_DELEGATE_OneParam(FOnOtherPlayerFound,const FString);
	DECLARE_DELEGATE_OneParam(FOnBattleEnd,bool);
	DECLARE_DELEGATE(FOnMatchStart);
	DECLARE_DELEGATE(FOnMatchFail);
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnPVPStatusChanged,int,int,int);/*w,l,mmr*/
	
	FOnPVPStatusChanged m_OnPVPStatusChanged;
	FOnOtherPlayerSpawned m_OnOtherPlayerSpawned;
	FOnOtherPlayerFound m_OnOtherPlayerFound;
	FOnPVPMatched m_OnMatchSuccessed;
	FOnDmgChanged m_OnDmgChanged;
	FOnMatchStart m_OnMatchStart;
	FOnMatchFail m_OnMatchFail;
	FOnTick m_OnTimerTick;

	FTimerHandle m_TimerHandle_OnTimer;

	TWeakObjectPtr<AOtherPlayerPawn> m_PVPOtherPlayer;

	BigInt m_PlayerTotalDmg;
	
	BigInt m_OtherPlayerTotalDmg;

	BigInt m_TotalDmg;

	float m_fTimer;

	FDelegateHandle m_LevelLoadHandle;

	FSafeInt m_nWin;

	FSafeInt m_nLose;

	FSafeInt m_nMMR;

	
	
public:
	FString m_OtherPlayerDisplayName;
	UPROPERTY()
	UPlayFabJsonObject* m_StatObj;
	UPROPERTY()
	UPlayFabJsonObject* m_SkillObj;
	UPROPERTY()
	UPlayFabJsonObject* m_EquipObj;

	int m_nCurrentWingTicket;
public:
	
	virtual void OnLevelLoadComplete(UWorld* world) override;

	virtual void StartDungeon() override;

	virtual void EndDungeon(bool b) override;

	virtual void Tick(float deltaTime) override;
	
	
	void UpdateGauge();
	
	void RequestPVPMatching();

	void OnRequestComplete(const PlayFab::ClientModels::FGetLeaderboardAroundPlayerResult& rslt);

	void OnGetOtherPlayerSuccess(const PlayFab::ClientModels::FGetUserDataResult&);
	
	void MatchFail();

	void PVPEnd();

	void AddPlayerTotalDamage(const BigInt& v);

	void AddOtherPlayerTotalDamage(const BigInt& v);

	void SpawnPVPPlayer(UWorld* world);

	virtual AUnitPawn* GetNearestEnemy(const FVector& wantPos) override;

	void MoveToPVPDungeon();

	virtual FString GetOpenLevelAssetName() override;

	virtual bool IsBattleStarted() override;

	void SetPVPData(const FString& jsonStr);

	void SetPVPDataBeforeUpload(const FString& jsonStr);

	int GetMMR()
	{
		return m_nMMR.GetValue();
	}

	int GetWin()
	{
		return m_nWin.GetValue();
	}

	int GetLose()
	{
		return m_nLose.GetValue();
	}
};
