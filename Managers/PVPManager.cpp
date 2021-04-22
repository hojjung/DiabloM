#include "PVPManager.h"
#include "DiabloGameInstance.h"

void UPVPManager::RequestPVPMatching()
{
	UDiabloGameInstance::Get->m_PlayfabManager->RequestPVPMatching(
		6,PlayFab::UPlayFabClientAPI::FGetLeaderboardAroundPlayerDelegate::CreateUObject(this,&UPVPManager::OnRequestComplete));	
}

void UPVPManager::OnRequestComplete(const PlayFab::ClientModels::FGetLeaderboardAroundPlayerResult& rslt)//만일 이게 없으면?
{
	auto PlayerLeaderBoard = rslt.Leaderboard;
	
	PlayFab::ClientModels::FPlayerLeaderboardEntry PlayerEntry;
	
	PlayerEntry.DisplayName= UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname;
	
    PlayerLeaderBoard.Remove(PlayerEntry);
	
	PlayerLeaderBoard.Shrink();
	
	PlayFab::ClientModels::FPlayerLeaderboardEntry MatchedUser = PlayerLeaderBoard.GetRandom();
	
	PRINTF("Matched User:%s",*MatchedUser.DisplayName);

}
