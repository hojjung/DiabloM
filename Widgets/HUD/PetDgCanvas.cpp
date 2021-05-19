// My First Hack n Slash


#include "PetDgCanvas.h"

#include "Managers/DiabloGameInstance.h"
#include "Widgets/CommonElement/MaterialProgressBar.h"


void UPetDgCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_PetDgManager = UDiabloGameInstance::Get->m_PetDgManager;

	m_PetDgManager->m_OnTickTimer.AddUObject(this,&UPetDgCanvas::UpdateTimer);

	m_Handle = UDiabloGameInstance::Get->GetPlChar()->m_OnRageChanged.AddUObject(this,&UPetDgCanvas::PlayerUpdateRageBar);
	
	UDiabloGameInstance::Get->GetPlChar()->UpdateRage();
	
	m_PetDgManager->m_OnBattleEnd.AddUObject(this,&UPetDgCanvas::OnBattleEnd);

	m_PetDgManager->m_OnMonsterKilled.BindUObject(this,&UPetDgCanvas::UpdateMonsterCount);

	m_MonsterCountBar->SetProgressValue(0.f);
	
	m_TxtMonsterCountRemain->SetText(FText::FromString(TEXT("펫 사냥 시작")));

	m_nBeforeTickets = UDiabloGameInstance::Get->m_GoldManager->GetPetGachaTicket();
}

void UPetDgCanvas::UpdateTimer(float timer)
{
	float PercentOne = timer / PETDGTIME;

	m_BarTime->SetProgressValue(1.f-PercentOne);

	m_TxtTime->SetText(FText::FromString(FString::Printf(TEXT("남은 시간:%.1f"),PETDGTIME - timer)));
}


void UPetDgCanvas::PlayerUpdateRageBar(float cV, float mV)
{
	float PercentOne = cV / mV;
	
	m_RageBar->SetProgressValue(PercentOne);

	m_TxtRageValue->SetText(FText::FromString(UDiabloGameInstance::Get->GetPlChar()->GetRageFormatStr()));
}

void UPetDgCanvas::UpdateMonsterCount()
{
	float PercentOne = m_PetDgManager->GetMobCountPercent();
	
	m_MonsterCountBar->SetProgressValue(PercentOne);
	
	m_TxtMonsterCountRemain->SetText(FText::FromString(m_PetDgManager->GetMobRemainCountStr()));
}



void UPetDgCanvas::OnBattleEnd(bool isPlayerWon)
{
	PlayAnimation(ShowResult);

	if(isPlayerWon)
	{
		m_TxtRequestedInfo->SetText(FText::FromString(TEXT("펫 던전 토벌 성공")));
		
		int ObtainedBounty = m_PetDgManager->GetCurrentReward();
		
		m_ImgTxtGainedPetGachaTickets->SetText( FText::FromString(FString::Printf(TEXT("얻은 펫 티켓:%d"),ObtainedBounty)));

		m_ImgTxtRemainTime->SetText(FText::FromString(FString::Printf(TEXT("남은시간:%.1f"),UDiabloGameInstance::Get->m_PetDgManager->GetRemainTime())));
	}
	else
	{
		m_TxtRequestedInfo->SetText(FText::FromString(TEXT("펫 던전 토벌 실패")));

		m_ImgTxtGainedPetGachaTickets->SetText( FText::FromString(FString::Printf(TEXT("얻은 펫 티켓:0"))));

		m_ImgTxtRemainTime->SetText(FText::FromString(TEXT("시간초과")));
	}

	m_ImgTxtBeforePetGachaTickets->SetText( FText::FromString(FString::Printf(TEXT("이전 펫 티켓:%d"),m_nBeforeTickets)));

	int AfterTickets = UDiabloGameInstance::Get->m_GoldManager->GetPetGachaTicket();
	
	m_ImgTxtAfterPetGachaTickets->SetText( FText::FromString(FString::Printf(TEXT("현재 펫 티켓:%d"),AfterTickets)));
}
