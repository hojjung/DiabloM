#include "PVPManager.h"
#include "DiabloGameInstance.h"
#include "JsonSerializer.h"
#include "PlayFabJsonObject.h"

void UPVPManager::RequestPVPMatching()
{
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
		PRINTF("NoUser,Need AiBOT");
		return;
	}

	PlayFab::ClientModels::FPlayerLeaderboardEntry PlayerEntry;

	PlayerEntry.DisplayName = UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname;

	PlayerLeaderBoard.Remove(PlayerEntry);

	PlayFab::ClientModels::FPlayerLeaderboardEntry MatchedUser = PlayerLeaderBoard.GetRandom();

	UDiabloGameInstance::Get->m_PlayfabManager->RequestGetOtherPlayerMainData(
		MatchedUser.PlayFabId, FGetUsrDataDele::CreateUObject(this, &UPVPManager::OnGetOtherPlayerSuccess));
}

void UPVPManager::OnGetOtherPlayerSuccess(const PlayFab::ClientModels::FGetUserDataResult& rslt)
{
	if (!rslt.Data.Find(UPlayfabManager::MainData))
	{
		PRINTF("PVP-NoData");
		return;
	}

	UPlayFabJsonObject* PlayfabJson = UPlayFabJsonObject::ConstructJsonObject(this);

	if (!PlayfabJson->DecodeJson(rslt.Data[UPlayfabManager::MainData].Value))
	{
		PRINTF("FailDecode");
		return;
	}

	UPlayFabJsonObject* StatObj = PlayfabJson->GetObjectField(TEXT("Stat"));

	int BaseAttack = StatObj->GetNumberField(TEXT("BaseAttack"));

	int Critical = StatObj->GetNumberField(TEXT("Critical"));

	int CriticalDmg = StatObj->GetNumberField(TEXT("CriticalDmg"));

	int SuperCritical = StatObj->GetNumberField(TEXT("SuperCritical"));

	int SuperCriticalDmg = StatObj->GetNumberField(TEXT("SuperCriticalDmg"));

	int MagicBomb = StatObj->GetNumberField(TEXT("MagicBomb"));

	int MagicBombDmg = StatObj->GetNumberField(TEXT("MagicBombDmg"));

	int SuperMagicBomb = StatObj->GetNumberField(TEXT("SuperMagicBomb"));

	int SuperMagicBombDmg = StatObj->GetNumberField(TEXT("SuperMagicBombDmg"));

	UPlayFabJsonObject* EquippedObj = PlayfabJson->GetObjectField(TEXT("CurrentEquipped"));

	int SkinIndex = EquippedObj->GetNumberField(TEXT("EquippedSkin"));

	int WeaponIndex = EquippedObj->GetNumberField(TEXT("EquippedWeapon"));

	int WeaponLevel = EquippedObj->GetNumberField(TEXT("EquippedWeaponLevel"));

	int PetIndex = EquippedObj->GetNumberField(TEXT("EquippedPet"));

	int EquippedPetLevel = EquippedObj->GetNumberField(TEXT("EquippedPetLevel"));

	int EquippedSkill01 = EquippedObj->GetNumberField(TEXT("EquippedSkill01"));
	
	int EquippedSkill01Level = EquippedObj->GetNumberField(TEXT("EquippedSkill01Level"));
	
	int EquippedSkill02 = EquippedObj->GetNumberField(TEXT("EquippedSkill02"));
	
	int EquippedSkill02Level = EquippedObj->GetNumberField(TEXT("EquippedSkill02Level"));
	
	int EquippedSkill03 = EquippedObj->GetNumberField(TEXT("EquippedSkill03"));
	
	int EquippedSkill03Level = EquippedObj->GetNumberField(TEXT("EquippedSkill03Level"));
	
	int EquippedSkill04 = EquippedObj->GetNumberField(TEXT("EquippedSkill04"));
	
	int EquippedSkill04Level = EquippedObj->GetNumberField(TEXT("EquippedSkill04Level"));
}
