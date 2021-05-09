// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "MonsterSpawnManager.h"
#include "PlayFabClientDataModels.h"
#include "Characters/Pawns/OtherPlayerPawn.h"
#include "Datas/ItemTierData.h"
#include "UObject/NoExportTypes.h"
#include "PVPManager.generated.h"

class UPlayFabJsonObject;
/**
 * 
 */
UCLASS()
class DIABLOM_API UPVPManager : public UMonsterSpawnManager
{
	GENERATED_BODY()
public:
	DECLARE_DELEGATE_ThreeParams(FOnPVPMatched,UPlayFabJsonObject*,UPlayFabJsonObject*,UPlayFabJsonObject*);
	DECLARE_DELEGATE_OneParam(FOnOtherPlayerFound,const FString);
	DECLARE_DELEGATE(FOnMatchFail);
	DECLARE_DELEGATE(FOnMatchStart);
	DECLARE_DELEGATE_ThreeParams(FOnDmgChanged,float,BigInt,BigInt);
	DECLARE_DELEGATE_OneParam(FOnBattleEnd,bool);
	DECLARE_DELEGATE_OneParam(FOnOtherPlayerSpawned,AOtherPlayerPawn*);

	UPVPManager();

	FOnOtherPlayerSpawned m_OnOtherPlayerSpawned;

	

	FOnDmgChanged m_OnDmgChanged;

	FOnPVPMatched m_OnMatchSuccessed;

	FOnOtherPlayerFound m_OnOtherPlayerFound;

	FOnMatchFail m_OnMatchFail;

	FOnMatchStart m_OnMatchStart;

	FTimerHandle m_TimerHandle_OnTimer;

	TWeakObjectPtr<AOtherPlayerPawn> m_PVPOtherPlayer;


	BigInt m_PlayerTotalDmg;
	
	BigInt m_OtherPlayerTotalDmg;

	BigInt m_TotalDmg;

	float m_fTimer;

	FDelegateHandle m_LevelLoadHandle;
	
public:
	FString m_OtherPlayerDisplayName;
	UPROPERTY()
	UPlayFabJsonObject* m_StatObj;
	UPROPERTY()
	UPlayFabJsonObject* m_SkillObj;
	UPROPERTY()
	UPlayFabJsonObject* m_EquipObj;

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
};
