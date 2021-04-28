#include "PVPCanvas.h"
#include "CommonElement/MaterialProgressBar.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Managers/DiabloGameInstance.h"

void UPVPCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UDiabloGameInstance::Get->m_PVPManager->m_OnDmgChanged.BindUObject(this,&UPVPCanvas::UpdateDmgGauge);
	
	m_TxtPlayerName->SetText(FText::FromString(UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname));
	
	m_TxtOtherPlayerName->SetText(FText::FromString(UDiabloGameInstance::Get->m_PVPManager->m_OtherPlayerDisplayName));

	UDiabloGameInstance::Get->m_PVPManager->m_OnTick.BindUObject(this,&UPVPCanvas::UpdateTimer);

	UDiabloGameInstance::Get->m_PVPManager->m_OnBattleEnd.BindUObject(this,&UPVPCanvas::OnBattleEnd);
	
}

void UPVPCanvas::UpdateDmgGauge(float percentOne, BigInt playerDmg, BigInt otherPlayerDmg)
{
	m_BarPlayerDmg->SetProgressValue(percentOne);
	
	m_TxtPlayerDmgValue->SetText(FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(playerDmg,2)));
	m_TxtOtherPlayerDmgValue->SetText(FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(otherPlayerDmg,2)));
}

void UPVPCanvas::ReqeustPopupText(const FText& str)
{
	m_TxtRequestedInfo->SetText(str);
}

void UPVPCanvas::UpdateTimer(float timer)
{
	m_TxtTimer->SetText(FText::FromString(FString::Printf(TEXT("남은시간:%.1f"),timer)));
}

void UPVPCanvas::OnBattleEnd(bool isPlayerWon)
{
	PlayAnimation(ShowResult);
	
	if(isPlayerWon)
	{
		m_TxtRequestedInfo->SetText(FText::FromString(TEXT("승리")));	
	}
	else
	{
		m_TxtRequestedInfo->SetText(FText::FromString(TEXT("패배")));
	}
	
}
