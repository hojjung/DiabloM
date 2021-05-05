#include "PVPManager.h"
#include "DiabloGameInstance.h"
#include "DungeonManager.h"
#include "JsonSerializer.h"
#include "PlayFabJsonObject.h"

#define PVPTIME 40

UPVPManager::UPVPManager()
{
	m_bIsMatchStarted = false;

	m_fTimer = 0.f;
}

void UPVPManager::RequestPVPMatching()
{
	m_OnMatchStart.ExecuteIfBound();
	UDiabloGameInstance::Get->m_PlayfabManager->RequestPVPMatching(
		6, PlayFab::UPlayFabClientAPI::FGetLeaderboardAroundPlayerDelegate::CreateUObject(
			this, &UPVPManager::OnRequestComplete));
}

void UPVPManager::OnRequestComplete(const PlayFab::ClientModels::FGetLeaderboardAroundPlayerResult& rslt) //만일 이게 없으면?
{
	//타플레이어들은 이게 안만들어짐
	TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry> PlayerLeaderBoard = rslt.Leaderboard;

	if (PlayerLeaderBoard.Num() <= 1)
	{
		PRINTF("Match Failed but Testing FIX");
		//m_OnMatchFail.ExecuteIfBound();
		//return;
		FString TestID = TEXT("8CAA7224F3D58944");

		m_OtherPlayerDisplayName = TEXT("윤빠띠");

		m_OnOtherPlayerFound.ExecuteIfBound(m_OtherPlayerDisplayName);

		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetOtherPlayerMainData(
			TestID, FGetUsrDataDele::CreateUObject(this, &UPVPManager::OnGetOtherPlayerSuccess));
	}
	else//Successed
	{
		PlayFab::ClientModels::FPlayerLeaderboardEntry PlayerEntry;

		PlayerEntry.DisplayName = UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname;

		PlayerLeaderBoard.Remove(PlayerEntry);

		PlayFab::ClientModels::FPlayerLeaderboardEntry MatchedUser = PlayerLeaderBoard.GetRandom();
		
		m_OtherPlayerDisplayName = MatchedUser.DisplayName;

		m_OnOtherPlayerFound.ExecuteIfBound(m_OtherPlayerDisplayName);

		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetOtherPlayerMainData(
			MatchedUser.PlayFabId, FGetUsrDataDele::CreateUObject(this, &UPVPManager::OnGetOtherPlayerSuccess));
	}
}

void UPVPManager::OnGetOtherPlayerSuccess(const PlayFab::ClientModels::FGetUserDataResult& rslt)
{
	if (!rslt.Data.Find(UPlayfabManager::MainData))
	{
		m_OnMatchFail.ExecuteIfBound();
		return;
	}

	UPlayFabJsonObject* PlayfabJson = UPlayFabJsonObject::ConstructJsonObject(this);

	if (!PlayfabJson->DecodeJson(rslt.Data[UPlayfabManager::MainData].Value))
	{
		PRINTF("FailDecode");
		return;
	}

	m_StatObj = PlayfabJson->GetObjectField(TEXT("Upgrade"));

	m_SkillObj = PlayfabJson->GetObjectField(TEXT("Skill"));

	m_EquipObj = PlayfabJson->GetObjectField(TEXT("CurrentEquipped"));

	m_OnMatchSuccessed.ExecuteIfBound(m_StatObj, m_SkillObj, m_EquipObj);
	//
	UDiabloGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(m_TimerHandle_OnTimer, this,
	                                                                 &UPVPManager::MoveStageLevelToPVP, 1.7f, false);
}

void UPVPManager::MatchFail()
{
	PRINTF("MatchFail");
}

