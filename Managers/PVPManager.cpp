#include "PVPManager.h"
#include "DiabloGameInstance.h"
#include "JsonSerializer.h"
#include "PlayFabJsonObject.h"

UPVPManager::UPVPManager()
{
	m_bIsMatchStarted = false;
}

void UPVPManager::RequestPVPMatching()
{
	m_OnMatchStart.Broadcast();
	UDiabloGameInstance::Get->m_PlayfabManager->RequestPVPMatching(
		6, PlayFab::UPlayFabClientAPI::FGetLeaderboardAroundPlayerDelegate::CreateUObject(
			this, &UPVPManager::OnRequestComplete));
}

void UPVPManager::OnRequestComplete(const PlayFab::ClientModels::FGetLeaderboardAroundPlayerResult& rslt) //만일 이게 없으면?
{
	//타플레이어들은 이게 안만들어짐
	auto PlayerLeaderBoard = rslt.Leaderboard;

	if (PlayerLeaderBoard.Num() < 1)
	{
		m_OnMatchFail.Broadcast();
		return;
	}

	PlayFab::ClientModels::FPlayerLeaderboardEntry PlayerEntry;

	PlayerEntry.DisplayName = UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname;

	PlayerLeaderBoard.Remove(PlayerEntry);

	PlayFab::ClientModels::FPlayerLeaderboardEntry MatchedUser = PlayerLeaderBoard.GetRandom();


	//m_OtherPlayerDisplayName = MatchedUser.DisplayName;
	FString TestID = TEXT("8CAA7224F3D58944");
	
	m_OtherPlayerDisplayName = TEXT("윤빠띠");
	
	m_OnOtherPlayerFound.Broadcast(m_OtherPlayerDisplayName);

	UDiabloGameInstance::Get->m_PlayfabManager->RequestGetOtherPlayerMainData(
		TestID, FGetUsrDataDele::CreateUObject(this, &UPVPManager::OnGetOtherPlayerSuccess));
}

void UPVPManager::OnGetOtherPlayerSuccess(const PlayFab::ClientModels::FGetUserDataResult& rslt)
{
	if (!rslt.Data.Find(UPlayfabManager::MainData))
	{
		m_OnMatchFail.Broadcast();
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

	m_OnMatchSuccessed.Broadcast(m_StatObj,m_SkillObj,m_EquipObj);
	//
	UDiabloGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(m_TimerHandle_OnTimer, this, &UPVPManager::MoveStageLevelToPVP,1.7f,false);
}

void UPVPManager::MatchFail()
{
	PRINTF("MatchFail");
}

void UPVPManager::UpdateGauge()
{
	m_TotalDmg = m_PlayerTotalDmg+m_OtherPlayerTotalDmg;

	BigInt PlayerDmgCache = m_PlayerTotalDmg;

	PlayerDmgCache.MultiplyFast(10);

	PlayerDmgCache.Divide(m_TotalDmg);

	float PercentOne = (float)PlayerDmgCache.ToInt()/10.f;

	m_OnDmgChanged.Broadcast(PercentOne,m_PlayerTotalDmg,m_OtherPlayerTotalDmg);
}

void UPVPManager::MoveStageLevelToPVP()
{
	UGameplayStatics::OpenLevel(UDiabloGameInstance::Get->GetWorld(),TEXT("PVPStage"), true);
	
}

void UPVPManager::PVPStart()
{
	APlayerDiabloCharacter* PlChar = UDiabloGameInstance::Get->GetPlChar();

	PlChar->ShowNameCard(UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname);
	
	PlChar->FocusTarget(m_PVPOtherPlayer.Get());

	m_PVPOtherPlayer.Get()->ShowNameCard(m_OtherPlayerDisplayName);
	
	m_PVPOtherPlayer.Get()->FocusTarget(PlChar);

	m_PlayerTotalDmg=0;

	m_OtherPlayerTotalDmg=0;

	m_TotalDmg=0;

	m_bIsMatchStarted=true;
}

void UPVPManager::PVPEnd()
{
	m_bIsMatchStarted=false;
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

