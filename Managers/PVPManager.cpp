#include "PVPManager.h"
#include "DiabloGameInstance.h"
#include "JsonSerializer.h"

void UPVPManager::RequestPVPMatching()
{
	UDiabloGameInstance::Get->m_PlayfabManager->RequestPVPMatching(
		6,PlayFab::UPlayFabClientAPI::FGetLeaderboardAroundPlayerDelegate::CreateUObject(this,&UPVPManager::OnRequestComplete));	
}

void UPVPManager::OnRequestComplete(const PlayFab::ClientModels::FGetLeaderboardAroundPlayerResult& rslt)//만일 이게 없으면?
{
	//타플레이어들은 이게 안만들어짐
	auto PlayerLeaderBoard = rslt.Leaderboard;
	
	PlayFab::ClientModels::FPlayerLeaderboardEntry PlayerEntry;
	
	PlayerEntry.DisplayName= UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname;
	
    PlayerLeaderBoard.Remove(PlayerEntry);
	
	PlayFab::ClientModels::FPlayerLeaderboardEntry MatchedUser = PlayerLeaderBoard.GetRandom();
	//FGetUsrDataDele::CreateUObject(this, &UPlayfabManager::OnSuccessGetMainData)
	UDiabloGameInstance::Get->m_PlayfabManager->RequestGetOtherPlayerMainData(MatchedUser.PlayFabId,FGetUsrDataDele::CreateUObject(this, &UPVPManager::OnGetOtherPlayerSuccess));
	//MatchedUser.PlayFabId
}

void UPVPManager::OnGetOtherPlayerSuccess(const PlayFab::ClientModels::FGetUserDataResult& rslt)
{
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(rslt.Data[UPlayfabManager::MainData].Value);
	
	TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());

	FString LogoutLoaded;
	FString IAPLoaded;
	FString GachaLoaded;
	FString UpgradeLoaded;
	FString SkillLoaded;
	FString WeaponLoaded;
	FString SkinLoaded;
	FString PetLoaded;

	if (FJsonSerializer::Deserialize(reader, jsonObj) && jsonObj.IsValid())
	{
		//LogoutLoaded = jsonObj->GetStringField(TEXT("Logout"));	
		//IAPLoaded = jsonObj->GetStringField(TEXT("IAP"));
		//GachaLoaded = jsonObj->GetStringField(TEXT("Gacha"));
		UpgradeLoaded = jsonObj->GetStringField(TEXT("Upgrade"));
		SkillLoaded = jsonObj->GetStringField(TEXT("Skill"));
		WeaponLoaded = jsonObj->GetStringField(TEXT("Weapon"));
		SkinLoaded = jsonObj->GetStringField(TEXT("Skin"));
		PetLoaded = jsonObj->GetStringField(TEXT("Pet"));
	}
}