void UPVPManager::UpdateGauge()
{
	m_TotalDmg = m_PlayerTotalDmg + m_OtherPlayerTotalDmg;

	BigInt PlayerDmgCache = m_PlayerTotalDmg;

	PlayerDmgCache.MultiplyFast(10);

	PlayerDmgCache.Divide(m_TotalDmg);

	float PercentOne = (float)PlayerDmgCache.ToInt() / 10.f;

	PercentOne = FMath::Clamp(PercentOne, 0.1f, 0.9f);

	m_OnDmgChanged.ExecuteIfBound(PercentOne, m_PlayerTotalDmg, m_OtherPlayerTotalDmg);
}

void UPVPManager::MoveStageLevelToPVP()
{
	UGameplayStatics::OpenLevel(UDiabloGameInstance::Get->GetWorld(),TEXT("PVPStage"), true);

	m_LevelLoadHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UPVPManager::OnLevelLoad);
}


void UPVPManager::PVPStart()
{
	APlayerDiabloCharacter* PlChar = UDiabloGameInstance::Get->GetPlChar();

	PlChar->ShowNameCard(UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname);

	PlChar->FocusTarget(m_PVPOtherPlayer.Get());

	m_PVPOtherPlayer.Get()->ShowNameCard(m_OtherPlayerDisplayName);

	m_PVPOtherPlayer.Get()->FocusTarget(PlChar);

	m_PlayerTotalDmg = 0;

	m_OtherPlayerTotalDmg = 0;

	m_TotalDmg = 0;

	m_bIsMatchStarted = true;
}

void UPVPManager::PVPEnd()
{
	m_fTimer = 0.f;

	m_bIsMatchStarted = false;

	APlayerDiabloCharacter* PlChar = UDiabloGameInstance::Get->GetPlChar();

	PlChar->m_bUseFSM = false;

	m_PVPOtherPlayer.Get()->m_bUseFSM = false;

	m_OnBattleEnd.ExecuteIfBound(m_PlayerTotalDmg.IsGreaterOrEqual(m_OtherPlayerTotalDmg));

	UDiabloGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(m_TimerHandle_OnTimer, this,
	                                                                 &UPVPManager::MoveStageLevelToNormalDungeon, 2.2f,
	                                                                 false);
}

void UPVPManager::MoveStageLevelToNormalDungeon()
{
	UDiabloGameInstance::Get->m_DungeonManager->OpenLevel();
}


void UPVPManager::AddPlayerTotalDamage(const BigInt& v)
{
	m_PlayerTotalDmg.Add(v);

	UpdateGauge();
}

void UPVPManager::AddOtherPlayerTotalDamage(const BigInt& v)
{
	m_OtherPlayerTotalDmg.Add(v);

	UpdateGauge();
}

void UPVPManager::Tick(float deltaTime)
{
	if (!m_bIsMatchStarted)
	{
		return;
	}

	m_fTimer += deltaTime;

	float TimeRemain = PVPTIME - m_fTimer;

	m_OnTick.ExecuteIfBound(TimeRemain);

	if (m_fTimer > PVPTIME)
	{
		PVPEnd();
	}
}

void UPVPManager::SpawnPVPPlayer(UWorld* world)
{
	FVector Loc = FVector(290, 290, 102);

	FRotator Rot = FRotator(0, 130, 0);

	FActorSpawnParameters Param;

	Param.bNoFail = true;

	m_PVPOtherPlayer = world->SpawnActor<AOtherPlayerPawn>(AOtherPlayerPawn::StaticClass(), Loc, Rot, Param);
	//
	m_PVPOtherPlayer->SetPVPPlayerPawn(UDiabloGameInstance::Get->m_PVPManager->m_StatObj,
	                                   UDiabloGameInstance::Get->m_PVPManager->m_SkillObj,
	                                   UDiabloGameInstance::Get->m_PVPManager->m_EquipObj);

	m_OnOtherPlayerSpawned.ExecuteIfBound(m_PVPOtherPlayer.Get());
}

void UPVPManager::OnLevelLoad(UWorld* world)
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(m_LevelLoadHandle);
	PRINTF("PVPManager! World:%s", *world->GetMapName());
	SpawnPVPPlayer(world);
	PVPStart();
}
