#include "PVPManager.h"
#include "DiabloGameInstance.h"
#include "JsonSerializer.h"
#include "PlayFabJsonObject.h"

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

	m_EquipObj = PlayfabJson->GetObjectField(TEXT("CurrentEquipped"));

	m_OnMatchSuccessed.Broadcast(m_StatObj,m_EquipObj);
	//
	UDiabloGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(m_TimerHandle_OnTimer, this, &UPVPManager::MoveStageLevelToPVP,1.7f,false);
}

void UPVPManager::MatchFail()
{
	PRINTF("MatchFail");
}

void UPVPManager::MoveStageLevelToPVP()
{
	UGameplayStatics::OpenLevel(UDiabloGameInstance::Get->GetWorld(),TEXT("PVPStage"), true);

	
}

