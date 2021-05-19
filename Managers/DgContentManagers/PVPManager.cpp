#include "PVPManager.h"
#include "DungeonManager.h"
#include "JsonSerializer.h"
#include "PlayFabJsonObject.h"
#include "Managers/DiabloGameInstance.h"

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
	else //Successed
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
	                                                                 &UPVPManager::MoveToPVPDungeon, 2.2f, false);
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

void UPVPManager::PVPEnd()
{
	bool PlayerWin = m_PlayerTotalDmg.IsGreaterOrEqual(m_OtherPlayerTotalDmg);

	EndDungeon(PlayerWin);
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



void UPVPManager::OnLevelLoadComplete(UWorld* world)
{
	PRINTF("PVPManager! World:%s", *world->GetMapName());
	SpawnPVPPlayer(world);
	StartDungeon();
}

void UPVPManager::StartDungeon()
{
	Super::StartDungeon();

	APlayerDiabloCharacter* PlChar = UDiabloGameInstance::Get->GetPlChar();

	PlChar->ShowNameCard(UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname);

	PlChar->FocusTarget(m_PVPOtherPlayer.Get());

	m_PVPOtherPlayer.Get()->ShowNameCard(m_OtherPlayerDisplayName);

	m_PVPOtherPlayer.Get()->FocusTarget(PlChar);

	m_PlayerTotalDmg = 0;

	m_OtherPlayerTotalDmg = 0;

	m_TotalDmg = 0;

	m_nCurrentWingTicket=0;

	m_fTimer = 0.f;

	m_bIsMatchStarted = true;
}

void UPVPManager::EndDungeon(bool b)
{
	if (m_bIsMatchStarted)
	{
		m_bIsMatchStarted = false;

		if(b)
		{
			++m_nWin;

			m_nCurrentWingTicket = FMath::RandRange(3,5);
		}
		else
		{
			++m_nLose;

			m_nCurrentWingTicket = FMath::RandRange(1,2);
		}

		UDiabloGameInstance::Get->m_GoldManager->AddWingTicket(m_nCurrentWingTicket);		

		APlayerDiabloCharacter* PlChar = UDiabloGameInstance::Get->GetPlChar();

		PlChar->m_bUseFSM = false;

		m_PVPOtherPlayer.Get()->m_bUseFSM = false;

		UDiabloGameInstance::Get->m_PlayfabManager->OnPvPComplete();
	}

	Super::EndDungeon(b);
}

void UPVPManager::Tick(float deltaTime)
{
	if (!m_bIsMatchStarted)
	{
		return;
	}

	Super::Tick(deltaTime);

	m_fTimer += deltaTime;

	float TimeRemain = PVPTIME - m_fTimer;

	m_OnTimerTick.Broadcast(TimeRemain);


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

AUnitPawn* UPVPManager::GetNearestEnemy(const FVector& wantPos)
{
	if (!IsBattleStarted())
	{
		return nullptr;
	}

	return m_PVPOtherPlayer.Get();
}

void UPVPManager::MoveToPVPDungeon()
{
	UDiabloGameInstance::Get->m_DungeonManager->OpenLevel(this);
}

FString UPVPManager::GetOpenLevelAssetName()
{
	return TEXT("PVPStage");
}

bool UPVPManager::IsBattleStarted()
{
	return m_PVPOtherPlayer.Get() && m_bIsMatchStarted;
}

void UPVPManager::SetPVPData(const FString& jsonStr)//cloud return
{
	UPlayFabJsonObject* JsonObj = UPlayFabJsonObject::ConstructJsonObject(this);

	if(!JsonObj->DecodeJson(jsonStr))
	{
		return;
	}

	int W = JsonObj->GetNumberField(TEXT("Win"));
	
	int L = JsonObj->GetNumberField(TEXT("Lose"));

	int M = JsonObj->GetNumberField(TEXT("MMR"));

	m_nWin.SetValue(W);//title
	m_nLose.SetValue(L);//title
	m_nMMR.SetValue(M);//statistic

	m_OnPVPStatusChanged.Broadcast(m_nWin.GetValue(),m_nLose.GetValue(),m_nMMR.GetValue());
}

void UPVPManager::SetPVPDataBeforeUpload(const FString& jsonStr)
{
	SetPVPData(jsonStr);

	UDiabloGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(
		m_TimerHandle_OnTimer, this, &UPVPManager::MoveToNormalDungeon, 4.2f, false);
}
